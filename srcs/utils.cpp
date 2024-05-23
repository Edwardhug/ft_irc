#include <iostream>
#include <cctype>

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