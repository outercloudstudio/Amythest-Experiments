#include "dllmain.hpp"

#include <mc/src/common/world/level/ChunkPos.hpp>
#include <mc/src/common/world/level/chunk/LevelChunk.hpp>

SafetyHookInline _OverworldGeneratorMultinoise_$ctor;

OverworldGeneratorMultinoise* OverworldGeneratorMultinoise_$ctor(OverworldGeneratorMultinoise* self, void* dimension, void* levelSeed, void* biome) {
    // Log::Info("CONSTRUCTING OVERWORLD GENERATOR MULTINOISE! {}", (void*)self);

    return _OverworldGeneratorMultinoise_$ctor.fastcall<OverworldGeneratorMultinoise*>(self, dimension, levelSeed, biome);
}

SafetyHookInline _OverworldGeneratorMultinoise_generateDensityCellsForChunk;

void* OverworldGeneratorMultinoise_generateDensityCellsForChunk(OverworldGeneratorMultinoise* self, ChunkPos* chunkPos) {
    // Log::Info("generateDensityCellsForChunk! {} {}", chunkPos->x, chunkPos->z);

    return _OverworldGeneratorMultinoise_generateDensityCellsForChunk.fastcall<void*>(self, chunkPos);
}

SafetyHookInline _OverworldGeneratorMultinoise_loadChunk;

void OverworldGeneratorMultinoise_loadChunk(OverworldGeneratorMultinoise* self, LevelChunk* levelChunk) {
    // Log::Info("loadChunk!");

    _OverworldGeneratorMultinoise_loadChunk.fastcall<void>(self, levelChunk);
}

SafetyHookInline _OverworldGenerator_buildSurfaces;

void OverworldGenerator_buildSurfaces(OverworldGenerator* self, BuildParameters* buildParameters) {
    Log::Info("buildsurfaces! 0x{}", (uintptr_t)buildParameters);

    DebugBreak();

    _OverworldGenerator_buildSurfaces.fastcall<void>(self, buildParameters);
}

// SafetyHookInline _BiomeComponentStorage_tryGetComponent;

// void* BiomeComponentStorage_tryGetComponent(void* a) {
//     void* result = _BiomeComponentStorage_tryGetComponent.fastcall<void*>(a);

//     Log::Info("build build Parameters at {:p}", result);

//     DebugBreak();

//     return result;
// }

class SurfaceBuilderComponent {
   public:
    std::byte padding0[0x8];
    void* mSurfaceBuilder;
};

SafetyHookInline _tryGetComponent_SurfaceBuilderComponent;
SurfaceBuilderComponent* tryGetComponent_SurfaceBuilderComponent(void* self) {
    auto* result = _tryGetComponent_SurfaceBuilderComponent.call<SurfaceBuilderComponent*, void*>(self);
    // Log::Info("Some SurfaceBuilder vtable address: 0x{:x}", GetVtable(result->mSurfaceBuilder));
    return result;
}

ModFunction void Initialize(AmethystContext& ctx, const Amethyst::Mod& mod) {
    Amethyst::InitializeAmethystMod(ctx, mod);

    Amethyst::HookManager& hooks = Amethyst::GetHookManager();

    hooks.CreateHookAbsolute(_tryGetComponent_SurfaceBuilderComponent, SigScan("40 53 48 83 EC 20 65 48 8B 04 25 58 00 00 00 48 8B D9 B9 28 00 00 00 48 8B 10 8B 04 11 39 05 95 13 B5 02 7F 33 48 8B 03 48 8B 4B 08 0F B7 15 89 13 B5 02 48 3B C1 74 0E ? ? ? 74 11 48 83 C0 10 48 3B C1 75 F2 33 C0 48 83 C4 20 5B ? 48 8B 40 08 48 83 C4 20 5B ? 48 8D 0D 59 13 B5 02 E8 8C 5F"), &tryGetComponent_SurfaceBuilderComponent);
    hooks.CreateHookAbsolute(_OverworldGenerator_buildSurfaces, SigScan("48 8B CA E9 ? ? ? ? CC CC CC CC CC CC CC CC 48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56"), &OverworldGenerator_buildSurfaces);

    //? 89 ? ? ? 57 48 83 ? ? ? B9 ? ? ? ? ? 48 8B ? 48 8B ? 0F ? ? ? ? ? 0F
}
