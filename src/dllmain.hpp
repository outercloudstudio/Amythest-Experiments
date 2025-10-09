#pragma once
#include <Windows.h>

#include <amethyst/runtime/ModContext.hpp>
#include <amethyst/runtime/events/GameEvents.hpp>
#include <amethyst/runtime/mod/Mod.hpp>

#define ModFunction extern "C" __declspec(dllexport)

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    return TRUE;
}

void OnStartJoinGame(OnStartJoinGameEvent& event);

class OverworldGeneratorMultinoise {};
class OverworldGenerator {};

class BuildParameters {
   public:
    void* biome;                          // +0x00
    void* random;                         // +0x08
    void* blockVolume;                    // +0x10
    BlockPos* blockPos;                   // +0x18
    float noiseValue;                     // +0x20
    short seaLevel;                       // +0x24
    std::unique_ptr<void>* surfaceNoise;  // +0x28
    int waterLevelStrategy;               // +0x30
    int surfaceHeight;                    // +0x34
    void* heightmapWrapper;               // +0x38
    bool cavesAndCliffsCompatible;

    BuildParameters(
        void* biome,
        void* random,
        void* blockVolume,
        BlockPos* blockPos,
        float noiseValue,
        short seaLevel,
        std::unique_ptr<void>* surfaceNoise,
        int waterLevelStrategy,
        int surfaceHeight,
        void* heightmapWrapper,
        bool cavesAndCliffsCompatible)
        : biome(biome), random(random), blockVolume(blockVolume), blockPos(blockPos), noiseValue(noiseValue), seaLevel(seaLevel), surfaceNoise(surfaceNoise), waterLevelStrategy(waterLevelStrategy), surfaceHeight(surfaceHeight), heightmapWrapper(heightmapWrapper), cavesAndCliffsCompatible(cavesAndCliffsCompatible) {
    }
};

static_assert(offsetof(BuildParameters, biome) == 0x00);
static_assert(offsetof(BuildParameters, random) == 0x08);
static_assert(offsetof(BuildParameters, blockVolume) == 0x10);
static_assert(offsetof(BuildParameters, blockPos) == 0x18);
static_assert(offsetof(BuildParameters, noiseValue) == 0x20);
static_assert(offsetof(BuildParameters, seaLevel) == 0x24);
static_assert(offsetof(BuildParameters, surfaceNoise) == 0x28);