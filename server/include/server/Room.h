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
    Lobby(RoomId id, UserId hostId)
        : id(id), hostId(hostId), userIds({hostId}) {}

    void addUser(UserId userId);

    void removeUser(UserId userId);

    uint8_t getNumUsers() const { return static_cast<uint8_t>(userIds.size()); }

    std::unordered_set<UserId> getUserIds() const { return userIds; }

    nlohmann::json serialize() const;

private:
    RoomId id;
    UserId hostId;
    std::unordered_set<UserId> userIds;

};

class LobbyManager {
public:
    bool hasLobby(RoomId roomId);

    // FIXME: unsure if addLobby needs to be public
    void addLobby(RoomId roomId, UserId hostId);

    void removeLobby(RoomId roomId);

    void addUser(RoomId roomId, UserId userId);

    void removeUser(UserId userId);

    std::unordered_set<UserId> getUserIds(RoomId roomId);

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

    void addGame(RoomId roomId, std::unordered_set<UserId> userIds);

    void removeGame(RoomId roomId);

    void endGame(RoomId roomId); // End game on players leaving

    nlohmann::json serializeGame(RoomId roomId) const;

    void printState();
    
private:
    std::unordered_map<RoomId, Game*> games;

};

#endif
