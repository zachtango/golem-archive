#ifndef MOVE_H
#define MOVE_H

#include "Game.h"
#include "Crystals.h"
#include <cstdint>
#include <vector>


class Game::Move {
public:
    virtual Game::MoveType getMoveType() = 0;

};

class Game::PlayMove : public Game::Move {
public:
    PlayMove(uint8_t merchantCardId) : merchantCardId(merchantCardId) {}

    Game::MoveType getMoveType() { return Game::MoveType::PlayMove; }

    virtual Game::PlayMoveType getPlayMoveType() = 0;

protected:
    uint8_t merchantCardId;
};

class Game::CrystalPlayMove : public Game::PlayMove {
public:
    CrystalPlayMove(uint8_t merchantCardId) : PlayMove(merchantCardId) {}

    Game::PlayMoveType getPlayMoveType() { return Game::PlayMoveType::CrystalPlayMove; }

};

class Game::UpgradePlayMove : public Game::PlayMove {
public:
    UpgradePlayMove(uint8_t merchantCardId, std::vector<CrystalUpgrade> upgrades)
        : PlayMove(merchantCardId), upgrades(upgrades) {}

    Game::PlayMoveType getPlayMoveType() { return Game::PlayMoveType::UpgradePlayMove; }

private:
    std::vector<CrystalUpgrade> upgrades; // [1, 3] upgrades

};

class Game::TradePlayMove : public Game::PlayMove {
public:
    TradePlayMove(uint8_t merchantCardId, uint8_t numTrades)
        : PlayMove(merchantCardId), numTrades(numTrades) {}

    Game::PlayMoveType getPlayMoveType() { return Game::PlayMoveType::TradePlayMove; }

private:
    uint8_t numTrades; // [1, 10] trades

};

class Game::AcquireMove : public Game::Move {
public:
    AcquireMove(uint8_t merchantCardId, std::vector<Crystal> crystals) : merchantCardId(merchantCardId), crystals(crystals) {}

    Game::MoveType getMoveType() { return Game::MoveType::AcquireMove; }

private:
    uint8_t merchantCardId;
    std::vector<Crystal> crystals;
};

class Game::RestMove : public Game::Move {
public:
    RestMove() {}

    Game::MoveType getMoveType() { return Game::MoveType::RestMove; }

};

class Game::ClaimMove : public Game::Move {
public:
    ClaimMove(uint8_t pointCardId) : pointCardId(pointCardId) {}

    Game::MoveType getMoveType() { return Game::MoveType::ClaimMove; }

private:
    uint8_t pointCardId;

};

#endif