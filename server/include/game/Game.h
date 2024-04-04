#ifndef GAME_H
#define GAME_H

#include "game/MerchantCardManager.h"
#include "game/PointCardManager.h"
#include "game/Crystals.h"
#include "server/User.h"
#include "server/Room.h"
#include "nlohmann/json.hpp"
#include <cstdint>
#include <deque>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <array>
#include <utility>


constexpr const uint8_t NUM_UNIQUE_MERCHANT_CARDS {44};
constexpr const uint8_t NUM_UNIQUE_POINT_CARDS {35};
constexpr const uint8_t NUM_ACTIVE_MERCHANT_CARDS {6};
constexpr const uint8_t NUM_ACTIVE_POINT_CARDS {5};

class Bot;

class Game {
public:
    friend class Bot;

    enum class MoveType {
        PlayMove = 0,
        AcquireMove = 1,
        RestMove = 2,
        ClaimMove = 3
    };

    enum class PlayMoveType {
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


    Game(RoomId id, const std::vector<UserId> &userIds, const std::vector<std::string> &userNames, const std::vector<UserId> &botIds, const std::vector<std::string> &botNames);
    ~Game();

    void move(UserId userId, const Move &move);

    void removeCrystalOverflow(UserId userId, Crystals newCrystals);

    void endGame() { isDone = true; }

    void playerChat(UserId userId, std::string message);

    std::vector<UserId> getUserIds();

    bool hasUser(UserId userId);

    nlohmann::json serialize() const;

private:
    struct ActiveMerchantCard {
        uint8_t merchantCardId;
        Crystals crystals;

        ActiveMerchantCard(uint8_t merchantCardId) : merchantCardId(merchantCardId) {}

        nlohmann::json serialize() const {
            nlohmann::json data;

            data["id"] = merchantCardId;
            data["crystals"] = crystals.serialize();

            return data;
        }
    };

    class Player; // should only be used within the Game class

    uint8_t round;

    RoomId id;

    bool isDone;

    uint8_t numPlayers; // [2, 5] players
    uint8_t maxGolems; // [5, 6] golems

    uint8_t turn; // [0, numPlayers - 1]
    bool lastRound;

    std::deque<uint8_t> pointCardIds; // point card deck
    std::deque<uint8_t> activePointCardIds; // point cards on field

    std::deque<uint8_t> merchantCardIds; // merchant card deck
    std::deque<uint8_t> activeMerchantCardIds; // merchant cards on field
    std::vector<Crystals> fieldCrystals; // field crystals on field

    uint8_t numCopperTokens; // [0, 2x numPlayers]
    uint8_t numSilverTokens; // [0, 2x numPlayers]

    std::unordered_map<UserId, Player*> players; // player id to player

    std::vector<std::pair<std::string, std::string>> chat;

    MerchantCardManager merchantCardManager;
    PointCardManager pointCardManager;

    void _move(Player *player, const Move &move);

    void _playMove(Player *player, const PlayMove &move);
    void _acquireMove(Player *player, const AcquireMove &move);
    void _restMove(Player *player);
    void _claimMove(Player *player, const ClaimMove &move);

    void _crystalPlayMove(Player *player, const CrystalPlayMove &move);
    void _upgradePlayMove(Player *player, const UpgradePlayMove &move);
    void _tradePlayMove(Player *player, const TradePlayMove &move);

};

#endif