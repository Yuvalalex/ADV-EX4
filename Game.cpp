#include "Game.h"
#include "Utils.h"
#include "ErrorManager.h"
#include "SaveManager.h"
#include "NormalGameMode.h"
#include "SaveGameMode.h"
#include "LoadGameMode.h"
#include <conio.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

Game* Game::instance = nullptr;

Game::Game()
    : p1(0, 0, CHAR_PLAYER_1, 1, Color::LIGHTCYAN),
    p2(0, 0, CHAR_PLAYER_2, 2, Color::LIGHTMAGENTA),
    status(GameStatus::MENU),
    gameTick(0),
    levelTime(0.0f),
    gameMode(nullptr),
    enemyMoveCounter(0) {
    instance = this;
}

Game::~Game() {
    instance = nullptr;
}

bool Game::init(int argc, char* argv[]) {
    try {
        system("mode con: cols=80 lines=25");
        Utils::hideCursor();
        Utils::setUseColor(true);

        screenGameManager.loadAllGameLevels(&p1, &p2);
        resetGame();
        createGameMode(argc, argv);

        if (!gameMode) {
            gameMode = std::make_unique<NormalGameMode>();
        }

        status = GameStatus::RUNNING;
        RiddleManager::loadRiddles(RIDDLE_FILE_NAME);

        return true;
    }
    catch (const std::exception& e) {
        ErrorManager::logError(std::string("Initialization failed: ") + e.what());
        return false;
    }
}

void Game::createGameMode(int argc, char* argv[]) {
    bool saveMode = false;
    bool loadMode = false;
    bool silentMode = false;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-save") {
            saveMode = true;
        }
        else if (arg == "-load") {
            loadMode = true;
        }
        else if (arg == "-silent") {
            silentMode = true;
        }
    }

    // Create appropriate mode
    if (loadMode) {
        gameMode = std::make_unique<LoadGameMode>(silentMode);
        status = GameStatus::RUNNING;
    }
    else if (saveMode) {
        gameMode = std::make_unique<SaveGameMode>();
    }
    else {
        gameMode = std::make_unique<NormalGameMode>();
    }
}

void Game::start() {
    // Skip menu in load mode
    if (dynamic_cast<LoadGameMode*>(gameMode.get())) {
        status = GameStatus::RUNNING;
        runGameLoop();
        status = GameStatus::EXIT;
        return;
    }

    // Interactive mode with menu loop
    handleMenu();
    while (status != GameStatus::EXIT) {
        try {
            switch (status) {
            case GameStatus::MENU:
                handleMenu();
                break;
            case GameStatus::RUNNING:
                runGameLoop();
                break;
            case GameStatus::GAME_OVER:
                handleGameOver();
                break;
            default:
                status = GameStatus::MENU;
                break;
            }
        }
        catch (const std::exception& e) {
            ErrorManager::logError(std::string("Game loop error: ") + e.what());
            status = GameStatus::MENU;
        }
    }
}

void Game::handleMenu() {
    MenuAction action = menuManager.showMainMenu();

    switch (action) {
    case MenuAction::NEW_GAME:
        resetGame();
        gameMode = std::make_unique<NormalGameMode>();
        status = GameStatus::RUNNING;
        break;

    case MenuAction::LOAD_GAME:
        if (!SaveManager::loadCompleteState(STATE_SAVE_FILE, p1, p2, levelTime, screenGameManager)) {
            ErrorManager::logError("No valid save file found.");
            Utils::sleep(1500);
        }
        else {
            status = GameStatus::RUNNING;
        }
        break;

    case MenuAction::INSTRUCTIONS:
        menuManager.showInstructions();
        break;

    case MenuAction::EXIT:
        handleExitGame();
        break;

    case MenuAction::NONE:
        handleMenu();
        break;
    }
}

void Game::runGameLoop() {
    if (gameMode->shouldRender()) {
        Utils::hideCursor();
    }

    while (status == GameStatus::RUNNING) {
        levelTime += static_cast<float>(GAME_SPEED_MS) / 1000.0f;

        Screen* currentScreenPtr = screenGameManager.getCurrentScreen();
        if (!currentScreenPtr) {
            break;
        }

        Screen& currentScreen = *currentScreenPtr;

        // Check level transitions
        if (p1.isFinished() && p2.isFinished()) {
            loadNextLevel();
            continue;
        }

        // Check previous level transition
        if (p1.getGoToPrevRoom() && p2.getGoToPrevRoom()) {
            loadPrevLevel();
            continue;
        }

        // Check game over
        if (p1.getLives() <= 0 || p2.getLives() <= 0) {
            status = GameStatus::GAME_OVER;
            return;
        }

        // Process single game tick
        processGameTick();

        // Render frame if needed
        if (gameMode->shouldRender()) {
            renderFrame(currentScreen);
        }

        // Apply delay if needed
        if (!gameMode->shouldSkipDelays()) {
            Utils::sleep(GAME_SPEED_MS);
        }

        gameTick++;
    }

    // Notify game mode of completion
    gameMode->onGameEnd(gameTick);
}

