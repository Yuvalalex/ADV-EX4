================================================================================
                            ADVENTURE WORLD
                        CPP- 2-Player Console Game
================================================================================

Student: YUVAL ALEXANDRONY
ID: 322218033
Course: C++ 2026A

================================================================================
HOW TO RUN
================================================================================

Visual Studio:
1. Open MyGame.sln
2. Build -> Rebuild Solution
3. Debug -> Start Without Debugging

Command Line (G++):
g++ -std=c++17 *.cpp -o game.exe


================================================================================
FILES NEEDED
================================================================================

Level files:
- adv-world_01.screen
- adv-world_02.screen  
- adv-world_03.screen

- Riddles.txt

UI files:
- theMainMenu.txt
- gameUserInstructions.txt
- FinishGame.txt
- GameOver.txt
- ExitGame.txt
- LevelUpDisplay.txt
- LevelDownDisplay.txt
- GAME_SAVED_SUCCESSFULLY.txt


================================================================================
CONTROLS
================================================================================

Player 1 ($):          Player 2 (&):
W - Up                 I - Up
X - Down               M - Down
A - Left               J - Left
D - Right              L - Right
S - Stop               K - Stop
E - Drop item          O - Drop item

Player 0 - SYSTEM CONTROLS:
ESC - Pause menu (options: Continue, Save, Exit to Menu)
Note: All input is case-insensitive

================================================================================
GAME OBJECTS
================================================================================

W = Wall (blocked)
K = Key (collect to open doors)
1-9 = Door (needs that many keys & Marke as 'D')
@ = Bomb (explodes after 2.5 sec)
% = Enemy (avoid or kill with Bomb/sword)
! = Torch (lights up dark rooms)
| = Sword (kills enemies)
? = Riddle (answer to pass)
^ = Teleport (random destination)
# = Spring (launches player)
* = Obstacle (can push)
/ = Switch ON
\ = Switch OFF
X = Exit to next level

MORE:
*   Riddle: Passable only after correct answer
**  Obstacle: Can be pushed with sufficient force
*** Exit:
        Requires all switches ON (for next level)
        x = Exit to previous level (Look like Regular Exit)

L = Legend Marker  = Stats display position    
F = Fog Marker     = Indicates dark room           

================================================================================
HOW THINGS WORK
================================================================================

Movement:
- Pick a direction, player keeps moving
- Press stop key to stop
- Speed is 1 cell per cycle

Springs:
- Walk into spring to compress it
- Release launches you
- velocity = number of springs compressed
- duration = springs^2

Enemies - (in Bonus.txt)

Dark Rooms:
- Marked with F in level file
- Need torch to see
- Walls always visible

Scoring:
- Correct riddle: +50
- Wrong riddle: -50
- Start with 3 lives each


LIGHTING SYSTEM:
- Dark rooms marked with 'F' character in level file
- Walls & Players & Torch always visible (for navigation)
- Torch illuminates entire room when held by either player
- All other objects hidden until torch acquired


FILE FORMATS:
- Level files: Plain text with single character per cell
- Save file: Structured text (level, time, player states, board matrices)
- Riddles file: Question|Answer format (one per line)


================================================================================
DESIGN CHOICES
================================================================================

- Used manager classes to organize code (ScreenManager ,InputManager, PhysicsManager...)
- Used smart pointers (unique_ptr, shared_ptr) for memory
- Each game object has its own class
- Colors can be turned on/off in menu


================================================================================
                          TESTING INFORMATION
================================================================================

MANUAL TESTING CHECKLIST:
--------------------------
- Menu navigation
- New game start
- Player movement (both players)
- Key collection and door opening
- Bomb placement and explosion
- Spring compression and launch
- Enemy 
- Riddle interaction
- Switch toggling
- Obstacle pushing
- Teleport usage
- Dark room with torch
- Save game (ESC → S)
- Load last Saved game (Menu -> 2)
- Level progression
- Game over conditions

================================================================================
CODE SOURCES
================================================================================

EXTERNAL CODE SOURCES:
----------------------
Console utility functions (gotoxy, SetConsoleTextAttribute, _kbhit, _getch)
are based on:
- Course lab materials
- Windows documentation
- Keren Kalif's Game  


AI tools used:
- Used for help with debugging & cleaning
- Also used for code review and fixing bugs
- All code was checked and understood by me
- I made the design decisions myself

================================================================================
KNOWN ISSUES
================================================================================

- Windows only (uses Windows.h)
- Console must be 80x25
- Two players share one keyboard


================================================================================

ID: 322218033

================================================================================
