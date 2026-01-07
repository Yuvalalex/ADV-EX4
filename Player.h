#pragma once
#include "Point.h"
#include "Constants.h"
#include "BaseGameObject.h"
#include <string>
#include <vector>

// Forward declarations
class Screen;
class SpringObject;

class Player {
private:
    // Position data
    Point location;
    Point startPosition;
    Point initialPos;

    // Visual representation
    char symbol;
    Color color;
    int id;

    // Player stats
    int lives;
    int score;
    int keys;
    bool finished;
    bool goToPrevRoom;

    // Movement and interaction
    Direction currentDir;
    std::string currentMessage;
    std::string currentAnswer;

    // Spring compression mechanics
    bool isCompressingSpring;
    int compressedSpringsCount;
    Direction compressionDir;
    std::vector<SpringObject*> activeSprings;

    // Momentum/physics
    Direction momentumDir;
    int velocity;
    int velocityDuration;

    // Inventory
    bool _hasTorch;
    bool _hasBomb;
    bool _hasSword;

public:
    // Constructors
    Player();
    Player(int x, int y, char c, int playerId, Color col);

    // Core update and rendering
    void update(float deltaTime);
    void draw() const;

    // Reset functions
    void reset();              // Full reset for new game
    void resetLevelState();    // Partial reset for next level

    // Position management
    Point getPosition() const { return location; }
    void setPosition(Point p) { location = p; }
    void setStartPosition(Point p) { startPosition = p; location = p; }
    void setInitialPosition(Point p) { initialPos = p; }
    void resetToInitialPosition();

    // Movement
    Direction getDirection() const { return currentDir; }
    void setDirection(Direction d) { currentDir = d; }
    void stopMoving() { currentDir = Direction::STAY; }

    // Life and score
    int getLives() const { return lives; }
    void decreaseLife() { lives--; }
    void setLives(int l) { lives = l; }
    int getScore() const { return score; }
    void addScore(int s) { score += s; }
    void setScore(int s) { score = s; }

    // Keys
    int getKeys() const { return keys; }
    void addKey() { keys++; }
    bool useKey();
    void setKeys(int k) { keys = k; }

    // Level progression
    bool isFinished() const { return finished; }
    void setFinished(bool f) { finished = f; }
    bool getGoToPrevRoom() const { return goToPrevRoom; }
    void setGoToPrevRoom(bool f) { goToPrevRoom = f; }

    // Visual properties
    Color getColor() const { return color; }
    char getRepr() const { return symbol; }
    int getId() const { return id; }

    // Messages and UI
    void setMessage(const std::string& msg) { currentMessage = msg; }
    std::string getMessage() const { return currentMessage; }
    void setAnswer(const std::string& ans) { currentAnswer = ans; }
    std::string getAnswer() const { return currentAnswer; }
    void resetMessages() { currentMessage = ""; currentAnswer = ""; }

    // Inventory management
    void pickBomb() { _hasBomb = true; }
    void pickTorch() { _hasTorch = true; }
    void pickSword() { _hasSword = true; }
    bool get_HasBomb() const { return _hasBomb; }
    bool get_HasTorch() const { return _hasTorch; }
    bool get_HasSword() const { return _hasSword; }
    void set_HasBomb(bool b) { _hasBomb = b; }
    void set_HasTorch(bool b) { _hasTorch = b; }
    void set_HasSword(bool b) { _hasSword = b; }

    // Item disposal
    ObjectType handleDispose(Screen& screen, Player& otherPlayer);

    // Spring mechanics
    void addSpring(SpringObject* spring) { activeSprings.push_back(spring); }
    void releaseSpring(Screen& screen);
    void startCompressing(Direction dir);
    void stopCompressing();
    void incrementCompression() { compressedSpringsCount++; }
    bool getIsCompressing() const { return isCompressingSpring; }
    int getCompressedSpringsCount() const { return compressedSpringsCount; }

    // Momentum/physics
    void setMomentum(int vel, int dur, Direction dir);
    void applyMomentum(int vel, int dur, Direction dir);
    void stopMomentum();
    void decreaseMomentumDuration();
    bool hasMomentum() const { return velocityDuration > 0; }
    int getMomentumVelocity() const { return velocity; }
    Direction getMomentumDir() const { return momentumDir; }
    int getVelocityDuration() const { return velocityDuration; }
    int getForce() const { return (velocityDuration > 0) ? velocity : 1; }

    // Energy transfer between players
    void transferEnergyTo(Player& target);
};