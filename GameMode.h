#pragma once
#include <string>
#include <memory>

class Player;
class Screen;

// Abstract interface for different game modes
class IGameMode {
public:
    virtual ~IGameMode() = default;

    // Get input for current tick - different implementation per mode
    // Returns true to continue game, false to end game
    virtual bool getInput(int tick, Player& p1, Player& p2, Screen& currentScreen) = 0;

    // Record/handle events after each tick
    virtual void recordEvent(int tick, const std::string& eventData) = 0;

    // Called when game ends
    virtual void onGameEnd(int finalTick) = 0;

    // Check if we should render to screen
    virtual bool shouldRender() const = 0;

    // Check if we should skip delays
    virtual bool shouldSkipDelays() const = 0;

    // Get name for debugging
    virtual std::string getModeName() const = 0;
};