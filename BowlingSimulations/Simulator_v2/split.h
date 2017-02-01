#include <string>
#include <vector>

#pragma once

std::vector<std::string> split(const std::string &str, char delim) {
    std::vector<std::string> tokens;

    unsigned int loc = 0;
    
    while (str.find(delim, loc) != std::string::npos) {
        std::size_t delimLoc = str.find(delim, loc);
        std::string token = str.substr(loc, delimLoc);
        tokens.push_back(token);
        loc = delimLoc + 1;
    }

    if (loc < str.size()) {
        tokens.push_back(str.substr(loc, str.size()));
    }

    return tokens;
}
