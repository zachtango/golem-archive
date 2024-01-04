#ifndef PLAYER_H
#define PLAYER_H

#include "Crystals.h"
#include "Game.h"
#include <deque>
#include <unordered_set>


class Game::Player {
public:
    Player(std::string id);

    /*
        These are all public because the player is only
        accessible within the Game class, and the Game
        should have full access to all these attributes
        of a player to manage their state
    */
    std::string id;
    uint8_t turn;

    std::deque<uint8_t> pointCardIds;
    std::deque<uint8_t> merchantCardIds;
    std::unordered_set<uint8_t> usedMerchantCardIds;

    uint8_t numCopperTokens;
    uint8_t numSilverTokens;

    Crystals crystals;
};

#endif