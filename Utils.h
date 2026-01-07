#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include "Constants.h"

class Utils {
private:
    static bool isColorEnabled;

public:
    static void setUseColor(bool enable) { isColorEnabled = enable; }
    static void toggleColor() { isColorEnabled = !isColorEnabled; }
    static bool isColorOn() { return isColorEnabled; }
    static void setTextColor(Color color);

    static void cls();
    static void gotoxy(int x, int y);
    static void hideCursor();
    static void sleep(int milliseconds);
    static void printTextFile(const std::string& filename);
};