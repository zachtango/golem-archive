#ifndef UTIL_H
#define UTIL_H

#include <cstdint>
#include <string>


uint8_t stringToUINT8_T(const std::string &s) {
    std::string sCopy(s);

    uint8_t place = 1;
    uint8_t value = 0;
    while (!sCopy.empty()) {
        value += place * (sCopy.back() - '0');
        place *= 10;
        sCopy.pop_back();
    }

    return value;
}

#endif