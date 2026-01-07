#include "SaveManager.h"
#include "FileManager.h"
#include "ErrorManager.h"
#include "Screen.h"
#include <fstream>
#include <sstream>

using namespace std;

// Helper function to format player data for saving
string formatPlayerData(const Player& player) {
    stringstream ss;
    ss << " " << player.getKeys()
        << " " << (player.get_HasSword() ? 1 : 0)
        << " " << (player.get_HasTorch() ? 1 : 0)
        << " " << (player.get_HasBomb() ? 1 : 0);
    return ss.str();
}

bool SaveManager::saveCompleteState(const string& filename,
    Player& p1, Player& p2,
    float levelTime,
    ScreenGameManager& screenManager) {
    try {
        ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        // Save global game state
        file << screenManager.getCurrentLevelNumber() << " " << levelTime << endl;

        // Save player 1 state
        file << p1.getPosition().getX() << " " << p1.getPosition().getY()
            << " " << p1.getLives() << " " << p1.getScore()
            << formatPlayerData(p1) << endl;

        // Save player 2 state
        file << p2.getPosition().getX() << " " << p2.getPosition().getY()
            << " " << p2.getLives() << " " << p2.getScore()
            << formatPlayerData(p2) << endl;

        // Save all screen states
        const auto& screens = screenManager.getAllScreens();
        file << screens.size() << endl;

        for (const auto& screen : screens) {
            // Save level identifier
            file << screen->getLevel() << endl;

            // Save board matrix
            for (int y = 0; y < LEVEL_HEIGHT; ++y) {
                for (int x = 0; x < LEVEL_WIDTH; ++x) {
                    Point p(x, y);
                    char c = screen->getObjectAt(p);

                    // Don't save player characters as part of static board
                    if (c == CHAR_PLAYER_1 || c == CHAR_PLAYER_2) {
                        c = ' ';
                    }

                    file << c;
                }
                file << endl;
            }
        }

        return true;
    }
    catch (...) {
        return false;
    }
}

bool SaveManager::loadCompleteState(const string& filename,
    Player& p1, Player& p2,
    float& levelTime,
    ScreenGameManager& screenManager) {
    if (!FileManager::fileExists(filename)) {
        return false;
    }

    try {
        ifstream file(filename);
        int targetLevelIdx = 1;

        // Load global state
        file >> targetLevelIdx >> levelTime;

        int x, y, lives, score, keys, hasSword, hasTorch, hasBomb;

        // Load original levels first (reset)
        screenManager.loadAllGameLevels(&p1, &p2);

        // Advance to target level
        while (screenManager.getCurrentLevelNumber() < targetLevelIdx) {
            screenManager.moveToNextLevel();
            if (screenManager.isGameFinished()) {
                break;
            }
        }

        // Load player 1 state
        file >> x >> y >> lives >> score >> keys >> hasSword >> hasTorch >> hasBomb;
        p1.setPosition(Point(x, y));
        p1.setLives(lives);
        p1.setScore(score);
        p1.setKeys(keys);
        p1.set_HasBomb(hasBomb);
        p1.set_HasSword(hasSword);
        p1.set_HasTorch(hasTorch);

        // Load player 2 state
        file >> x >> y >> lives >> score >> keys >> hasSword >> hasTorch >> hasBomb;
        p2.setPosition(Point(x, y));
        p2.setLives(lives);
        p2.setScore(score);
        p2.setKeys(keys);
        p2.set_HasBomb(hasBomb);
        p2.set_HasSword(hasSword);
        p2.set_HasTorch(hasTorch);

        // Load saved board states
        int numScreens = 0;
        file >> numScreens;

        string dummyLine;
        getline(file, dummyLine); // Clear buffer

        const auto& screens = screenManager.getAllScreens();

        for (int i = 0; i < numScreens; ++i) {
            int savedLevelNum;
            file >> savedLevelNum;
            getline(file, dummyLine); // Clear buffer

            // Find matching screen
            Screen* targetScreen = nullptr;
            for (auto& screen : screens) {
                if (screen->getLevel() == savedLevelNum) {
                    targetScreen = screen.get();
                    break;
                }
            }

            // Read board matrix from file
            for (int row = 0; row < LEVEL_HEIGHT; ++row) {
                string line;
                getline(file, line);

                if (targetScreen) {
                    // Pad line if shorter than expected
                    if (line.size() < LEVEL_WIDTH) {
                        line.resize(LEVEL_WIDTH, ' ');
                    }

                    // Update board characters
                    for (int col = 0; col < LEVEL_WIDTH; ++col) {
                        Point p(col, row);
                        targetScreen->getBoard().setCharAt(p, line[col]);
                    }
                }
            }

            // Reinitialize objects from updated board
            if (targetScreen) {
                targetScreen->getObjectManager().initFromBoard(
                    targetScreen->getBoard(), nullptr, nullptr);
            }
        }

        return true;
    }
    catch (const exception& e) {
        ErrorManager::logError(e.what());
        return false;
    }
}