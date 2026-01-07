#include "SaveGameMode.h"
#include <fstream>
#include <iostream>

bool SaveGameMode::getInput(int tick, Player& p1, Player& p2, Screen& currentScreen) {
    // Save mode - return true to let normal input happen
    return true;
}

void SaveGameMode::recordEvent(int tick, const std::string& eventData) {
    std::string eventLine = "TICK:" + std::to_string(tick) + "," + eventData;
    events.push_back(eventLine);
}

void SaveGameMode::onGameEnd(int finalTick) {
    saveStepsFile();
    saveEventsFile();
    std::cout << "Game saved to adv-world.steps and adv-world.result" << std::endl;
}

bool SaveGameMode::saveStepsFile() const {
    std::ofstream file("adv-world.steps");
    if (!file.is_open()) {
        std::cerr << "Failed to save adv-world.steps" << std::endl;
        return false;
    }

    file << "SCREENS:adv-world_01.screen\n";
    file << "SEED:12345\n";

    for (const auto& step : steps) {
        file << step << "\n";
    }

    file.close();
    return true;
}

bool SaveGameMode::saveEventsFile() const {
    std::ofstream file("adv-world.result");
    if (!file.is_open()) {
        std::cerr << "Failed to save adv-world.result" << std::endl;
        return false;
    }

    for (const auto& event : events) {
        file << event << "\n";
    }

    file.close();
    return true;
}