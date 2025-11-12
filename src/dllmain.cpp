#include "dllmain.hpp"

#include <cstdlib>
#include <ctime>
#include <mc/src/common/world/level/ChunkPos.hpp>
#include <mc/src/common/world/level/chunk/LevelChunk.hpp>

SafetyHookInline _PerlinSimplexNoise_ctor;

PerlinSimplexNoise* PerlinSimplexNoise_ctor(PerlinSimplexNoise* self, void* iRandom, int a, bool b) {
    PerlinSimplexNoise* result = _PerlinSimplexNoise_ctor.fastcall<PerlinSimplexNoise*>(self, iRandom, a, b);

    return result;
}

SafetyHookInline _PerlinSimplexNoise_getValue;

float PerlinSimplexNoise_getValue(PerlinSimplexNoise* self, float x, float y) {
    float result = _PerlinSimplexNoise_getValue.fastcall<float>(self, x, y);

    float random = static_cast<float>(rand()) / RAND_MAX;

    return result;
    // return random * 2.0f - 1.0f;
}

SafetyHookInline _Biome_getTemperatureWorldGen;

float Biome_getTemperatureWorldGen(void* self, BlockPos position, short a) {
    float result = _Biome_getTemperatureWorldGen.fastcall<float>(self, position, a);

    return 0.0f;

    // return result;
}

SafetyHookInline _PerlinNoise_getRegion;

void PerlinNoise_getRegion(void* self, float* outputBuffer, Vec3 startPos, int sizeX, int sizeY, int sizeZ, Vec3 scale) {
    _PerlinNoise_getRegion.fastcall<void>(self, outputBuffer, startPos, sizeX, sizeY, sizeZ, scale);

    // Log::Info("Perlin Noise Read {:x}", (uintptr_t)outputBuffer);
    // for (int i = 0; i < sizeX * sizeY * sizeZ; i++) {
    //     outputBuffer[i] = 10000.0f;
    //     // Log::Info("Perlin Noise Read {}", outputBuffer[i]);
    // }

    // DebugBreak();
}

SafetyHookInline _OverworldGeneratorMultinoise__generateDensityCellsForChunk;

void OverworldGeneratorMultinoise__generateDensityCellsForChunk(void* self, float* output, void* worldgenCache, void* noodleCavifier, void* oreVeinifier) {
    _OverworldGeneratorMultinoise__generateDensityCellsForChunk.fastcall<void>(self, output, worldgenCache, noodleCavifier, oreVeinifier);

    Log::Info("Generating Density Cells {:x}", (uintptr_t)output);

    DebugBreak();
}

ModFunction void Initialize(AmethystContext& ctx, const Amethyst::Mod& mod) {
    Amethyst::InitializeAmethystMod(ctx, mod);

    Amethyst::HookManager& hooks = Amethyst::GetHookManager();

    hooks.CreateHookAbsolute(_PerlinSimplexNoise_ctor, SigScan("? ? ? ? ? 53 55 56 41 56 48 83 EC 28 48 8B 41 18 4C 8B F2 48 2B 41 08 48 8B F1 48 63 11 41"), &PerlinSimplexNoise_ctor);
    hooks.CreateHookAbsolute(_PerlinSimplexNoise_getValue, SigScan("48 8B C4 ? ? ? ? ? ? ? ? ? ? 48 81 EC B8 00 00 00 4C 63 11 0F 28 C2 ? ? ? ? 0F 57 F6 ? ? ? ? ? ? ? ? ? ? ? 45 0F 57 ED ? ? ? ? ? ? F3 44"), &PerlinSimplexNoise_getValue);
    hooks.CreateHookAbsolute(_Biome_getTemperatureWorldGen, SigScan("? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 57 48 83 EC 60 48 8B F1 ? ? ? ? ? 48 81 C1 60 01 00 00 41 8D 58 01 48 8B FA E8"), &Biome_getTemperatureWorldGen);
    hooks.CreateHookAbsolute(_PerlinNoise_getRegion, SigScan("48 8B C4 ? ? ? ? ? ? ? ? ? ? ? ? 41 54 41 56 41 57 48 81 EC A0 00 00 00 44 8B B4 24 E0 00 00 00 4D 8B E0 44"), &PerlinNoise_getRegion);
    hooks.CreateHookAbsolute(_OverworldGeneratorMultinoise__generateDensityCellsForChunk, SigScan("48 8B C4 ? ? ? ? ? ? ? ? ? ? ? ? 41 54 41 56 41 57 48 81 EC A0 00 00 00 44 8B B4 24 E0 00 00 00 4D 8B E0 44"), &OverworldGeneratorMultinoise__generateDensityCellsForChunk);

    srand(time(0));
}
