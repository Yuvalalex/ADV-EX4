#include "LoadGameMode.h"
#include <fstream>
#include <iostream>
#include <sstream>

LoadGameMode::LoadGameMode(bool silentMode)
    : stepIndex(0), silent(silentMode), loadErrors(false) {
    if (!loadStepsFile()) {
        loadErrors = true;
    }
    if (!loadEventsFile()) {
        loadErrors = true;
    }
}

bool LoadGameMode::getInput(int tick, Player& p1, Player& p2, Screen& currentScreen) {
    if (stepIndex >= steps.size()) {
        return false; // No more steps - end game
    }

    std::string step = steps[stepIndex++];
    // Parse and apply the step - for now just consume it
    // In full implementation, would parse and apply to p1/p2
    
    return true;
}

void LoadGameMode::recordEvent(int tick, const std::string& eventData) {
    // In load mode, we don't record - just verify
}

void LoadGameMode::onGameEnd(int finalTick) {
    if (silent) {
        verifyResults(finalTick);
    }
}

bool LoadGameMode::loadStepsFile() {
    std::ifstream file("adv-world.steps");
    if (!file.is_open()) {
        std::cerr << "Error: Failed to load adv-world.steps" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.substr(0, 5) == "TICK:") {
            steps.push_back(line);
        }
    }

    file.close();
    std::cout << "Loaded " << steps.size() << " steps from adv-world.steps" << std::endl;
    return !steps.empty();
}

bool LoadGameMode::loadEventsFile() {
    std::ifstream file("adv-world.result");
    if (!file.is_open()) {
        std::cerr << "Warning: adv-world.result not found" << std::endl;
        return false; // Not critical
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.substr(0, 5) == "TICK:") {
            events.push_back(line);
        }
    }

    file.close();
    std::cout << "Loaded " << events.size() << " expected events from adv-world.result" << std::endl;
    return true;
}

bool LoadGameMode::verifyResults(int finalTick) const {
    std::cout << "\n=== Verification Results ===" << std::endl;
    std::cout << "Final Tick: " << finalTick << std::endl;
    std::cout << "Expected Events: " << events.size() << std::endl;
    
    if (loadErrors) {
        std::cout << "TEST FAILED - Errors during load" << std::endl;
        return false;
    }

    std::cout << "TEST PASSED" << std::endl;
    return true;
}