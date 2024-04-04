#include "server/User.h"
#include "game/Game.h"
#include "game/PointCardManager.h"
#include "game/MerchantCardManager.h"
#include "game/Player.h"
#include "game/Move.h"
#include "game/Crystals.h"
#include <memory>
#include <cstdlib>
#include <vector>
#include <string>


class Bot : public Game::Player {
public:

    Bot(UserId id, std::string userName, uint8_t turn, Crystals crystals)
        : Game::Player(id, userName, turn, crystals) {
        isBot = true;
    }

    std::unique_ptr<Game::Move> getMove(const Game &game) {
        const Game::Player* bot = game.players.at(userId);

        // If can claim a point card, claim a point card
        const PointCard *pointCardToBuy = nullptr;
        for (const auto id : game.activePointCardIds) {
            const PointCard *pointCard = game.pointCardManager.getPointCard(id);

            if (bot->crystals >= pointCard->getCrystals()) {
                pointCardToBuy = (pointCardToBuy != nullptr && pointCardToBuy->getCrystals() >= pointCard->getCrystals()) ? pointCardToBuy : pointCard;
            }
        }

        if (pointCardToBuy != nullptr) {
            return std::make_unique<Game::ClaimMove>(pointCardToBuy->getId());
        }

        int randomNumber = std::rand() % 100;

        // 20% to acquire a merchant card
        if (randomNumber < 20) {
            return std::make_unique<Game::AcquireMove>(*game.activeMerchantCardIds.begin());
        }

        // 80% to play a merchant card
        std::vector<uint8_t> canPlayMerchantCardIds;

        for (const auto merchantCardId : bot->merchantCardIds) {
            const MerchantCard *merchantCard = game.merchantCardManager.getMerchantCard(merchantCardId);

            if (canPlay(bot, merchantCard)) {
                canPlayMerchantCardIds.push_back(merchantCardId);
            }
        }

        if (canPlayMerchantCardIds.size() == 0) {
            // Rest if can't play any merchant cards
            return std::make_unique<Game::RestMove>();
        }
        
        uint8_t playMerchantCardId = canPlayMerchantCardIds[std::rand() % canPlayMerchantCardIds.size()];
        const MerchantCard *playMerchantCard = game.merchantCardManager.getMerchantCard(playMerchantCardId);

        switch(playMerchantCard->getMerchantCardType()) {
            case MerchantCardType::Crystal:
                return std::make_unique<Game::CrystalPlayMove>(playMerchantCardId);
            case MerchantCardType::Trade: {
                Crystals fromCrystals = static_cast<const TradeMerchantCard *>(playMerchantCard)->getFromCrystals();
                uint8_t numTrades = 0;

                while (bot->crystals >= fromCrystals) {
                    fromCrystals += fromCrystals;
                    numTrades += 1;
                }

                return std::make_unique<Game::TradePlayMove>(playMerchantCardId, numTrades);
            }
            // case MerchantCardType::Upgrade:
            //     return player->crystals > 0;
        }

        // FIXME throw an error
        return std::make_unique<Game::RestMove>();
    }

private:
    UserId userId;

    bool canPlay(const Game::Player *player, const MerchantCard *merchantCard) {
        switch(merchantCard->getMerchantCardType()) {
            case MerchantCardType::Crystal:
                return true;
            case MerchantCardType::Trade:
                return player->crystals >= static_cast<const TradeMerchantCard *>(merchantCard)->getFromCrystals();
            case MerchantCardType::Upgrade:
                return false;
                // return player->crystals > 0; for now don't upgrade lols
        }

        // FIXME throw an error
        return false;
    }

};
