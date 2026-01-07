#include "LevelManager.h"
#include "FileManager.h"
#include "ErrorManager.h"
#include <algorithm>

bool LevelManager::loadLevelsFromDirectory() {
    try {
        // Find all files with .screen 
        levelFiles = FileManager::getFileNames(".screen");

        // Sort files alphabetically to ensure correct order
        std::sort(levelFiles.begin(), levelFiles.end());

        return !levelFiles.empty();
    }
    catch (const std::exception& e) {
        ErrorManager::logError(e.what());
        return false;
    }
}

std::string LevelManager::getCurrentLevelFile() const {
    if (currentLevelIndex < levelFiles.size()) {
        return levelFiles[currentLevelIndex];
    }
    return "";
}

void LevelManager::advanceLevel() {
    currentLevelIndex++;
}

bool LevelManager::hasMoreLevels() const {
    return currentLevelIndex < levelFiles.size();
}

void LevelManager::goBackLevel() {
    if (currentLevelIndex > 0) {
        currentLevelIndex--;
    }
}

void LevelManager::setLevelIndex(int index) {
    if (index >= 0 && index < static_cast<int>(levelFiles.size())) {
        currentLevelIndex = static_cast<size_t>(index);
    }
}