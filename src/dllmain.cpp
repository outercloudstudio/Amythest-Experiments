#include "dllmain.hpp"

#include <mc/src/common/world/level/ChunkPos.hpp>
#include <mc/src/common/world/level/chunk/LevelChunk.hpp>

SafetyHookInline _OverworldGenerator_buildSurfaces;

void OverworldGenerator_buildSurfaces(OverworldGenerator* self, BuildParameters* buildParameters) {
    // Log::Info("buildsurfaces! modified 0x{}", (uintptr_t)buildParameters);

    // buildParameters->surfaceHeight = 60;
    // buildParameters->seaLevel = 200;
    buildParameters->blockPos->x = 0;

    _OverworldGenerator_buildSurfaces.fastcall<void>(self, buildParameters);
}

SafetyHookInline _OverworldGenerator_loadChunk;

void OverworldGenerator_loadChunk(OverworldGenerator* self, LevelChunk* levelChunk, bool forceImmediateReplacementDataLoad) {
    Log::Info("load chunk! 0x{:X}", (uintptr_t)self);

    void** vtable = *(void***)self;
    void* function = vtable[0x1c0 / sizeof(void*)];

    Log::Info("Function at {:p}", function);

    DebugBreak();

    _OverworldGenerator_buildSurfaces.fastcall<void>(self, levelChunk, forceImmediateReplacementDataLoad);
}

ModFunction void Initialize(AmethystContext& ctx, const Amethyst::Mod& mod) {
    Amethyst::InitializeAmethystMod(ctx, mod);

    Amethyst::HookManager& hooks = Amethyst::GetHookManager();

    hooks.CreateHookAbsolute(_OverworldGenerator_buildSurfaces, SigScan("48 8B CA E9 ? ? ? ? CC CC CC CC CC CC CC CC 48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56"), &OverworldGenerator_buildSurfaces);
    hooks.CreateHookAbsolute(_OverworldGenerator_loadChunk, SigScan("? 89 ? ? ? 55 56 57 41 ? 41 ? 41 ? 41 ? 48 ? ? ? ? ? ? ? 48 81 ? ? ? ? ? 48 8B ? ? ? ? ? 48 33 ? ? 89 ? ? ? ? ? 48 8B ? 4C 8B ? 4C ? ? ? 48 81"), &OverworldGenerator_loadChunk);
}
