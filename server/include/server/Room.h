#ifndef ROOM_H
#define ROOM_H

#include "server/User.h"
#include "nlohmann/json.hpp"
#include <unordered_map>
#include <unordered_set>
#include <string>

class Game;

using RoomId = std::string;

class Lobby {
public:
    Lobby(RoomId id, UserId hostId, const std::string &hostName)
        : id(id), hostId(hostId) {
        userIdToName[hostId] = hostName;
    }

    void addUser(UserId userId, const std::string &userName);
    
    void addBot(UserId botId, const std::string &userName);

    void removeUser(UserId userId);

    uint8_t getNumUsers() const { return static_cast<uint8_t>(userIdToName.size()); }

    std::vector<UserId> getBotNames() const {
        std::vector<std::string> names;
        for (auto const &[_, name] : botIdToName) {
            names.push_back(name);
        }
        return names;
    }

    std::vector<UserId> getUserIds() const {
        std::vector<UserId> ids;
        for (auto const &[id, _] : userIdToName) {
            ids.push_back(id);
        }
        return ids;
    }

    std::vector<UserId> getBotIds() const {
        std::vector<UserId> ids;
        for (auto const &[id, _] : botIdToName) {
            ids.push_back(id);
        }
        return ids;
    }

    void changeUserName(UserId userId, const std::string &newName);

    nlohmann::json serialize() const;

private:
    RoomId id;
    UserId hostId;
    std::unordered_map<UserId, std::string> userIdToName;
    std::unordered_map<UserId, std::string> botIdToName;
};

class LobbyManager {
public:
    bool hasLobby(RoomId roomId);

    // FIXME: unsure if addLobby needs to be public
    void addLobby(RoomId roomId, UserId hostId, const std::string &hostName);

    void removeLobby(RoomId roomId);

    void addUser(RoomId roomId, UserId userId, const std::string &userName);

    void addBot(RoomId roomId, UserId botId, const std::string &botName);

    void removeUser(UserId userId);

    void changeUserName(RoomId roomId, UserId userId, const std::string &newName);

    std::vector<UserId> getUserIds(RoomId roomId);

    std::vector<UserId> getBotIds(RoomId roomId);

    std::vector<UserId> getBotNames(RoomId roomId);

    nlohmann::json serializeLobby(RoomId roomId) const;

    void printState();

private:
    std::unordered_map<RoomId, Lobby*> lobbies;
    std::unordered_map<UserId, RoomId> userIdToRoomId;

};

class GameManager {
public:
    bool hasGame(RoomId roomId);

    Game* getGame(RoomId roomId);

    void addGame(RoomId roomId, const std::vector<UserId> &userIds, const std::vector<std::string> &userNames, const std::vector<UserId> &botIds, const std::vector<std::string> &botNames);

    void removeGame(RoomId roomId);

    void endGame(RoomId roomId); // End game on players leaving

    nlohmann::json serializeGame(RoomId roomId) const;

    void printState();
    
private:
    std::unordered_map<RoomId, Game*> games;

};

#endif
