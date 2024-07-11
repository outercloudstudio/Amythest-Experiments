#include "dllmain.hpp"

#include <memory>
#include <minecraft/src-deps/core/utility/NonOwnerPointer.hpp>
#include <minecraft/src-vanilla/vanilla_shared/common/world/level/dimension/OverworldDimension.hpp>
#include <minecraft/src-vanilla/vanilla_shared/common/world/level/dimension/VanillaDimensionFactory.hpp>
#include <minecraft/src/common/ActorUniqueID.hpp>
#include <minecraft/src/common/dataloadhelper/DefaultDataLoadHelper.hpp>
#include <minecraft/src/common/nbt/CompoundTag.hpp>
#include <minecraft/src/common/network/ServerNetworkHandler.hpp>
#include <minecraft/src/common/server/ServerPlayer.hpp>
#include <minecraft/src/common/world/actor/Actor.hpp>
#include <minecraft/src/common/world/level/ChunkPos.hpp>
#include <minecraft/src/common/world/level/DimensionManager.hpp>
#include <minecraft/src/common/world/level/ILevel.hpp>
#include <minecraft/src/common/world/level/block/registry/BlockTypeRegistry.hpp>
#include <minecraft/src/common/world/level/chunk/BlockVolume.hpp>
#include <minecraft/src/common/world/level/chunk/ChunkSource.hpp>
#include <minecraft/src/common/world/level/chunk/LevelChunk.hpp>
#include <minecraft/src/common/world/level/dimension/Dimension.hpp>
#include <minecraft/src/common/world/level/dimension/DimensionHeightRange.hpp>
#include <minecraft/src/common/world/level/levelgen/WorldGenerator.hpp>
#include <minecraft/src/common/world/level/levelgen/structure/StructureFeatureRegistry.hpp>
#include <minecraft/src/common/world/phys/Vec2.hpp>

AmethystContext* amethyst;

class ILevel;
class Scheduler;

SafetyHookInline _registerDimensionTypes;

class BiomeArea {};

static std::mutex generateChunkMutex;

class TestGenerator : public WorldGenerator {
   public:
    TestGenerator(Dimension& dimension) : WorldGenerator(dimension, std::make_unique<StructureFeatureRegistry>()) {
        Log::Info("TestGenerator");
    }

    /**@vIndex {11} */
    virtual void loadChunk(LevelChunk& lc, bool forceImmediateReplacementDataLoad) {
        // generateChunkMutex.lock();

        Log::Info("[loadChunk] mPosition: {}, loadState: {}, min: {}, max: {}", lc.mPosition, (int)lc.mLoadState, lc.mMin, lc.mMax);

        BlockVolume blockVolume = BlockVolume(16, 320, 16, BlockTypeRegistry::getDefaultBlockState(HashedString("diamond_block")));

        // Log::Warning("Created BlockVolume! 0x{:x}", (uint64_t)&blockVolume);
        // Log::Warning("BlockVolume blocks vector: 0x{:x} - 0x{:x}", (uint64_t)blockVolume._blocksHolder.data(), (uint64_t)blockVolume._blocksHolder.data()[blockVolume._blocksHolder.size()]);

        // for (int i = 0; i < blockVolume.mWidth * blockVolume.mHeight * blockVolume.mDepth; i++) {
        //     auto location = blockVolume.mBlocks.mBegin + i;

        //     Log::Info("Block Location ${}: 0x{:x}  0x{:x} - 0x{:x}   valid? {}", i, (uint64_t)location, (uint64_t)blockVolume.mBlocks.mBegin, (uint64_t)blockVolume.mBlocks.mEnd, (uint64_t)location >= (uint64_t)blockVolume.mBlocks.mBegin && (uint64_t)location <= (uint64_t)blockVolume.mBlocks.mEnd);

        //     auto block = blockVolume.mBlocks.mBegin[i];

        //     Log::Info("Block ${}: 0x{:x} 0x{:x}", i, (uint64_t)block, (uint64_t)location);
        // }

        // generateChunkMutex.unlock();

        lc.setBlockVolume(&blockVolume, 0);

        lc.mLoadState = ChunkState::Generated;

        if (lc.mGenerator == nullptr) {
            lc.mGenerator = this;
        }
    }

    /**@vIndex {38} */
    virtual void prepareHeights(class BlockVolume& box, class ChunkPos const& chunkPos, bool factorInBeardsAndShavers) { Assert("Unimplemented"); }

    /**@vIndex {39} */
    virtual void prepareAndComputeHeights(class BlockVolume& box, class ChunkPos const& chunkPos, std::vector<short>& ZXheights, bool factorInBeardsAndShavers, int skipTopN) { Assert("Unimplemented"); }

    /**@vIndex {40} */
    virtual class BiomeArea getBiomeArea(class BoundingBox const& area, uint32_t scale) const { Assert("Unimplemented"); }

    /**@vIndex {41} */
    virtual class BiomeSource const& getBiomeSource() const { Assert("Unimplemented"); }

    /**@vIndex {42} */
    virtual struct BlockVolumeDimensions getBlockVolumeDimensions() const { Assert("Unimplemented"); }

    /**@vIndex {43} */
    virtual class BlockPos findSpawnPosition() const {
        return BlockPos(0, 100, 0);
    }

    /**@vIndex {46} */
    virtual void decorateWorldGenLoadChunk(
        class Biome const& biome,
        class LevelChunk& lc,
        class BlockVolumeTarget& target,
        class Random& random,
        class ChunkPos const& pos) const {
        Assert("Unimplemented");
    }

    /**@vIndex {47} */
    virtual void decorateWorldGenPostProcess(
        class Biome const& biome,
        class LevelChunk& lc,
        class BlockSource& source,
        class Random& random) const {
        Assert("Unimplemented");
    }
};

class TestOverworldDimension : public OverworldDimension {
   public:
    TestOverworldDimension(ILevel& level, Scheduler& callbackContext)
        : OverworldDimension(level, callbackContext) {
        Log::Info("TestOverworldDimension");
    };

    virtual std::unique_ptr<class WorldGenerator> createGenerator(const br::worldgen::StructureSetRegistry&) override {
        return std::make_unique<TestGenerator>(*this);
    }
};

OwnerPtr<Dimension> makeTestDimension(ILevel& level, Scheduler& scheduler) {
    return OwnerPtr<Dimension>(std::make_shared<TestOverworldDimension>(level, scheduler));
}

SafetyHookInline __loadNewPlayer;

struct LambdaFields {
    ServerNetworkHandler* serverNetworkHandler;
    ServerPlayer* player;
    CompoundTag** compound;
};

void _loadNewPlayer(LambdaFields* a1) {
    ILevel& level = *a1->serverNetworkHandler->mLevel;
    CompoundTag* actorData = *a1->compound;
    ServerPlayer* player = a1->player;
    DefaultDataLoadHelper dataLoadHelper = DefaultDataLoadHelper();

    if (actorData) {
        player->mInitMethod = ActorInitializationMethod::LOADED;
        player->load(*actorData, dataLoadHelper);
    } else {
        player->mInitMethod = ActorInitializationMethod::SPAWNED;
    }

    // Try and load the dimension from NBT
    if (!player->hasDimension()) {
        DimensionType dimId = DimensionType(3);  // VanillaDimensions::Undefined

        // load from compoundTag
        if (actorData && actorData->contains("DimensionId")) {
            dimId = DimensionType(actorData->getInt("DimensionId"));
        }

        dimId = level.getLastOrDefaultSpawnDimensionId(dimId);
        WeakRef<Dimension> dimension = level.getOrCreateDimension(dimId);

        if (!dimension) {
            Assert("Failed to get or create dimension for loading player which had a serialzed dimension id {}", dimId.runtimeID);
        }

        player->setDimension(dimension);
    }

    if (!player->hasDimension()) {
        Assert("Could not get or create dimension when loading the player");
    }

    const Dimension& dimension = player->getDimensionConst();
    const BlockPos& defaultSpawn = level.getDefaultSpawn();

    // the defaultSpawn BlockPos seems to get validated before doing this, emitting for now..
    // seems to be just a check for: bug 475722 (which is hidden to public)
    player->moveTo(Vec3(defaultSpawn), Vec2(0.0, 0.0));
    player->prepareRegion(*dimension.mChunkSource.get());

    if (actorData) {
        player->reload();
        player->load(*actorData, dataLoadHelper);
    }
}

void registerDimensionTypes(OwnerPtrFactory<Dimension, ILevel&, Scheduler&>* factory, void* a, void* b, void* c) {
    //_registerDimensionTypes.call(factory, a, b, c);

    // register a dimension with the overworld name because custom names don't seem to get created
    // I suspect they are registered on demand when loading into the dimension.
    factory->registerFactory("overworld", makeTestDimension);

    Log::Info("registerDimensionTypes 0x{:x} 0x{:x} 0x{:x} 0x{:x}", (uintptr_t)factory, (uintptr_t)a, (uintptr_t)b, (uintptr_t)c);
}

SafetyHookInline _getOrCreateDimension;

