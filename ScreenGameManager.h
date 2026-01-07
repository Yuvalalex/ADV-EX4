#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Screen.h"
#include "LevelManager.h"
#include "Player.h"

class ScreenGameManager {
private:
    // Holds all game screens in memory throughout runtime
    std::vector<std::shared_ptr<Screen>> allScreens;

    // Pointer to currently active screen
    Screen* currScreen;

    int currentLevelIdx;

    LevelManager levelManager;

    // Helper to update current screen pointer
    void updateCurrentPointer();

public:
    ScreenGameManager();
    ~ScreenGameManager();

    // Initialization: loads all level files and prepares screens
    void loadAllGameLevels(Player* p1, Player* p2);

    // Level transition logic
    void moveToNextLevel();
    void moveToPrevLevel();
    void restartGame();

    // Getters and info
    Screen* getCurrentScreen();
    bool isGameFinished() const;
    int getCurrentLevelNumber() const;

    // Render current state
    void drawCurrentState(Player* p1, Player* p2);

    LevelManager& getLevelManager() { return levelManager; }
    const std::vector<std::shared_ptr<Screen>>& getAllScreens() const { return allScreens; }
};