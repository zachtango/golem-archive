#include "game/Game.h"
#include "server/Room.h"


void Lobby::addUser(UserId userId) {
    userIds.insert(userId);
}

void Lobby::removeUser(UserId userId) {
    userIds.erase(userId);

    if (userId == hostId && userIds.size()) {
        hostId = *userIds.begin();
    }
}

nlohmann::json Lobby::serialize() const {
    nlohmann::json data;

    data["id"] = id;
    data["hostId"] = hostId;
    data["userIds"] = userIds;

    return data;
}

bool LobbyManager::hasLobby(RoomId roomId) {
    return lobbies.count(roomId);
}

// FIXME: unsure if addLobby needs to be public
void LobbyManager::addLobby(RoomId roomId, UserId hostId) {
    if (hasLobby(roomId)) {
        return;
    }

    lobbies[roomId] = new Lobby(roomId, hostId);

    userIdToRoomId[hostId] = roomId;
}

void LobbyManager::removeLobby(RoomId roomId) {
    if (!hasLobby(roomId)) {
        return;
    }

    auto lobby = lobbies[roomId];
    auto userIds = lobby->getUserIds();

    for (auto userId : userIds) {
        userIdToRoomId.erase(userId);
    }

    delete lobbies[roomId];
    lobbies.erase(roomId);
    
}

void LobbyManager::addUser(RoomId roomId, UserId userId) {
    // Remove user from previous lobby
    if (userIdToRoomId.count(userId)) {
        removeUser(userId);
    }

    if (!hasLobby(roomId)) {
        addLobby(roomId, userId);
    }

    lobbies[roomId]->addUser(userId);
    userIdToRoomId[userId] = roomId;
}

void LobbyManager::removeUser(UserId userId) {
    RoomId roomId = userIdToRoomId.at(userId);

    auto lobby = lobbies[roomId];
    lobby->removeUser(userId);

    if (!lobby->getNumUsers()) {
        removeLobby(roomId);
    }
}

std::unordered_set<UserId> LobbyManager::getUserIds(RoomId roomId) {
    return lobbies.at(roomId)->getUserIds();
}

void LobbyManager::printState() {
    std::cout << "----Lobbies----\n";
    for (const auto &[roomId, _] : lobbies) {
        std::cout << '\t' << roomId << '\n';
    }
    for (const auto &[userId, roomId] : userIdToRoomId) {
        std::cout << '\t' << userId << ' ' << roomId << '\n';
    }
    std::cout << '\n';
}

bool GameManager::hasGame(RoomId roomId) {
    return games.count(roomId);
}

Game* GameManager::getGame(RoomId roomId) {
    return games.at(roomId);
}

nlohmann::json LobbyManager::serializeLobby(RoomId roomId) const {
    return lobbies.at(roomId)->serialize();
}

void GameManager::addGame(RoomId roomId, std::unordered_set<UserId> userIds) {
    if (hasGame(roomId)) {
        return;
    }

    games[roomId] = new Game(roomId, userIds);
}

void GameManager::removeGame(RoomId roomId) {
    if (!hasGame(roomId)) {
        return;
    }

    delete games[roomId];
    games.erase(roomId);
}

void GameManager::endGame(RoomId roomId) {
    if (!hasGame(roomId)) {
        return;
    }

    games[roomId]->endGame();
}

nlohmann::json GameManager::serializeGame(RoomId roomId) const {
    return games.at(roomId)->serialize();
}

void GameManager::printState() {
    std::cout << "----Games----\n";
    for (const auto &[roomId, _] : games) {
        std::cout << '\t' << roomId << '\n';
    }
    std::cout << '\n';
}
