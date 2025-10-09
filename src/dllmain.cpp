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

void OverworldGenerator_buildSurfaces(OverworldGenerator* self, void* threadData, BlockVolume* blockVolume, LevelChunk* levelChunk, ChunkPos* chunkPos, void* surfaceLevelCache) {
    // Log::Info("buildsurfaces!");

    _OverworldGenerator_buildSurfaces.fastcall<void>(self, threadData, blockVolume, levelChunk, chunkPos, surfaceLevelCache);
}

SafetyHookInline _BiomeSurfaceSystem_biomeStuff;

void BiomeSurfaceSystem_biomeStuff(void* buildParametersOffset) {
    // Log::Info("build surfaces at! {}", buildParametersOffset);

    ChunkBlockPos chunkBlockPos = *reinterpret_cast<ChunkBlockPos*>(reinterpret_cast<uintptr_t>(buildParametersOffset) - 0x160);

    // DebugBreak();

    Log::Info("build surfaces at! {} {} {}", chunkBlockPos.x, chunkBlockPos.y, chunkBlockPos.z);

    _BiomeSurfaceSystem_biomeStuff.fastcall<void>(buildParametersOffset);
}

ModFunction void Initialize(AmethystContext& ctx, const Amethyst::Mod& mod) {
    Amethyst::InitializeAmethystMod(ctx, mod);

    Log::Info("Hello, Amethyst World! {}", (void*)SigScan("40 ? 55 56 57 41 ? 48 81 ? ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? ? 89 ? ? ? ? ? ? 4D 8B ? 48 8B ? 48 8B ? 0F 57 ? ? 11 ? ? ? ? 11"));

    Amethyst::HookManager& hooks = Amethyst::GetHookManager();

    hooks.CreateHookAbsolute(_OverworldGeneratorMultinoise_$ctor, SigScan("? 89 ? ? ? 55 56 57 41 ? 41 ? 41 ? 41 ? 48 ? ? ? ? ? ? ? 48 81 ? ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? ? 89 ? ? ? ? ? ? 89 ? ? ? 49 8B ? 48 8B ? ? 89 ? ? ? 48 8B ? ? 89 ? ? ? 45 33 ? E8 ? ? ? ? ? 48 8D ? ? ? ? ? ? 89 ? 48 8D ? ? ? ? ? ? 89 ? ? 4C ? ? ? ? ? ? ? C6"), &OverworldGeneratorMultinoise_$ctor);
    hooks.CreateHookAbsolute(_OverworldGeneratorMultinoise_generateDensityCellsForChunk, SigScan("40 ? 55 56 57 41 ? 48 81 ? ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? ? 89 ? ? ? ? ? ? 4D 8B ? 48 8B ? 48 8B ? 0F 57 ? ? 11 ? ? ? ? 11"), &OverworldGeneratorMultinoise_generateDensityCellsForChunk);
    hooks.CreateHookAbsolute(_OverworldGeneratorMultinoise_loadChunk, SigScan("? 89 ? ? ? 55 56 57 41 ? 41 ? 41 ? 41 ? 48 ? ? ? ? ? ? ? 48 81 ? ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? ? 89 ? ? ? ? ? 48 8B ? 4C 8B ? 4C ? ? ? 48 81"), &OverworldGeneratorMultinoise_loadChunk);
    hooks.CreateHookAbsolute(_OverworldGenerator_buildSurfaces, SigScan("48 8B ? 55 53 56 57 41 ? 41 ? 41 ? 41 ? 48 ? ? ? 48 81 ? ? ? ? ? ? 29 ? ? ? 29 ? ? 48 8B ? ? ? ? ? 48 33 ? ? 89 ? ? 48 ? ? ? ? ? ? 4C"), &OverworldGenerator_buildSurfaces);
    hooks.CreateHookAbsolute(_BiomeSurfaceSystem_biomeStuff, SigScan("40 53 48 83 EC 20 65 48 8B 04 25 58 00 00 00 48 8B D9 B9 28 00 00 00 48 8B 10 8B 04 11 39 05 95 13 B5 02 7F 33 48 8B 03 48 8B 4B 08 0F B7 15 89 13 B5 02 48 3B C1 74 0E ? ? ? 74 11 48 83 C0 10 48 3B C1 75 F2 33 C0 48 83 C4 20 5B ? 48 8B 40 08 48 83 C4 20 5B ? 48 8D 0D 59 13 B5 02 E8 8C 5F"), &BiomeSurfaceSystem_biomeStuff);

    //? 89 ? ? ? 57 48 83 ? ? ? B9 ? ? ? ? ? 48 8B ? 48 8B ? 0F ? ? ? ? ? 0F
}
