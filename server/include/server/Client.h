#ifndef CLIENT_H
#define CLIENT_H

#include "game/Game.h"
#include "game/Crystals.h"
#include "game/Move.h"
#include "server/util.h"
#include "server/User.h"
#include <string>
#include <vector>

namespace Client {

    enum MessageType {
        JoinLobby = 0,
        StartGame = 1,
        Move = 2
    };

    void handleMoveMessage(Game &game, UserId userId, std::string message);
    void _handlePlayMoveMessage(Game &game, UserId userId, std::string message);

    // FIXME: define this in the header?
    // FIXME: handle each move in their own function?
    void handleMoveMessage(Game &game, UserId userId, std::string message) {
        switch(message[0] - '0') {
            case static_cast<int>(Game::MoveType::PlayMove): {
                _handlePlayMoveMessage(game, userId, message.substr(1));
            }

            case static_cast<int>(Game::MoveType::AcquireMove): {
                uint8_t merchantCardId = stringToUINT8_T(message.substr(1, 2));                
                std::vector<Crystal> crystals;
                uint8_t numCrystals = static_cast<uint8_t>(message[3] - '0');
                
                for (uint8_t i = 0; i < numCrystals; i++) {
                    crystals.push_back(static_cast<Crystal>(message[4 + i]));
                }

                game.move(userId, Game::AcquireMove(merchantCardId, crystals));
            }

            case static_cast<int>(Game::MoveType::RestMove): {
                game.move(userId, Game::RestMove());
            }

            case static_cast<int>(Game::MoveType::ClaimMove): {
                uint8_t pointCardId = stringToUINT8_T(message.substr(1, 2));

                game.move(userId, Game::ClaimMove(pointCardId));
            }

        }
    }
    
    void _handlePlayMoveMessage(Game &game, UserId userId, std::string message) {
        uint8_t merchantCardId = stringToUINT8_T(message.substr(1, 2));
        switch (message[0] - '0') {
            case static_cast<int>(Game::PlayMoveType::CrystalPlayMove): {
                game.move(userId, Game::CrystalPlayMove(merchantCardId));
            }
            case static_cast<int>(Game::PlayMoveType::UpgradePlayMove): {
                std::vector<CrystalUpgrade> upgrades;
                uint8_t numUpgrades = static_cast<uint8_t>(message[1] - '0');

                for (uint8_t i = 0; i < numUpgrades; i++) {
                    upgrades.push_back(CrystalUpgrade(
                        static_cast<Crystal>(message[3 + 2 * i]),
                        static_cast<Crystal>(message[3 + 2 * i + 1])
                    ));
                }

                game.move(userId, Game::UpgradePlayMove(merchantCardId, upgrades));
            }
            case static_cast<int>(Game::PlayMoveType::TradePlayMove): {
                uint8_t numTrades = stringToUINT8_T(message.substr(3, 2));
                game.move(userId, Game::TradePlayMove(merchantCardId, numTrades));
            }
        }
    }

};

#endif
