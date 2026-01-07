#pragma once
#include "BaseGameObject.h"
#include "Player.h"
#include "Screen.h"
#include "ColorManager.h"
#include "Constants.h"

class ExitObject : public BaseGameObject {
private:
    bool isNextRoomExit; // true = next level, false = previous level

public:
    ExitObject(Point loc, bool goesToNextRoom)
        : BaseGameObject(ObjectType::EXIT,
            CHAR_EXIT_NEXT ,
            loc, ColorManager::getExitColor(CHAR_EXIT_NEXT), true, true),
        isNextRoomExit(goesToNextRoom) {}

    virtual bool handleCollision(Player& player, Player& otherPlayer, Screen& screen) override {
        if (isNextRoomExit) {
            // Check if all switches are activated before allowing exit
            bool allSwitchesOn = screen.getisAllSwichesOn();

            if (allSwitchesOn && !otherPlayer.getGoToPrevRoom()) {
                player.setFinished(true);
                player.setMessage("EXIT NEXT ROOM!");
                player.stopMoving();
                screen.updateObject(this);
                screen.setRoomExitPos(player.getPosition());
                return true;
            }
            else {
                player.resetMessages();
                player.setMessage("NOT ALL SWITCHES ON!");
                player.stopMoving();
                return false;
            }
        }
        else {
            // Previous room exit
            bool canGoBack = screen.legal_levState_to_go_prev();

            if (canGoBack && !otherPlayer.isFinished()) {
                player.setGoToPrevRoom(true);
                player.setMessage("EXIT PREV ROOM!");
                return true;
            }
            else {
                player.resetMessages();
                player.setMessage("CAN'T EXIT PREV ROOM!");
                player.stopMoving();
                return false;
            }
        }
    }
};
