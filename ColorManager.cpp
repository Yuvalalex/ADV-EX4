#include "ColorManager.h"

Color ColorManager::getColor(ObjectType type) {
    switch (type) {
    case ObjectType::WALL:      return Color::LIGHTGREY;
    case ObjectType::DOOR:      return Color::BROWN;
    case ObjectType::KEY:       return Color::LIGHTGREEN;
    case ObjectType::BOMB:      return Color::RED;
    case ObjectType::ENEMY:     return Color::LIGHTRED;
    case ObjectType::TORCH:     return Color::YELLOW;
    case ObjectType::RIDDLE:    return Color::MAGENTA;
    case ObjectType::EXIT:      return Color::LIGHTBLUE;
    case ObjectType::TELEPORT:  return Color::CYAN;
    case ObjectType::SPRING:    return Color::GREEN;
    case ObjectType::OBSTACLE:  return Color::DARKGREY;
    case ObjectType::SWITCH:    return Color::RED;
    default:                    return Color::WHITE;
    }
}

Color ColorManager::getBombColor(bool isTicking, int flickerState) {
    if (!isTicking) {
        return Color::RED;
    }
    // Flicker effect between Yellow and Red when ticking
    return (flickerState % 5 == 0) ? Color::YELLOW : Color::RED;
}

Color ColorManager::getSwitchColor(bool isOn) {
    return isOn ? Color::LIGHTGREEN : Color::RED;
}

Color ColorManager::getExitColor(bool isOn) {
    return isOn ? Color::LIGHTGREEN : Color::YELLOW;
}

Color ColorManager::getPlayerColor(int playerId) {
    if (playerId == 1) return Color::LIGHTCYAN;
    if (playerId == 2) return Color::LIGHTMAGENTA;
    return Color::WHITE;
}