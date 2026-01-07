#pragma once
#include "BaseGameObject.h"
#include "RiddleManager.h"
#include "ColorManager.h"
#include "Game.h"

class RiddleObject : public BaseGameObject {
public:
    RiddleObject(Point loc)
        : BaseGameObject(ObjectType::RIDDLE, CHAR_RIDDLE, loc,
            ColorManager::getColor(ObjectType::RIDDLE), true, true) {}

    virtual bool handleCollision(Player& player, Player& otherPlayer, Screen& screen) override {
        player.stopMoving();

        RiddleResult result = RiddleManager::handleRiddleInteraction(player, otherPlayer, screen);

      
        if (result.solved) {
            // Remove riddle after successful solution
            this->setActive(false);
            screen.clearObjectAt(location);
            return true;
        }

        return false; // Block passage until solved
    }
};