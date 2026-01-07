#pragma once

// Game Modes and States
enum class GameMode { NEW_GAME, SAVE, LOAD };
enum class GameStatus { MENU, RUNNING, GAME_OVER, EXIT };
enum class MenuAction { NEW_GAME, LOAD_GAME, INSTRUCTIONS, EXIT, NONE };
enum class PauseAction { CONTINUE, SAVE, EXIT_TO_MENU };

// Movement and Actions
enum class Direction {
    UP, DOWN, LEFT, RIGHT, STAY,
    ESCAPE, DISPOSE, INVALID, NONE
};

// Console Colors
enum class Color {
    BLACK = 0, BLUE = 1, GREEN = 2, CYAN = 3,
    RED = 4, MAGENTA = 5, BROWN = 6, LIGHTGREY = 7,
    DARKGREY = 8, LIGHTBLUE = 9, LIGHTGREEN = 10, LIGHTCYAN = 11,
    LIGHTRED = 12, LIGHTMAGENTA = 13, YELLOW = 14, WHITE = 15
};

// Game Object Types
enum class ObjectType {
    WALL, DOOR, KEY, BOMB, ENEMY, TORCH, RIDDLE, EXIT, TELEPORT,
    SPRING, OBSTACLE, SWITCH, PLAYER, SWORD, NONE
};

// Board Dimensions and Settings
constexpr int LEVEL_WIDTH = 80;
constexpr int LEVEL_HEIGHT = 25;
constexpr int GAME_SPEED_MS = 150;
constexpr int INITIAL_LIVES = 3;
constexpr int LEGEND_HEIGHT = 3;

// Game Object Symbols
constexpr char CHAR_PLAYER_1 = '$';
constexpr char CHAR_PLAYER_2 = '&';
constexpr char CHAR_WALL = 'W';
constexpr char CHAR_KEY = 'K';
constexpr char CHAR_BOMB = '@';
constexpr char CHAR_ENEMY = '%';
constexpr char CHAR_DOOR = 'D';
constexpr char CHAR_TELEPORT = '^';
constexpr char CHAR_SPRING = '#';
constexpr char CHAR_OBSTACLE = '*';
constexpr char CHAR_SWITCH_ON = '/';
constexpr char CHAR_SWITCH_OFF = '\\';
constexpr char CHAR_TORCH = '!';
constexpr char CHAR_RIDDLE = '?';
constexpr char CHAR_SWORD = '|';
constexpr char CHAR_LEGEND = 'L';
constexpr char CHAR_FOG = 'F';
constexpr char CHAR_EXIT_NEXT = 'X';
constexpr char CHAR_EXIT_PREV = 'x';

// File Names - Level Files
constexpr const char* LEVEL_1_FILE = "adv-world_01.screen";
constexpr const char* LEVEL_2_FILE = "adv-world_02.screen";
constexpr const char* LEVEL_3_FILE = "adv-world_03.screen";
constexpr const char* LEVEL_FINAL_FILE = "adv-world-final.screen";




// File Names - Game Data
constexpr const char* STATE_SAVE_FILE = "savedGame.txt";
constexpr const char* RIDDLE_FILE_NAME = "Riddles.txt";

// File Names - UI Files
constexpr const char* MAIN_MENU_FILE_NAME = "theMainMenu.txt";
constexpr const char* GAME_USER_INSTRUCTIONS_FILE_NAME = "gameUserInstructions.txt";
constexpr const char* GAME_WIN_FILE_NAME = "FinishGame.txt";
constexpr const char* GAME_NEXT_LEV_FILE_NAME = "LevelUpDisplay.txt";
constexpr const char* GAME_PREV_LEV_FILE_NAME = "LevelDownDisplay.txt";
constexpr const char* GAME_OVER_FILE_NAME = "GameOver.txt";
constexpr const char* EXIT_GAME_FILE_NAME = "ExitGame.txt";
constexpr const char* GAME_SAVED_SUCCESSFULLY_FILE_NAME = "GAME_SAVED_SUCCESSFULLY.txt";


