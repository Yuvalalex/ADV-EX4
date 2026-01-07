#include "ScreenGameManager.h"
#include "ErrorManager.h"
#include <iostream>

ScreenGameManager::ScreenGameManager()
    : currScreen(nullptr), currentLevelIdx(0) {}

ScreenGameManager::~ScreenGameManager() {
    allScreens.clear();
}

void ScreenGameManager::loadAllGameLevels(Player* p1, Player* p2) {
    if (!levelManager.loadLevelsFromDirectory()) {
        ErrorManager::logError("ScreenGameManager: No level files found in directory.");
        return;
    }

    const auto& files = levelManager.getAllFiles();
    allScreens.clear();
    int levelCounter = 1;

    //   Load levels WITHOUT setting player positions yet
    for (const auto& filename : files) {
        auto newScreen = std::make_shared<Screen>();
        newScreen->setLevel(levelCounter++);

        // Load board with NULL players - positions will be set later
        if (!newScreen->setBoardLevel(filename, nullptr, nullptr)) {
            ErrorManager::logError("ScreenGameManager: Failed to parse level file: " + filename);
        }

        allScreens.push_back(newScreen);
    }

    // Start at first level
    restartGame();

    //  set player positions for FIRST level only
    if (currScreen && p1 && p2) {
        currScreen->setBoardLevel(levelManager.getCurrentLevelFile(), p1, p2);
    }
}

void ScreenGameManager::updateCurrentPointer() {
    if (!allScreens.empty() &&
        currentLevelIdx >= 0 &&
        currentLevelIdx < static_cast<int>(allScreens.size())) {
        currScreen = allScreens[currentLevelIdx].get();
    }
    else {
        currScreen = nullptr;
    }
}


void ScreenGameManager::restartGame() {
    currentLevelIdx = 0;
    levelManager.setLevelIndex(0);  
    updateCurrentPointer();
}

void ScreenGameManager::moveToNextLevel() {
    currentLevelIdx++;
    levelManager.advanceLevel();  
    updateCurrentPointer();
}

void ScreenGameManager::moveToPrevLevel() {
    if (currentLevelIdx > 0) {
        currentLevelIdx--;
        levelManager.goBackLevel();  
    }
    updateCurrentPointer();
}

Screen* ScreenGameManager::getCurrentScreen() {
    return currScreen;
}

bool ScreenGameManager::isGameFinished() const {
    return currentLevelIdx >= static_cast<int>(allScreens.size());
}

int ScreenGameManager::getCurrentLevelNumber() const {
    return currentLevelIdx + 1;
}

void ScreenGameManager::drawCurrentState(Player* p1, Player* p2) {
    if (currScreen) {
        if (currScreen->getTime() == 0) {
            currScreen->reprintFullBoard(*p1, *p2);
        }
        else {
            currScreen->updateVisuals();
        }
    }
}