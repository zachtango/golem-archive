#ifndef CRYSTALS_H
#define CRYSTALS_H

#include <cstdint>

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
    Crystals(uint8_t yellows, uint8_t greens, uint8_t blues, uint8_t pinks)
        : yellows(yellows), greens(greens), blues(blues), pinks(pinks) {}

    

private:
    uint8_t yellows; // [0, 10]
    uint8_t greens; // [0, 10]
    uint8_t blues; // [0, 10]
    uint8_t pinks; // [0, 10]

};

#endif CRYSTALS_H


