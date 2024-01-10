#ifndef CLIENT_H
#define CLIENT_H

#include "game/Game.h"
#include "game/Crystals.h"
#include "game/Move.h"
#include "server/util.h"
#include "server/User.h"
#include <string>
#include <vector>
#include <iostream>

namespace Client {

    enum MessageType {
        JoinLobby = 0,
        StartGame = 1,
        Move = 2,
        RemoveCrystalOverflow = 3
    };

    void handleMoveMessage(Game &game, UserId userId, std::string message);
    void _handlePlayMoveMessage(Game &game, UserId userId, std::string message);

    void handleRemoveCrystalOverflowMessage(Game &game, UserId userId, std::string message);

    // FIXME: define this in the header?
    // FIXME: handle each move in their own function?
    void handleMoveMessage(Game &game, UserId userId, std::string message) {
        switch(message[0] - '0') {
            case static_cast<int>(Game::MoveType::PlayMove): {
                std::cout << "Play " << message.substr(1) << '\n';

                _handlePlayMoveMessage(game, userId, message.substr(1));
                break;
            }

            case static_cast<int>(Game::MoveType::AcquireMove): {
                std::cout << "Acquire " << message.substr(1, 2) << '\n';

                uint8_t merchantCardId = stringToUINT8_T(message.substr(1, 2));                
                std::vector<Crystal> crystals;
                uint8_t numCrystals = static_cast<uint8_t>(message[3] - '0');
                
                for (uint8_t i = 0; i < numCrystals; i++) {
                    crystals.push_back(static_cast<Crystal>(message[4 + i] - '0'));
                }

                game.move(userId, Game::AcquireMove(merchantCardId, crystals));
                break;
            }

            case static_cast<int>(Game::MoveType::RestMove): {
                std::cout << "Rest\n";

                game.move(userId, Game::RestMove());
                break;
            }

            case static_cast<int>(Game::MoveType::ClaimMove): {
                std::cout << "Claim " << message.substr(1, 2) << '\n';

                uint8_t pointCardId = stringToUINT8_T(message.substr(1, 2));

                game.move(userId, Game::ClaimMove(pointCardId));
                break;
            }

        }
    }
    
    void _handlePlayMoveMessage(Game &game, UserId userId, std::string message) {
        uint8_t merchantCardId = stringToUINT8_T(message.substr(1, 2));
        switch (message[0] - '0') {
            case static_cast<int>(Game::PlayMoveType::CrystalPlayMove): {
                std::cout << "Client Handler: crystal play move\n";

                game.move(userId, Game::CrystalPlayMove(merchantCardId));
                break;
            }
            case static_cast<int>(Game::PlayMoveType::UpgradePlayMove): {
                std::cout << "Client Handler: upgrade play move\n";

                std::vector<CrystalUpgrade> upgrades;
                uint8_t numUpgrades = static_cast<uint8_t>(message[3] - '0');

                for (uint8_t i = 0; i < numUpgrades; i++) {
                    upgrades.push_back(CrystalUpgrade(
                        static_cast<Crystal>(message[4 + 2 * i] - '0'),
                        static_cast<Crystal>(message[4 + 2 * i + 1] - '0')
                    ));
                }

                game.move(userId, Game::UpgradePlayMove(merchantCardId, upgrades));
                break;
            }
            case static_cast<int>(Game::PlayMoveType::TradePlayMove): {
                std::cout << "Client Handler: trade play move\n";

                uint8_t numTrades = stringToUINT8_T(message.substr(3, 2));
                game.move(userId, Game::TradePlayMove(merchantCardId, numTrades));
                break;
            }
        }
    }

    void handleRemoveCrystalOverflowMessage(Game &game, UserId userId, std::string message) {
        uint8_t yellows = stringToUINT8_T(message.substr(0, 2));
        uint8_t greens = stringToUINT8_T(message.substr(2, 2));
        uint8_t blues = stringToUINT8_T(message.substr(4, 2));
        uint8_t pinks = stringToUINT8_T(message.substr(6, 2));
        
        game.removeCrystalOverflow(userId, Crystals(yellows, greens, blues, pinks));
    }

};

#endif
