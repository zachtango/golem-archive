#ifndef PLAYER_H
#define PLAYER_H

#include "game/Crystals.h"
#include "game/Game.h"
#include "server/User.h"
#include <deque>
#include <unordered_set>


class Game::Player {
public:
    Player(UserId id, uint8_t turn, Crystals crystals)
        : id(id), turn(turn), merchantCardIds({0, 1}),
            numCopperTokens(0), numSilverTokens(0), crystals(crystals) {}

    /*
        These are all public because the player is only
        accessible within the Game class, and the Game
        should have full access to all these attributes
        of a player to manage their state
    */
    UserId id;
    uint8_t turn;

    std::deque<uint8_t> pointCardIds;
    std::deque<uint8_t> merchantCardIds;
    std::unordered_set<uint8_t> usedMerchantCardIds;

    uint8_t numCopperTokens;
    uint8_t numSilverTokens;

    Crystals crystals;
};

#endif