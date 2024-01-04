#ifndef GAME_H
#define GAME_H

#include <cstdint>
#include <deque>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "Crystals.h"


class Game {
public:
    enum MoveType {
        PlayMove = 0,
        AcquireMove = 1,
        RestMove = 2,
        ClaimMove = 3
    };
    
    enum PlayMoveType {
        CrystalPlayMove = 0,
        UpgradePlayMove = 1,
        TradePlayMove = 2
    };

    // Move objects are created to define move player makes
    class Move;
    class PlayMove;
    class CrystalPlayMove;
    class UpgradePlayMove;
    class TradePlayMove;
    class AcquireMove;
    class RestMove;
    class ClaimMove;


    Game(const std::vector<std::string> &playerIds);

    void move(std::string_view playerId, const Move &move);

    void removeCrystalOverflow(std::string_view playerId, Crystals crystals);

private:
    class Player; // should only be used within the Game class

    uint8_t numPlayers; // [2, 5] players
    uint8_t numGolemsToWin; // [5, 6] golems

    uint8_t turn; // [0, numPlayers - 1]
    bool lastRound;

    std::deque<uint8_t> pointCardIds; // point card deck
    std::deque<uint8_t> activePointCardIds; // point cards on field

    std::deque<uint8_t> merchantCardIds; // merchant card deck
    std::deque<uint8_t> activeMerchantCardIds; // merchant cards on field

    uint8_t numCopperTokens; // [0, 2x numPlayers]
    uint8_t numSilverTokens; // [0, 2x numPlayers]

    std::unordered_map<std::string, Player> players; // player id to player
};

#endif