void Game::processGameTick() {
    Screen* currentScreen = screenGameManager.getCurrentScreen();
    if (!currentScreen) {
        return;
    }

    Point oldP1Pos = p1.getPosition();
    Point oldP2Pos = p2.getPosition();

    // Get input (from player or file in replay mode)
    if (!gameMode->getInput(gameTick, p1, p2, *currentScreen)) {
        status = GameStatus::GAME_OVER;
        return;
    }

    // Handle input
    inputManager.handleInput(gameTick, p1, p2, entityManager,
        *currentScreen, status, screenGameManager, levelTime);

    if (status != GameStatus::RUNNING) {
        return;
    }

    // Update physics
    physicsManager.movePlayer(p1, p2, *currentScreen, p1.getDirection());
    physicsManager.movePlayer(p2, p1, *currentScreen, p2.getDirection());

    // Handle momentum
    if (p1.hasMomentum()) {
        physicsManager.movePlayer(p1, p2, *currentScreen, p1.getMomentumDir());
    }
    if (p2.hasMomentum()) {
        physicsManager.movePlayer(p2, p1, *currentScreen, p2.getMomentumDir());
    }

    // Update player states
    p1.update(GAME_SPEED_MS);
    p2.update(GAME_SPEED_MS);

    // Record movement
    if (oldP1Pos != p1.getPosition()) {
        currentScreen->movePlayer(oldP1Pos, p1.getPosition(), p1);
        gameMode->recordEvent(gameTick, "MOVE,P1");
    }

    if (oldP2Pos != p2.getPosition()) {
        currentScreen->movePlayer(oldP2Pos, p2.getPosition(), p2);
        gameMode->recordEvent(gameTick, "MOVE,P2");
    }

    // Update bombs
    entityManager.updateBombs(*currentScreen, p1, p2, static_cast<float>(GAME_SPEED_MS));

    // Move enemies at intervals
    if (++enemyMoveCounter >= ENEMY_MOVE_INTERVAL) {
        entityManager.moveEnemies(*currentScreen, p1, p2);
        enemyMoveCounter = 0;
    }
}

void Game::renderFrame(Screen& currentScreen) {
    screenGameManager.drawCurrentState(&p1, &p2);
    int currentLevel = screenGameManager.getCurrentLevelNumber();
    currentScreen.updateStats(p1, p2, currentLevel, levelTime, status);
}

void Game::resetGame() {
    gameTick = 0;
    levelTime = 0.0f;
    enemyMoveCounter = 0;

    resetPlayer(p1);
    resetPlayer(p2);

    if (screenGameManager.getCurrentScreen() != nullptr) {
        screenGameManager.loadAllGameLevels(&p1, &p2);
    }

    entityManager.clear();
}

void Game::resetPlayer(Player& player) {
    player.setLives(INITIAL_LIVES);
    player.setScore(0);
    player.setKeys(0);
    player.set_HasSword(false);
    player.set_HasTorch(false);
    player.set_HasBomb(false);
    player.setFinished(false);
    player.setGoToPrevRoom(false);
}

void Game::loadNextLevel() {
    handleLevelTransitions();
}

void Game::loadPrevLevel() {
    // Check if we can go back
    if (screenGameManager.getCurrentLevelNumber() <= 1) {
        p1.setGoToPrevRoom(false);
        p2.setGoToPrevRoom(false);
        return;
    }

    if (gameMode->shouldRender()) {
        Utils::cls();
        Utils::printTextFile(GAME_PREV_LEV_FILE_NAME);
        Utils::sleep(1000);
    }

    screenGameManager.moveToPrevLevel();
    Screen* currentScreen = screenGameManager.getCurrentScreen();
    Point exitPosition = currentScreen->getRoomExitPos();

    if (currentScreen && currentScreen->inBoard(exitPosition)) {
        setupPlayersForLevel(currentScreen, exitPosition);
        p1.setGoToPrevRoom(false);
        p2.setGoToPrevRoom(false);
        p1.stopMoving();
        p2.stopMoving();

        if (gameMode->shouldRender()) {
            currentScreen->reprintFullBoard(p1, p2);
        }
    }
}

void Game::handleLevelTransitions() {
    if (gameMode->shouldRender()) {
        Utils::cls();
        Utils::printTextFile(GAME_NEXT_LEV_FILE_NAME);
        Utils::sleep(1000);
    }

    screenGameManager.moveToNextLevel();
    Screen* currentScreen = screenGameManager.getCurrentScreen();

    if (currentScreen) {
        setupPlayersForLevel(currentScreen);
    }

    p1.stopMoving();
    p2.stopMoving();
    p1.resetLevelState();
    p2.resetLevelState();

    if (currentScreen && gameMode->shouldRender()) {
        currentScreen->reprintFullBoard(p1, p2);
    }

    // Check if game is complete
    if (screenGameManager.isGameFinished()) {
        if (gameMode->shouldRender()) {
            Utils::cls();
            Utils::setTextColor(Color::WHITE);
            Utils::printTextFile(GAME_WIN_FILE_NAME);
            Utils::setTextColor(Color::WHITE);
            Utils::sleep(2000);
        }
        status = GameStatus::GAME_OVER;
    }
    else {
        p1.setFinished(false);
        p2.setFinished(false);
    }
}

void Game::setupPlayersForLevel(Screen* currentScreen) {
    if (!currentScreen) {
        return;
    }

    Point enterPos = currentScreen->getNextRoomEnterPos();
    p1.setPosition(enterPos);
    p2.setPosition(enterPos);
    currentScreen->setPlayers(&p1, &p2);
}

void Game::setupPlayersForLevel(Screen* currentScreen, Point position) {
    if (!currentScreen) {
        return;
    }

    p1.setPosition(position);
    p2.setPosition(position);
    currentScreen->setPlayers(&p1, &p2);
}

void Game::handleGameOver() {
    int totalScore = p1.getScore() + p2.getScore();

    if (gameMode->shouldRender()) {
        menuManager.showGameOver(totalScore);
    }

    status = GameStatus::MENU;
}

void Game::handleExitGame() {
    if (gameMode->shouldRender()) {
        menuManager.showExitGame();
    }

    status = GameStatus::EXIT;
}