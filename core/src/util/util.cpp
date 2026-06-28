#include "util.h"


std::string trimSpaces(std::string str) {
    auto left = str.find_first_not_of(" \t");
    if (left == std::string::npos) {
        return "";
    }
    auto right = str.find_last_not_of(" ");
    if (right == std::string::npos) {
        return "";
    }
    return str.substr(left, right - left + 1);
}