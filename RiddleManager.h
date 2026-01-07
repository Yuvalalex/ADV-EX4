#pragma once
#include <string>
#include <vector>

// Forward declarations
class Player;
class Screen;

struct Riddle {
    std::string question;
    std::string answer;
};

struct RiddleResult {
    bool solved;
    bool answered;
    int playerId;
    std::string question;
    std::string answer;
    bool correct;

    RiddleResult()
        : solved(false), answered(false), playerId(0), correct(false) {}
};

class RiddleManager {
private:
    static std::vector<Riddle> riddles;

public:
    // Load riddles from file
    static bool loadRiddles(const std::string& filename);

    // Handle player interaction with riddle
    static RiddleResult handleRiddleInteraction(Player& player, Player& otherPlayer,
        Screen& screen);
};