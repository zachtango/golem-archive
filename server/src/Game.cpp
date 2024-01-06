#include "game/Game.h"
#include "game/Player.h"
#include "game/Move.h"
#include <cstdint>
#include <algorithm>
#include <exception>
#include <vector>


Game::Game(RoomId id, const std::unordered_set<UserId> &userIds)
    : id(id), numPlayers(static_cast<uint8_t>(userIds.size())), maxGolems(userIds.size() > 3 ? 5 : 6), turn(0),
        lastRound(false), numCopperTokens(2 * numPlayers), numSilverTokens(2 * numPlayers) {

    for (uint8_t pointCardId = 0; pointCardId < NUM_UNIQUE_POINT_CARDS; pointCardId++) {
        pointCardIds.push_back(pointCardId);
    }

    for (uint8_t i = 0; i < NUM_ACTIVE_POINT_CARDS; i++) {
        activePointCardIds.push_back(pointCardIds.front());
        pointCardIds.pop_front();
    }

    // Start at 2 because 0 and 1 are starter cards
    for (uint8_t merchantCardId = 2; merchantCardId < NUM_UNIQUE_MERCHANT_CARDS; merchantCardId++) {
        merchantCardIds.push_back(merchantCardId);
    }

    for (uint8_t i = 0; i < NUM_ACTIVE_MERCHANT_CARDS; i++){ 
        activeMerchantCards.push_back(ActiveMerchantCard(merchantCardIds.front()));
        merchantCardIds.pop_front();
    }

    uint8_t nextTurn = 0;
    for (UserId userId : userIds) {
        // FIXME: give first player and fourth and fifth player diff crystals
        players[userId] = new Player(userId, nextTurn, Crystals(4, 0, 0, 0));
        nextTurn += 1;
    }

}

Game::~Game() {
    for (auto [_, player] : players) {
        delete player;
    }
}

void Game::move(UserId userId, const Move &move) {
    if (isDone) {
        // Game ended
        return;
    }

    Player *player = players.at(userId);

    if (player->turn != turn || player->crystals > 10) {
        // Dont let players go if it's not their turn or they're in a crystal overflow state
        return;
    }

    Game::MoveType moveType = move.getMoveType();

    switch (moveType) {
        case Game::MoveType::PlayMove:
            _playMove(player, static_cast<const PlayMove&>(move));
            break;
        case Game::MoveType::AcquireMove:
            _acquireMove(player, static_cast<const AcquireMove&>(move));
            break;
        case Game::MoveType::RestMove:
            _restMove(player);
            break;
        case Game::MoveType::ClaimMove:
            _claimMove(player, static_cast<const ClaimMove&>(move));
            break;
    }

    lastRound = player->pointCardIds.size() == maxGolems;

    if (player->crystals > 10) {
        // Wait for remove crystal overflow message
        return;
    }
    
    turn = (turn + 1) % numPlayers;

    isDone = lastRound && turn == 0;
}

void Game::removeCrystalOverflow(UserId userId, Crystals newCrystals) {
    if (isDone) {
        // Game ended
        return;
    }

    if (newCrystals != 10) {
        // new crystal inventory should be 10 (max)
        return;
    }

    Player *player = players.at(userId);

    if (player->crystals <= 10) {
        // player crystal inventory should be overflowed
        return;
    }

    if (newCrystals.yellows > player->crystals.yellows ||
        newCrystals.greens > player->crystals.greens ||
        newCrystals.blues > player->crystals.blues ||
        newCrystals.pinks > player->crystals.pinks) {
        // new crystal inventory should be subset of old crystal inventory
        return;
    }

    player->crystals = newCrystals;

    turn = (turn + 1) % numPlayers;

    isDone = lastRound && turn == 0;
}

nlohmann::json Game::serialize() const {
    nlohmann::json data;

    data["id"] = id;
    data["maxGolems"] = maxGolems;
    
    data["turn"] = turn;
    data["lastRound"] = lastRound;
    data["isDone"] = isDone;

    data["activePointCardIds"] = activePointCardIds;

    std::vector<nlohmann::json> activeMerchantCards;
    for (const ActiveMerchantCard &c : this->activeMerchantCards) {
        activeMerchantCards.push_back(c.serialize());
    }
    data["activeMerchantCards"] = activeMerchantCards;

    data["numCopperTokens"] = numCopperTokens;
    data["numSilverTokens"] = numSilverTokens;

    std::vector<nlohmann::json> players;
    for (const auto &[_, player] : this->players) {
        players.push_back(player->serialize());
    }

    data["players"] = players;

    return data;
}

void Game::_playMove(Player *player, const PlayMove &move) {
    Game::PlayMoveType playMoveType = move.getPlayMoveType();

    switch (playMoveType) {
        case Game::PlayMoveType::CrystalPlayMove:
            _crystalPlayMove(player, static_cast<const CrystalPlayMove&>(move));
            break;
        case Game::PlayMoveType::UpgradePlayMove:
            _upgradePlayMove(player, static_cast<const UpgradePlayMove&>(move));
            break;
        case Game::PlayMoveType::TradePlayMove:
            _tradePlayMove(player, static_cast<const TradePlayMove&>(move));
            break;
    }
}

