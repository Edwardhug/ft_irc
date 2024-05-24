#include "lib.hpp"

std::vector<std::string> split(std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

int ft_atoi(const char* str) {
    if (!str) {
        return 0;
    }
    while (std::isspace(*str)) {
        ++str;
    }
    int sign = 1;
    if (*str == '-' || *str == '+') {
        if (*str == '-') {
            sign = -1;
        }
        ++str;
    }
    int result = 0;
    while (std::isdigit(*str)) {
        result = result * 10 + (*str - '0');
        ++str;
    }

    return sign * result;
}