WeakRef<Dimension>* getOrCreateDimension(DimensionManager* self, WeakRef<Dimension>* result, DimensionType dimType) {
    Log::Info("getOrCreateDimension, creating dimType {:d}", dimType.runtimeID);

    result = _getOrCreateDimension.call<WeakRef<Dimension>*>(self, result, dimType);

    Log::Info("getOrCreateDimension result: 0x{:x}", (uint64_t)result->get());
    return result;
}

class SubChunk;

// SafetyHookInline _setFromBlockVolume;

// void setFromBlockVolume(SubChunk* _this, BlockVolume* blockVolume, short param2) {
//     Log::Info("setFromBlockVolume _this: 0x{:x} blockVolume: {:x} param2: {}", (uint64_t)_this, (uint64_t)blockVolume, param2);

//     _setFromBlockVolume.call(_this, blockVolume, param2);
// }

// SafetyHookInline _copyMemorySpan;

// void copyMemorySpan(void** dstSpan, unsigned long long count, void** srcSpan, long long* srcSpanEnd) {
//     Log::Info("copyMemorySpan dstSpan: 0x{:x} count: {} srcSpan: 0x{:x} srcSpanEnd: 0x{:x}", (uint64_t)dstSpan, (uint64_t)count, (uint64_t)srcSpan, (uint64_t)srcSpanEnd);

//     size_t size = *srcSpanEnd - (long long)*srcSpan;

//     Log::Info("Size: {}", size);

//     _copyMemorySpan.call(dstSpan, count, srcSpan, srcSpanEnd);

//     Log::Info("Success with size: {}", size);
// }

SafetyHookInline _findHighestNonAirBlock;

short findHighestNonAirBlock(BlockVolume* _this) {
    Log::Warning("findHighestNonAirBlock _this: 0x{:x}", (uint64_t)_this);

    short result = _findHighestNonAirBlock.call<short>(_this);

    Log::Info("Result: {}", result);

    // for (int i = 0; i < _this->mWidth * _this->mHeight * _this->mDepth; i++) {
    //     auto location = _this->mBlocks.mBegin + i;

    //     Log::Info("Block Location ${}: 0x{:x}", i, (uint64_t)location);

    //     auto block = _this->mBlocks.mBegin[i];

    //     Log::Info("Block ${}: 0x{:x}", i, (uint64_t)block);
    // }

    return result;
}

ModFunction void Initialize(AmethystContext* _amethyst) {
    InitializeVtablePtrs();
    amethyst = _amethyst;

    HookManager& hooks = amethyst->mHookManager;

    hooks.RegisterFunction<&findHighestNonAirBlock>("? 89 ? ? ? ? 89 ? ? ? ? 89 ? ? ? 41 ? 48 83 ? ? 48 ? ? 4C 8B ? 48 ? ? ? 33");
    hooks.CreateHook<&findHighestNonAirBlock>(_findHighestNonAirBlock, &findHighestNonAirBlock);

    // hooks.RegisterFunction<&copyMemorySpan>("48 85 ? 0F ? ? ? ? ? 56 41 ? 41 ? 48 83 ? ? 48 B8 ? ? ? ? ? ? ? ? 4D 8B ? 4D 8B ? 48 8B ? 48 3B ? 77 ? ? 89 ? ? ? 48 ? ? ? ? ? ? ? 48 8B ? ? 89 ? ? ? E8 ? ? ? ? ? 89 ? 48 8B ? ? 89 ? ? 48 ? ? ? ? 89 ? ? 48 8B ? 49 ? ? 49 ? ? 48 2B ? 4C 8B ? E8 ? ? ? ? 48 C1 ? ? 48 ? ? ? 48 ? ? ? ? 48 ? ? ? ? ? 89 ? ? 48 83 ? ? 41 ? 41 ? 5E ? E8 ? ? ?");
    // hooks.CreateHook<&copyMemorySpan>(_copyMemorySpan, &copyMemorySpan);

    // hooks.RegisterFunction<&setFromBlockVolume>("? 89 ? ? ? 55 56 57 48 8B ? 48 83 ? ? 48 8B ? ? ? ? ? 48 33 ? ? 89 ? ? 41 0F ? ? 48 8B ? 0F 57");
    // hooks.CreateHook<&setFromBlockVolume>(_setFromBlockVolume, &setFromBlockVolume);

    hooks.CreateHookAbsolute(_registerDimensionTypes, SlideAddress(0x40818E0), &registerDimensionTypes);
    hooks.CreateHookAbsolute(__loadNewPlayer, SlideAddress(0x174AFE0), &_loadNewPlayer);

    hooks.RegisterFunction<&DimensionManager::getOrCreateDimension>("48 89 5C 24 ? 44 89 44 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 48 8B FA 4C 8B F9");
    hooks.CreateHook<&DimensionManager::getOrCreateDimension>(_getOrCreateDimension, &getOrCreateDimension);
}