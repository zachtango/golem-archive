#ifndef POINT_CARD_MANAGER_H
#define POINT_CARD_MANAGER_H

#include "nlohmann/json.hpp"
#include "Crystals.h"
#include <cstdint>
#include <unordered_map>
#include <fstream>


class PointCard {
public:
    PointCard(uint8_t numPoints, uint8_t yellows, uint8_t greens, uint8_t blues, uint8_t pinks) :
        numPoints(numPoints),
        crystals(Crystals(yellows, greens, blues, pinks)) {}
    uint8_t getNumPoints() const { return numPoints; }
    Crystals getCrystals() const { return crystals; }
private:
    uint8_t numPoints;
    Crystals crystals;
};

class PointCardManager {
public:
    PointCardManager() {
        std::ifstream fin("models/PointCards.json");
        nlohmann::json data;
        fin >> data;
        for (auto pointCard : data) {
            uint8_t id = pointCard["id"];
            uint8_t numPoints = pointCard["numPoints"];
            pointCards[id] = new PointCard(
                numPoints,
                pointCard["crystals"][0],
                pointCard["crystals"][1],
                pointCard["crystals"][2],
                pointCard["crystals"][3]
            );
        }
    }

    PointCard* getPointCard(uint8_t pointCardId) {
        PointCard* pointCard = pointCards.at(pointCardId);
        return pointCard;
    }

    ~PointCardManager() {
        for (auto [_, pointCard] : pointCards) {
            delete pointCard;
        }
    }
private:
    std::unordered_map<int, PointCard*> pointCards;
};


#endif