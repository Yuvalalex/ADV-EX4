#include "InputManager.h"
#include "Move.h"
#include "SaveManager.h"
#include "Utils.h"
#include <conio.h>
#include <iostream>

InputManager::InputManager(MenuManager& mm) : menuManager(mm) {}

void InputManager::processKey(char key, Player& p1, Player& p2, EntityManager& entityManager,
    Screen& screen, GameStatus& status, ScreenGameManager& screenManager,
    float levelTime, long gameTick) {

    int playerId = Move::getPlayerIdFromKey(key);
    Direction dir = Move::getDirectionFromKey(key);

    // System keys (ESC for pause menu)
    if (playerId == 0) {
        if (dir == Direction::ESCAPE) {
            char actionCode = 0;


            PauseAction action = menuManager.showPauseMenu();

            if (action == PauseAction::SAVE) {
                if (SaveManager::saveCompleteState(STATE_SAVE_FILE, p1, p2,
                    levelTime, screenManager)) {
                    menuManager.showGameSavedMasg();
                    Utils::sleep(1000);
                    screen.reprintFullBoard(p1, p2);
                }
                actionCode = 'S';
            }
            else if (action == PauseAction::EXIT_TO_MENU) {
                status = GameStatus::MENU;
                actionCode = 'X';
            }
            else if (action == PauseAction::CONTINUE) {
                screen.reprintFullBoard(p1, p2);
            }
            
      
        }
    }
    // Player 1 input
    else if (playerId == 1 && !p1.isFinished()) {
        p1.resetMessages();
        if (dir != Direction::DISPOSE && dir != Direction::NONE) {
            p1.setDirection(dir);
        }
    }
    // Player 2 input
    else if (playerId == 2 && !p2.isFinished()) {
        p2.resetMessages();
        if (dir != Direction::DISPOSE && dir != Direction::NONE) {
            p2.setDirection(dir);
        }
    }

    // Handle item disposal/shooting
    if (dir == Direction::DISPOSE) {
        if (playerId == 1 && !p1.isFinished()) {
            p1.handleDispose(screen, p2);
        }
        if (playerId == 2 && !p2.isFinished()) {
            p2.handleDispose(screen, p1);
        }
    }
}

void InputManager::handleInput(  long gameTick,
    Player& p1, Player& p2,
    EntityManager& entityManager, Screen& screen,
    GameStatus& status, ScreenGameManager& screenManager,
    float levelTime) {
   
    while (_kbhit()) {
        char key = _getch();
        processKey(key, p1, p2, entityManager, screen, status,
            screenManager, levelTime, gameTick);
    }
    
}