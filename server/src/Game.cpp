#include "game/Game.h"
#include "game/Player.h"
#include "game/Move.h"
#include <cstdint>
#include <algorithm>
#include <exception>
#include <vector>
#include <iostream>
#include <random>
#include <string>


Game::Game(RoomId id, const std::vector<UserId> &userIds, const std::vector<std::string> &userNames)
    : round(1), id(id), isDone(false), numPlayers(static_cast<uint8_t>(userIds.size())),
        maxGolems(userIds.size() > 3 ? 5 : 6), turn(0),
        lastRound(false), numCopperTokens(2 * numPlayers), numSilverTokens(2 * numPlayers) {

    for (uint8_t pointCardId = 0; pointCardId < NUM_UNIQUE_POINT_CARDS; pointCardId++) {
        pointCardIds.push_back(pointCardId);
    }

    // Start at 2 because 0 and 1 are starter cards
    for (uint8_t merchantCardId = 2; merchantCardId < NUM_UNIQUE_MERCHANT_CARDS; merchantCardId++) {
        merchantCardIds.push_back(merchantCardId);
    }

    std::random_device rd;
    auto rng = std::default_random_engine {rd()};
    std::shuffle(std::begin(pointCardIds), std::end(pointCardIds), rng);
    std::shuffle(std::begin(merchantCardIds), std::end(merchantCardIds), rng);

    for (uint8_t i = 0; i < NUM_ACTIVE_POINT_CARDS; i++) {
        activePointCardIds.push_back(pointCardIds.front());
        pointCardIds.pop_front();
    }

    for (uint8_t i = 0; i < NUM_ACTIVE_MERCHANT_CARDS; i++){ 
        activeMerchantCardIds.push_back(merchantCardIds.front());
        merchantCardIds.pop_front();
    }

    uint8_t nextTurn = 0;

    players[userIds[0]] = new Player(userIds[0], userNames[0], nextTurn, Crystals(3, 0, 0, 0));
    nextTurn += 1;

    for (int i = 1; i < userIds.size(); i += 1, nextTurn += 1) {
        // FIXME: give first player and fourth and fifth player diff crystals
        players[userIds[i]] = new Player(userIds[i], userNames[i], nextTurn, Crystals(4, 0, 0, 0));
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
            std::cout << "Game: play move\n";
            _playMove(player, static_cast<const PlayMove&>(move));
            chat.push_back({"use-merchant-card", players.at(userId)->userName + " played a merchant card"});
            break;
        case Game::MoveType::AcquireMove:
            std::cout << "Game: acquire move\n";
            _acquireMove(player, static_cast<const AcquireMove&>(move));
            chat.push_back({"acquire-merchant-card", players.at(userId)->userName + " picked up a merchant card"});
            break;
        case Game::MoveType::RestMove:
            std::cout << "Game: rest move\n";
            _restMove(player);
            chat.push_back({"rest", players.at(userId)->userName + " rested"});
            break;
        case Game::MoveType::ClaimMove:
            std::cout << "Game: claim move\n";
            _claimMove(player, static_cast<const ClaimMove&>(move));
            chat.push_back({"purchase-point-card", players.at(userId)->userName + " claimed a golem"});
            break;
    }

    if (player->pointCardIds.size() == maxGolems) {
        lastRound = true;
    }

    if (player->crystals > 10) {
        // Wait for remove crystal overflow message
        return;
    }
    
    turn = static_cast<uint8_t>((turn + 1) % numPlayers);

    isDone = lastRound && turn == 0;

    if (turn == 0) {
        round += 1;
    }
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

    turn = static_cast<uint8_t>((turn + 1) % numPlayers);

    isDone = lastRound && turn == 0;

    chat.push_back({"remove-crystals", players.at(userId)->userName + " gave away crystals"});
}

void Game::playerChat(UserId userId, std::string message) {
    chat.push_back({"message", players.at(userId)->userName + ": " + message});
}

nlohmann::json Game::serialize() const {
    nlohmann::json data;

    data["round"] = round;
    data["id"] = id;
    data["maxGolems"] = maxGolems;
    
    data["turn"] = turn;
    data["lastRound"] = lastRound;
    data["isDone"] = isDone;

    std::vector<uint8_t> reversedActivePointCardIds;
    for (auto it = activePointCardIds.rbegin(); it != activePointCardIds.rend(); ++it) {
        reversedActivePointCardIds.push_back(*it);
    }

    data["activePointCardIds"] = reversedActivePointCardIds;

    std::vector<uint8_t> reversedMerchantCardIds;
    for (auto it = activeMerchantCardIds.rbegin(); it != activeMerchantCardIds.rend(); ++it) {
        reversedMerchantCardIds.push_back(*it);
    }

    data["activeMerchantCardIds"] = reversedMerchantCardIds;

    std::vector<nlohmann::json> fieldCrystals;
    for (const auto &crystals : this->fieldCrystals) {
        fieldCrystals.push_back(crystals.serialize());
    }
    data["fieldCrystals"] = fieldCrystals;

    data["numCopperTokens"] = numCopperTokens;
    data["numSilverTokens"] = numSilverTokens;

    std::vector<nlohmann::json> players;
    for (const auto &[_, player] : this->players) {
        players.push_back(player->serialize());
    }

    data["players"] = players;
    
    data["numPointCards"] = pointCardIds.size();
    data["numMerchantCards"] = merchantCardIds.size();

    data["chat"] = chat;

    return data;
}

