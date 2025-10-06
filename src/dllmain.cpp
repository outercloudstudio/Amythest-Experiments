#include "dllmain.hpp"

ModFunction void Initialize(AmethystContext& ctx, const Amethyst::Mod& mod) {
    Amethyst::InitializeAmethystMod(ctx, mod);

    Log::Info("Hello, Amethyst World!");
}
