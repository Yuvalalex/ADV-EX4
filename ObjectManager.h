#pragma once
#include "BaseGameObject.h"
#include "Constants.h"
#include <vector>
#include <memory>

// Forward declarations
class Enemy;
class RoomBoard;
class Player;

class ObjectManager {
private:
    // Fast lookup map for objects by position
    BaseGameObject* objectMap[LEVEL_HEIGHT][LEVEL_WIDTH];

    // Ownership container for all game objects
    std::vector<std::unique_ptr<BaseGameObject>> allObjects;

public:
    ObjectManager();

    void clearAll();

    // Initialize objects from board layout
    void initFromBoard(RoomBoard& board, Player* p1, Player* p2);

    // Object management
    void addObject(std::unique_ptr<BaseGameObject> obj);
    BaseGameObject* getObjectAt(Point p) const;
    void removeObjectAt(Point p);
    void moveObject(Point from, Point to);

    // Game state queries
    bool isAllSwichesOn();



    // Get specific object types
    std::vector<Enemy*> getEnemies() const;
    std::vector<BaseGameObject*> getBombs() const;

};