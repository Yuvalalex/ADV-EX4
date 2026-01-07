#pragma once
#include "GameMode.h"
#include <vector>
#include <string>

// Game mode that replays from recorded steps and events
class LoadGameMode : public IGameMode {
private:
    std::vector<std::string> steps;
    std::vector<std::string> events;
    size_t stepIndex;
    bool silent;
    bool loadErrors;

public:
    explicit LoadGameMode(bool silentMode = false);
    ~LoadGameMode() override = default;

    bool getInput(int tick, Player& p1, Player& p2, Screen& currentScreen) override;
    void recordEvent(int tick, const std::string& eventData) override;
    void onGameEnd(int finalTick) override;
    bool shouldRender() const override { return !silent; }
    bool shouldSkipDelays() const override { return silent; }
    std::string getModeName() const override { return silent ? "LOAD_SILENT" : "LOAD"; }

private:
    bool loadStepsFile();
    bool loadEventsFile();
    bool verifyResults(int finalTick) const;
};