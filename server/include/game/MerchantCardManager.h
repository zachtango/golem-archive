#ifndef MERCHANT_CARD_MANAGER_H
#define MERCHANT_CARD_MANAGER_H

#include "nlohmann/json.hpp"
#include "Crystals.h"
#include <cstdint>
#include <unordered_map>
#include <fstream>
#include <string>


enum class MerchantCardType {
    Crystal = 0,
    Upgrade = 1,
    Trade = 2
};

class MerchantCard {
public:
    virtual MerchantCardType getMerchantCardType() const = 0;
    virtual ~MerchantCard() {}
};

class CrystalMerchantCard : public MerchantCard {
public:
    CrystalMerchantCard(uint8_t yellow, uint8_t green, uint8_t blue, uint8_t pink) :
        crystals(Crystals(yellow, green, blue, pink)) {}
    MerchantCardType getMerchantCardType() const { return MerchantCardType::Crystal; }
    Crystals getCrystals() const { return crystals; }
private:
    Crystals crystals;
};

class UpgradeMerchantCard : public MerchantCard {
public:
    UpgradeMerchantCard(uint8_t numUpgrades) : numUpgrades(numUpgrades) {}
    MerchantCardType getMerchantCardType() const { return MerchantCardType::Upgrade; }
    uint8_t getNumUpgrades() const { return numUpgrades; }
private:
    uint8_t numUpgrades;
};

class TradeMerchantCard : public MerchantCard {
public:
    TradeMerchantCard(uint8_t fromYellows, uint8_t fromGreens, uint8_t fromBlues, uint8_t fromPinks,
                uint8_t toYellows, uint8_t toGreens, uint8_t toBlues, uint8_t toPinks) :
                fromCrystals(Crystals(fromYellows, fromGreens, fromBlues, fromPinks)),
                toCrystals(Crystals(toYellows, toGreens, toBlues, toPinks)) {}
    MerchantCardType getMerchantCardType() const { return MerchantCardType::Trade; }
    Crystals getFromCrystals() const { return fromCrystals; }
    Crystals getToCrystals() const { return toCrystals; }
private:
    Crystals fromCrystals;
    Crystals toCrystals;
};

class MerchantCardManager {
public:
    MerchantCardManager() {
        std::ifstream fin("models/MerchantCards.json");
        nlohmann::json data;
        fin >> data;
        for (auto merchantCard : data) {
            uint8_t id = merchantCard["id"];
            std::string type = merchantCard["type"];
            
            if (type == "Crystal") {
                merchantCards[id] = new CrystalMerchantCard(
                    merchantCard["crystals"][0],
                    merchantCard["crystals"][1],
                    merchantCard["crystals"][2],
                    merchantCard["crystals"][3]
                );
            } else if (type == "Upgrade") {
                merchantCards[id] = new UpgradeMerchantCard(
                    merchantCard["numUpgrades"]
                );
            } else if (type == "Trade") {
                merchantCards[id] = new TradeMerchantCard(
                    merchantCard["fromCrystals"][0],
                    merchantCard["fromCrystals"][1],
                    merchantCard["fromCrystals"][2],
                    merchantCard["fromCrystals"][3],
                    merchantCard["toCrystals"][0],
                    merchantCard["toCrystals"][1],
                    merchantCard["toCrystals"][2],
                    merchantCard["toCrystals"][3]
                );
            }
        }
    }

    MerchantCard* getMerchantCard(uint8_t merchantCardId) {
        MerchantCard* merchantCard = merchantCards.at(merchantCardId);
        return merchantCard;
    }

    const MerchantCard* getMerchantCard(uint8_t merchantCardId) const {
        return merchantCards.at(merchantCardId);
    }

    ~MerchantCardManager() {
        for (auto [_, merchantCard] : merchantCards) {
            delete merchantCard;
        }
    }
private:
    std::unordered_map<uint8_t, MerchantCard*> merchantCards;
};


#endif