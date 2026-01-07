#pragma once
#include "GameMode.h"
#include <vector>
#include <string>

// Game mode that records all inputs and events to files
class SaveGameMode : public IGameMode {
private:
    std::vector<std::string> steps;
    std::vector<std::string> events;

public:
    SaveGameMode() = default;
    ~SaveGameMode() override = default;

    bool getInput(int tick, Player& p1, Player& p2, Screen& currentScreen) override;
    void recordEvent(int tick, const std::string& eventData) override;
    void onGameEnd(int finalTick) override;
    bool shouldRender() const override { return true; }
    bool shouldSkipDelays() const override { return false; }
    std::string getModeName() const override { return "SAVE"; }

    // Accessors for testing
    const std::vector<std::string>& getSteps() const { return steps; }
    const std::vector<std::string>& getEvents() const { return events; }

private:
    bool saveStepsFile() const;
    bool saveEventsFile() const;
};