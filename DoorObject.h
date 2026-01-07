#pragma once
#include "BaseGameObject.h"
#include "Player.h"
#include "Screen.h"

class DoorObject : public BaseGameObject {
private:
    int requiredKeys;

public:
    DoorObject(Point loc, int keys = 1)
        : BaseGameObject(ObjectType::DOOR, CHAR_DOOR, loc, Color::BROWN, true, false),
        requiredKeys(keys) {}

    virtual bool handleCollision(Player& player, Player& otherPlayer, Screen& screen) override {
        if (player.getKeys() >= requiredKeys) {
            // Use required keys to open door
            for (int i = 0; i < requiredKeys; ++i) {
                player.useKey();
            }

            player.setMessage("Door Opened!");
            this->setActive(false);
            screen.clearObjectAt(location);

            return true; // Allow passage
        }

        // Not enough keys
        player.setMessage("Need " + std::to_string(requiredKeys) + " Keys!");
        return false; // Block passage
    }
};