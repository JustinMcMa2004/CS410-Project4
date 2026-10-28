// helper function to check whether every character in a string is whitespace

#ifndef ISBLANK_H
#define ISBLANK_H

#include <algorithm>
#include <cctype>
#include <string>

inline bool isBlank(const std::string& value) {
    return value.empty() ||
           std::all_of(value.begin(), value.end(),
               [](unsigned char c) {
                   return std::isspace(c);
               });
}

#endif