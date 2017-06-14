#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>

/* Proudly taken from MRezLibrary 8) */

namespace Str_utils {

void to_lower_case(std::string& str) {
    for (auto it = std::begin(str); it != std::end(str); it++) {
        if (*it >= 'A' && *it <= 'Z')
            *it += ('a' - 'A');
    }
}

void to_upper_case(std::string& str) {
    for (auto it = std::begin(str); it != std::end(str); it++) {
        if (*it >= 'a' && *it <= 'z')
            *it -= ('a' - 'A');
    }
}

void remove_spaces(std::string& str) {
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
}

void trim_left(std::string& str) {
    int trim_count = 0;
    for (int i = 0; i < str.length(); i++) {
        if (isspace(str[i])) trim_count++;
        else if (!isspace(str[i])) break;
    }
    if (trim_count > 0) str.erase(0, trim_count);
}

void trim_right(std::string& str) {
    int trim_count = 0;
    for (int i = str.length() - 1; i >= 0; i--) {
        if (isspace(str[i])) trim_count++;
        else if (!isspace(str[i])) break;
    }
    if (trim_count > 0) str.erase(str.length() - trim_count, str.length() - 1);
}

// Trim all spaces on the two ends (left and right) of the string
void trim(std::string& str) {
    trim_left(str);
    trim_right(str);
}

template <typename OP>
void split(const std::string &s, char delim, OP op) {
    std::stringstream ss(s);
    for(std::string item; std::getline(ss, item, delim);)
        *op++ = item;
}

std::vector<std::string> split(const std::string &s, char delim = ' ') {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

bool ends_with(const std::string& s, char ch) {
    return !s.empty() && s.back() == ch;
}

bool ends_with(const std::string& s, const std::string& suffix) {
    return std::mismatch(suffix.rbegin(), suffix.rend(), s.rbegin()).first == suffix.rend();
}

bool starts_with(const std::string& s, char ch) {
    return !s.empty() && s.front() == ch;
}

bool starts_with(const std::string& s, const std::string& prefix) {
    return std::mismatch(prefix.begin(), prefix.end(), s.begin()).first == prefix.end();
}



} //namespace str_utils;

#endif // STRING_UTILS_H