void Game::_acquireMove(Player *player, const AcquireMove &move) {
    uint8_t merchantCardId = move.getMerchantCardId();
    auto merchantCardIt =
        std::find_if(activeMerchantCards.begin(), activeMerchantCards.end(), 
            [&merchantCardId](const ActiveMerchantCard &c) { return c.merchantCardId == merchantCardId; });
    uint8_t merchantCardPosition = static_cast<uint8_t>(merchantCardIt - activeMerchantCards.begin());

    std::vector<Crystal> crystals = move.getCrystals();

    if (merchantCardPosition != crystals.size()) {
        // Must put exactly n crystals to get the nth card [0, 1, 2, ..., n]
        return;
    }

    for (Crystal crystal : crystals) {
        player->crystals.removeCrystal(crystal);
    }

    if (player->crystals < 0) {
        for (Crystal crystal : crystals) {
            player->crystals.addCrystal(crystal);
        }
        throw std::runtime_error("Acquire: not enough crystals");
    }

    player->crystals += activeMerchantCards[merchantCardPosition].crystals;

    activeMerchantCards.erase(merchantCardIt);

    if (!merchantCardIds.empty()) {
        activeMerchantCards.push_back(ActiveMerchantCard(merchantCardIds.front()));
        merchantCardIds.pop_front();
    }
}

void Game::_restMove(Player *player) {
    player->usedMerchantCardIds.clear();
}

void Game::_claimMove(Player *player, const ClaimMove &move) {

    uint8_t pointCardId = move.getPointCardId();
    PointCard *pointCard = pointCardManager.getPointCard(pointCardId);

    Crystals pointCardCrystals = pointCard->getCrystals();
    if (player->crystals < pointCardCrystals) {
        // Validate player has enough for point card
        return;
    }

    player->crystals -= pointCardCrystals;

    player->pointCardIds.push_back(pointCardId);

    if (activePointCardIds[0] == pointCardId) {
        if (numCopperTokens > 0) {
            numCopperTokens -= 1;
            player->numCopperTokens += 1;
        } else if (numSilverTokens > 0) {
            numSilverTokens -= 1;
            player->numSilverTokens += 1;
        }
    } else if (activePointCardIds[1] == pointCardId) {
        if (numCopperTokens > 0 && numSilverTokens > 0) {
            numSilverTokens -= 1;
            player->numSilverTokens += 1;
        }
    }
}


void Game::_crystalPlayMove(Player *player, const CrystalPlayMove &move) {
    uint8_t merchantCardId = move.getMerchantCardId();
    auto &merchantCardIds = player->merchantCardIds;
    auto &usedMerchantCardIds = player->usedMerchantCardIds;

    if (usedMerchantCardIds.count(merchantCardId) || std::find(merchantCardIds.begin(), merchantCardIds.end(), merchantCardId) == std::end(merchantCardIds)) {
        return;
    }

    // FIXME: not guaranteed the merchant card is Crystal type
    CrystalMerchantCard *merchantCard = static_cast<CrystalMerchantCard *>(merchantCardManager.getMerchantCard(merchantCardId));

    player->crystals += merchantCard->getCrystals();
    
}

void Game::_upgradePlayMove(Player *player, const UpgradePlayMove &move) {
    uint8_t merchantCardId = move.getMerchantCardId();
    std::vector<CrystalUpgrade> upgrades = move.getUpgrades();

    // FIXME: not guaranteed the merchant card is Upgrade type
    UpgradeMerchantCard *merchantCard = static_cast<UpgradeMerchantCard *>(merchantCardManager.getMerchantCard(merchantCardId));

    // Validate number of upgrades with merchant card
    if (merchantCard->getNumUpgrades() < upgrades.size()) {
        return;
    }

    // Validate upgrades
    for (const CrystalUpgrade &upgrade : upgrades) {
        if ((static_cast<uint8_t>(upgrade.toCrystal) - static_cast<uint8_t>(upgrade.fromCrystal)) != 1) {
            return;
        }
    }

    Crystals newCrystals = player->crystals;
    for (const CrystalUpgrade &upgrade : upgrades) {
        newCrystals.removeCrystal(upgrade.fromCrystal);
        newCrystals.addCrystal(upgrade.toCrystal);
    }

    // Validate player has enough crystals
    if (newCrystals < 0) {
        return;
    }

    player->crystals = newCrystals;

}

void Game::_tradePlayMove(Player *player, const TradePlayMove &move) {
    uint8_t merchantCardId = move.getMerchantCardId();
    
    uint8_t numTrades = move.getNumTrades();

    // FIXME: not guaranteed the merchant card is Trade type
    TradeMerchantCard *merchantCard = static_cast<TradeMerchantCard *>(merchantCardManager.getMerchantCard(merchantCardId));

    // With merchant card and numTrades calculate num crystals needed and validate player has enough
    Crystals fromCrystals = merchantCard->getFromCrystals() * numTrades;

    if (player->crystals < fromCrystals) {
        return;
    }

    Crystals toCrystals = merchantCard->getToCrystals() * numTrades;

    // Perform the trade
    player->crystals -= fromCrystals;
    player->crystals += toCrystals;

}

