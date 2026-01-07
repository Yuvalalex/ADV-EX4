#pragma once
#include "ScreenGameManager.h"
#include "Player.h"
#include "PhysicsManager.h"
#include "InputManager.h"
#include "MenuManager.h"
#include "EntityManager.h"
#include "RiddleManager.h"
#include "Constants.h"
#include "GameMode.h"
#include <memory>

/// @brief Main game controller - orchestrates all game systems
class Game {
private:
    static Game* instance;

    // Core managers
    ScreenGameManager screenGameManager;
    MenuManager menuManager;
    PhysicsManager physicsManager;
    EntityManager entityManager;
    InputManager inputManager;

    // Players
    Player p1;
    Player p2;

    // Game state
    GameStatus status;
    long gameTick;
    float levelTime;

    // Game mode - polymorphic interface
    std::unique_ptr<IGameMode> gameMode;
    
    // Enemy movement timing
    int enemyMoveCounter;
    static constexpr int ENEMY_MOVE_INTERVAL = 2;

public:
    Game();
    ~Game();

    /// @brief Get singleton instance
    static Game* getInstance() { return instance; }

    /// @brief Initialize game with command line arguments
    bool init(int argc, char* argv[]);

    /// @brief Start main game loop
    void start();

    /// @brief Get current game tick
    long getGameTick() const { return gameTick; }

    /// @brief Get current game mode
    IGameMode* getGameMode() const { return gameMode.get(); }

private:
    // State machine
    void handleMenu();
    void handleGameOver();
    void handleExitGame();

    // Game loop phases
    void runGameLoop();
    void processGameTick();
    void renderFrame(Screen& currentScreen);

    // Game initialization
    void resetGame();
    void resetPlayer(Player& player);
    void createGameMode(int argc, char* argv[]);

    // Level management
    void loadNextLevel();
    void loadPrevLevel();
    void handleLevelTransitions();
    void setupPlayersForLevel(Screen* currentScreen);
    void setupPlayersForLevel(Screen* currentScreen, Point position);
};