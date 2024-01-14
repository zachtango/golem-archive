#include "game/Crystals.h"


std::ostream &operator<<(std::ostream &os, const Crystals &crystals) {
    os << static_cast<int>(crystals.yellows) << ' ' <<
            static_cast<int>(crystals.greens) << ' ' <<
            static_cast<int>(crystals.blues) << ' ' <<
            static_cast<int>(crystals.pinks);

    return os;
}
