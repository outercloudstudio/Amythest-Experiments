#include "dllmain.hpp"

#include <mc/src/common/world/level/ChunkPos.hpp>

SafetyHookInline _OverworldGeneratorMultinoise_$ctor;

OverworldGeneratorMultinoise* OverworldGeneratorMultinoise_$ctor(OverworldGeneratorMultinoise* self, void* dimension, void* levelSeed, void* biome) {
    Log::Info("CONSTRUCTING OVERWORLD GENERATOR MULTINOISE! {}", (void*)self);

    return _OverworldGeneratorMultinoise_$ctor.fastcall<OverworldGeneratorMultinoise*>(self, dimension, levelSeed, biome);
}

SafetyHookInline _OverworldGeneratorMultinoise_generateDensityCellsForChunk;

void* OverworldGeneratorMultinoise_generateDensityCellsForChunk(OverworldGeneratorMultinoise* self, ChunkPos* chunkPos) {
    Log::Info("generateDensityCellsForChunk! {} {}", chunkPos->x, chunkPos->z);

    return _OverworldGeneratorMultinoise_generateDensityCellsForChunk.fastcall<void*>(self, chunkPos);
}

ModFunction void Initialize(AmethystContext& ctx, const Amethyst::Mod& mod) {
    Amethyst::InitializeAmethystMod(ctx, mod);

    Log::Info("Hello, Amethyst World! {}", (void*)SigScan("40 ? 55 56 57 41 ? 48 81 ? ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? ? 89 ? ? ? ? ? ? 4D 8B ? 48 8B ? 48 8B ? 0F 57 ? ? 11 ? ? ? ? 11"));

    Amethyst::HookManager& hooks = Amethyst::GetHookManager();

    hooks.CreateHookAbsolute(_OverworldGeneratorMultinoise_$ctor, SigScan("? 89 ? ? ? 55 56 57 41 ? 41 ? 41 ? 41 ? 48 ? ? ? ? ? ? ? 48 81 ? ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? ? 89 ? ? ? ? ? ? 89 ? ? ? 49 8B ? 48 8B ? ? 89 ? ? ? 48 8B ? ? 89 ? ? ? 45 33 ? E8 ? ? ? ? ? 48 8D ? ? ? ? ? ? 89 ? 48 8D ? ? ? ? ? ? 89 ? ? 4C ? ? ? ? ? ? ? C6"), &OverworldGeneratorMultinoise_$ctor);
    hooks.CreateHookAbsolute(_OverworldGeneratorMultinoise_generateDensityCellsForChunk, SigScan("40 ? 55 56 57 41 ? 48 81 ? ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? ? 89 ? ? ? ? ? ? 4D 8B ? 48 8B ? 48 8B ? 0F 57 ? ? 11 ? ? ? ? 11"), &OverworldGeneratorMultinoise_generateDensityCellsForChunk);
}
