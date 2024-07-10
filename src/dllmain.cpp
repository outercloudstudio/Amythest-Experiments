#include "dllmain.hpp"

// Ran when the mod is loaded into the game by AmethystRuntime
ModFunction void Initialize(AmethystContext *ctx)
{
    // Logging from <Amethyst/Log.h>
    Log::Info("Hello, Amethyst World!");

    // Add a listener to a built in amethyst event
    ctx->mEventManager.onStartJoinGame.AddListener(&OnStartJoinGame);

    byte patch[4] = {0xF8, 0xF8, 0xF8, 0xF8};
    ctx->mPatchManager.ApplyPatch(SlideAddress(0x436EFEA), patch, 4);
}

// Subscribed to amethysts on start join game event in Initialize
void OnStartJoinGame(ClientInstance *client)
{
    Log::Info("The player has joined the game!");
}