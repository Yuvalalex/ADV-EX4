#include "FileManager.h"
#include "ErrorManager.h"
#include <filesystem>
#include <fstream>
#include <algorithm>

namespace fs = std::filesystem;

std::vector<std::string> FileManager::getFileNames(const std::string& extension) {
    std::vector<std::string> files;

    try {
        if (!fs::exists(".")) {
            return files;
        }

        for (const auto& entry : fs::directory_iterator(".")) {
            if (entry.is_regular_file() && entry.path().extension() == extension) {
                files.push_back(entry.path().filename().string());
            }
        }
    }
    catch (...) {
        // Return empty vector on error
    }

    std::sort(files.begin(), files.end());
    return files;
}

std::vector<std::string> FileManager::readLines(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw GameException("Cannot open file: " + filename);
    }

    std::vector<std::string> lines;
    std::string line;
    bool isFirstLine = true;

    while (std::getline(file, line)) {
        // Remove carriage return if present
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        // Handle BOM (Byte Order Mark) on first line if present
        if (isFirstLine && line.size() >= 3 &&
            static_cast<unsigned char>(line[0]) == 0xEF) {
            line = line.substr(3);
        }

        isFirstLine = false;
        lines.push_back(line);
    }

    return lines;
}

void FileManager::writeLines(const std::string& filename,
    const std::vector<std::string>& lines) {
    std::ofstream file(filename);
    if (!file) {
        throw GameException("Cannot write to: " + filename);
    }

    for (const auto& line : lines) {
        file << line << std::endl;
    }
}

bool FileManager::fileExists(const std::string& filename) {
    return fs::exists(filename) && fs::is_regular_file(filename);
}