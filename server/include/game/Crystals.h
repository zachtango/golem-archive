#ifndef CRYSTALS_H
#define CRYSTALS_H

#include "nlohmann/json.hpp"
#include <cstdint>
#include <exception>
#include <iostream>


enum class Crystal {
    yellow = 0,
    green = 1,
    blue = 2,
    pink = 3
};

struct CrystalUpgrade {
    Crystal fromCrystal;
    Crystal toCrystal;

    CrystalUpgrade(Crystal fromCrystal, Crystal toCrystal)
        : fromCrystal(fromCrystal), toCrystal(toCrystal) {}
};

class Crystals {
public:
    friend std::ostream &operator<<(std::ostream &os, const Crystals &crystals);

    Crystals() : yellows(0), greens(0), blues(0), pinks(0) {}

    Crystals(uint8_t yellows, uint8_t greens, uint8_t blues, uint8_t pinks)
        : yellows(yellows), greens(greens), blues(blues), pinks(pinks) {}

    Crystals &operator+=(const Crystals &rhs) {
        yellows += rhs.yellows;
        greens += rhs.greens;
        blues += rhs.blues;
        pinks += rhs.pinks;

        return *this;
    }

    Crystals &operator-=(const Crystals &rhs) {
        yellows -= rhs.yellows;
        greens -= rhs.greens;
        blues -= rhs.blues;
        pinks -= rhs.pinks;

        return *this;
    }

    Crystals &operator*(uint8_t n) {
        yellows *= n;
        greens *= n;
        blues *= n;
        pinks *= n;

        return *this;
    }

    uint8_t &operator[](Crystal crystal) {
        switch (crystal) {
            case Crystal::yellow:
                return yellows;
            case Crystal::green:
                return greens;
            case Crystal::blue:
                return blues;
            case Crystal::pink:
                return pinks;
        }

        return pinks;
    }

    bool operator<(const Crystals &rhs) {
        return yellows < rhs.yellows || greens < rhs.greens || blues < rhs.blues || pinks < rhs.pinks;
    }

    bool operator<(int rhs) {
        return yellows < rhs || greens < rhs || blues < rhs || pinks < rhs;
    }

    void addCrystal(Crystal crystal) {
        (*this)[crystal] += 1;
    }

    uint8_t getCrystal(Crystal crystal) {
        return (*this)[crystal];
    }

    void removeCrystal(Crystal crystal) {
        (*this)[crystal] -= 1;
    }

    operator uint8_t() const {
        return static_cast<uint8_t>(yellows + greens + blues + pinks);
    }

    nlohmann::json serialize() const {
        return {yellows, greens, blues, pinks};
    }

    uint8_t yellows; // [0, 10]
    uint8_t greens; // [0, 10]
    uint8_t blues; // [0, 10]
    uint8_t pinks; // [0, 10]

};

#endif


