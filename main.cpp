#include "Game.h"
#include "ErrorManager.h"
#include "FileManager.h"
#include "Utils.h"
#include <iostream>
#include <memory>
#include <filesystem>

int main(int argc, char* argv[]) {
    // Initial console setup
    Utils::hideCursor();

    try {
        auto game = std::make_unique<Game>();

        if (game->init(argc, argv)) {
            game->start();
        }
        else {
            return 1; // Initialization failed
        }
    }
    catch (const GameException& e) {
        // Specific game logic error
        ErrorManager::logError(e.what());
        return 1;
    }
    catch (const std::exception& e) {
        // Generic system error
        ErrorManager::logError("System Error: " + std::string(e.what()));
        return 1;
    }
    catch (...) {
        // Unknown error
        ErrorManager::logError("An unknown fatal error occurred.");
        return 1;
    }

    return 0; // Success
}