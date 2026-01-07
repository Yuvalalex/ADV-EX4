#include "NormalGameMode.h"

bool NormalGameMode::getInput(int tick, Player& p1, Player& p2, Screen& currentScreen) {
    // Normal mode - just return true to let InputManager handle input normally
    return true;
}

void NormalGameMode::recordEvent(int tick, const std::string& eventData) {
    // Normal mode doesn't record anything
}

void NormalGameMode::onGameEnd(int finalTick) {
    // Nothing special needed
}