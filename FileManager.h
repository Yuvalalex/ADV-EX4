#pragma once
#include <vector>
#include <string>

class FileManager {
public:
    // Get all files with specified extension in current directory
    static std::vector<std::string> getFileNames(const std::string& extension);

    // Read all lines from a file
    static std::vector<std::string> readLines(const std::string& filename);

    // Write lines to a file
    static void writeLines(const std::string& filename, const std::vector<std::string>& lines);

    // Check if file exists
    static bool fileExists(const std::string& filename);
};