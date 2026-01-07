#pragma once
#include <vector>
#include <string>

class LevelManager {
private:
    std::vector<std::string> levelFiles;
    size_t currentLevelIndex;

public:
    LevelManager() : currentLevelIndex(0) {}

    // Load all .screen files from current directory
    bool loadLevelsFromDirectory();

    // Level navigation
    std::string getCurrentLevelFile() const;
    void advanceLevel();
    bool hasMoreLevels() const;
    void reset() { currentLevelIndex = 0; }

    // Getters and setters
    size_t getCurrentLevelIndex() const { return currentLevelIndex; }
    void setLevelIndex(size_t index) { currentLevelIndex = index; }
    const std::vector<std::string>& getAllFiles() const { return levelFiles; }
    void goBackLevel();
    void setLevelIndex(int index);

};