void Game::_playMove(Player *player, const PlayMove &move) {
    Game::PlayMoveType playMoveType = move.getPlayMoveType();
    uint8_t merchantCardId = move.getMerchantCardId();

    auto &merchantCardIds = player->merchantCardIds;
    auto &usedMerchantCardIds = player->usedMerchantCardIds;

    if (usedMerchantCardIds.count(merchantCardId) || std::find(merchantCardIds.begin(), merchantCardIds.end(), merchantCardId) == std::end(merchantCardIds)) {
        std::cout << "Game: " << static_cast<int>(merchantCardId) << " not found\n";
        return;
    }

    switch (playMoveType) {
        case Game::PlayMoveType::CrystalPlayMove:
            std::cout << "Game: crystal play move\n";
            _crystalPlayMove(player, static_cast<const CrystalPlayMove&>(move));
            break;
        case Game::PlayMoveType::UpgradePlayMove:
            std::cout << "Game: upgrade play move\n";
            _upgradePlayMove(player, static_cast<const UpgradePlayMove&>(move));
            break;
        case Game::PlayMoveType::TradePlayMove:
            std::cout << "Game: trade play move\n";
            _tradePlayMove(player, static_cast<const TradePlayMove&>(move));
            break;
    }

    usedMerchantCardIds.insert(merchantCardId);
}

void Game::_acquireMove(Player *player, const AcquireMove &move) {
    uint8_t merchantCardId = move.getMerchantCardId();
    auto merchantCardIt = std::find(activeMerchantCardIds.begin(), activeMerchantCardIds.end(), merchantCardId);
    uint8_t merchantCardPosition = static_cast<uint8_t>(merchantCardIt - activeMerchantCardIds.begin());

    std::vector<Crystal> crystals = move.getCrystals();

    if (merchantCardPosition != crystals.size()) {
        std::cout << "Crystal position mismatch\n";
        // Must put exactly n crystals to get the nth card [0, 1, 2, ..., n]
        return;
    }

    Crystals newCrystals = player->crystals;
    for (Crystal crystal : crystals) {
        std::cout << static_cast<int>(crystal) << '\n';
        if (newCrystals.getCrystal(crystal) == 0) {
            std::cout << "Acquire: not enough crystals\n";
            return;
        }
        newCrystals.removeCrystal(crystal);
    }

    if (merchantCardPosition < fieldCrystals.size()) {
        newCrystals += fieldCrystals[merchantCardPosition];
        fieldCrystals.erase(fieldCrystals.begin() + merchantCardPosition);
    }

    std::cout << "Crystals after drop: " << newCrystals << '\n';

    activeMerchantCardIds.erase(merchantCardIt);

    if (!merchantCardIds.empty()) {
        activeMerchantCardIds.push_back(merchantCardIds.front());
        merchantCardIds.pop_front();
    }

    // Drop crystals on merchant cards
    for (uint8_t i = 0; i < crystals.size(); i++) {
        fieldCrystals[i].addCrystal(crystals[i]);
    }

    player->merchantCardIds.push_back(merchantCardId);
    player->crystals = newCrystals;

    std::cout << "New crystals: " << newCrystals << '\n';
}

void Game::_restMove(Player *player) {
    player->usedMerchantCardIds.clear();
}

void Game::_claimMove(Player *player, const ClaimMove &move) {
    uint8_t pointCardId = move.getPointCardId();

    auto pointCardIt = std::find(activePointCardIds.begin(), activePointCardIds.end(), pointCardId);
    if (pointCardIt == std::end(activePointCardIds)) {
        // Point card not in active point cards
        return;
    }

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

    activePointCardIds.erase(pointCardIt);
    if (!pointCardIds.empty()) {
        activePointCardIds.push_back(pointCardIds.front());
        pointCardIds.pop_front();
    }

}


void Game::_crystalPlayMove(Player *player, const CrystalPlayMove &move) {
    uint8_t merchantCardId = move.getMerchantCardId();

    // FIXME: not guaranteed the merchant card is Crystal type
    CrystalMerchantCard *merchantCard = static_cast<CrystalMerchantCard *>(merchantCardManager.getMerchantCard(merchantCardId));

    player->crystals += merchantCard->getCrystals();}

void Game::_upgradePlayMove(Player *player, const UpgradePlayMove &move) {
    uint8_t merchantCardId = move.getMerchantCardId();

    std::vector<CrystalUpgrade> upgrades = move.getUpgrades();

    // FIXME: not guaranteed the merchant card is Upgrade type
    UpgradeMerchantCard *merchantCard = static_cast<UpgradeMerchantCard *>(merchantCardManager.getMerchantCard(merchantCardId));

    // Validate number of upgrades with merchant card
    if (merchantCard->getNumUpgrades() < upgrades.size()) {
        std::cout << "Upgrade: too many upgrades\n";
        return;
    }

    // Validate upgrades
    for (const CrystalUpgrade &upgrade : upgrades) {
        if ((static_cast<uint8_t>(upgrade.toCrystal) - static_cast<uint8_t>(upgrade.fromCrystal)) != 1) {
            std::cout << "Upgrade: invalid upgrade\n";
            return;
        }
    }

    Crystals newCrystals = player->crystals;
    for (const CrystalUpgrade &upgrade : upgrades) {
        if (newCrystals.getCrystal(upgrade.fromCrystal) == 0) {
            std::cout << "Upgrade: not enough crystals\n";
            return;
        }
        newCrystals.removeCrystal(upgrade.fromCrystal);
        newCrystals.addCrystal(upgrade.toCrystal);
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
    player->crystals += toCrystals;}

