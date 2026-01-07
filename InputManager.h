#pragma once
#include "MenuManager.h"
#include "Player.h"
#include "EntityManager.h"
#include "Screen.h"
#include "Constants.h"
#include "ScreenGameManager.h"

class InputManager {
private:
    MenuManager& menuManager;

    // Process a single key press
    void processKey(char key, Player& p1, Player& p2, EntityManager& entityManager,
        Screen& screen, GameStatus& status, ScreenGameManager& screenManager,
        float levelTime, long gameTick);

public:
    InputManager(MenuManager& mm);

    // Main input handling function
    void handleInput( long gameTick,
        Player& p1, Player& p2,
        EntityManager& entityManager, Screen& screen,
        GameStatus& status, ScreenGameManager& screenManager, float levelTime);
};