#include "common.h"
#include <filesystem>

std::string path;
std::string get_Project_Path()
{
    std::filesystem::path current_path = std::filesystem::current_path();
    return current_path.string();
}

// Path: Core\Render\renderwindow.cpp
// Compare this snippet from main.cpp: