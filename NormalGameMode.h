#pragma once
#include "GameMode.h"

// Normal gameplay - keyboard input, full rendering
class NormalGameMode : public IGameMode {
public:
    NormalGameMode() = default;
    ~NormalGameMode() override = default;

    bool getInput(int tick, Player& p1, Player& p2, Screen& currentScreen) override;
    void recordEvent(int tick, const std::string& eventData) override;
    void onGameEnd(int finalTick) override;
    bool shouldRender() const override { return true; }
    bool shouldSkipDelays() const override { return false; }
    std::string getModeName() const override { return "NORMAL"; }
};