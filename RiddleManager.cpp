#include "RiddleManager.h"
#include "Player.h"
#include "Screen.h"
#include "Utils.h"
#include "Game.h"
#include <fstream>
#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <algorithm>
#include <cctype>

std::vector<Riddle> RiddleManager::riddles;

// Helper function to normalize strings (uppercase and trim)
static inline std::string normalizeString(const std::string& str) {
    std::string result;

    // Convert to uppercase and remove control characters
    for (char ch : str) {
        if (ch == '\r' || ch == '\n') {
            continue;
        }
        result.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(ch))));
    }

    // Trim leading whitespace
    size_t start = 0;
    size_t end = result.size();

    while (start < end && std::isspace(static_cast<unsigned char>(result[start]))) {
        ++start;
    }

    // Trim trailing whitespace
    while (end > start && std::isspace(static_cast<unsigned char>(result[end - 1]))) {
        --end;
    }

    return result.substr(start, end - start);
}

bool RiddleManager::loadRiddles(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    riddles.clear();
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        // Format: "Question|Answer"
        size_t delimiterPos = line.find('|');
        if (delimiterPos != std::string::npos) {
            std::string question = line.substr(0, delimiterPos);
            std::string answer = line.substr(delimiterPos + 1);

            // Normalize answer for comparison
            answer = normalizeString(answer);

            // Remove trailing carriage returns from question
            while (!question.empty() &&
                (question.back() == '\r' || question.back() == '\n')) {
                question.pop_back();
            }

            riddles.push_back({ question, answer });
        }
    }

    file.close();
    return !riddles.empty();
}

RiddleResult RiddleManager::handleRiddleInteraction(Player& player, Player& otherPlayer,
    Screen& screen) {
    RiddleResult result;
    result.playerId = player.getId();
    result.solved = false;
    result.answered = false;

    if (riddles.empty()) {
        player.setMessage("No riddles loaded!");
        return result;
    }

    // Select random riddle
    int riddleIndex = rand() % riddles.size();
    const Riddle& currentRiddle = riddles[riddleIndex];

    result.question = currentRiddle.question;
    player.setMessage("RIDDLE: " + currentRiddle.question);

    std::string userInput = "";
    player.setAnswer(userInput + "_");
    screen.updateStats(player, otherPlayer);

    // Get current game tick for replay synchronization
    long currentTick = 0;
    if (Game::getInstance()) {
        currentTick = Game::getInstance()->getGameTick();
    }

    while (true) {
        char key = _getch();

        if (key == 13) { // Enter key
            break;
        }
        else if (key == 8) { // Backspace
            if (!userInput.empty()) {
                userInput.pop_back();
            }
        }
        else if (key == 27) { // ESC key - cancel
            player.setMessage("Cancelled.");
            player.setAnswer("");
            screen.updateStats(player, otherPlayer);
            return result;
        }
        else if (key >= 32 && key <= 126) { // Printable characters
            if (userInput.length() < 50) {
                key = std::toupper(static_cast<unsigned char>(key));
                userInput += key;
            }
        }

        // Real-time visual update
        player.setAnswer("Ans: " + userInput + "_");
        if (Game::getInstance()) {
            screen.updateStats(player, otherPlayer);
        }
        
    }

    // Validate answer
    std::string normalizedInput = normalizeString(userInput);
    player.setAnswer("Ans: " + normalizedInput);

    result.answered = true;
    result.answer = normalizedInput;
    result.correct = (normalizedInput == currentRiddle.answer);

    if (result.correct) {
        player.setMessage("CORRECT! (+50 Score)");
        player.addScore(50);
        result.solved = true;
    }
    else {
        player.setMessage("WRONG! (-50 Score)");
        player.addScore(-50);
    }

    // Final screen update
    if (Game::getInstance() ) {
        screen.updateStats(player, otherPlayer);
        Utils::sleep(1000);
    }

    player.resetMessages();
    return result;
}