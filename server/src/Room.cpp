#include "game/Game.h"
#include "server/Room.h"


void Lobby::addUser(UserId userId, const std::string &userName) {
    userIdToName.insert({userId, userName});
}

void Lobby::addBot(UserId botId, const std::string &botName) {
    botIdToName.insert({botId, botName});
}

void Lobby::removeUser(UserId userId) {
    userIdToName.erase(userId);

    if (userId == hostId && userIdToName.size()) {
        hostId = userIdToName.begin()->first;
    }
}

void Lobby::changeUserName(UserId userId, const std::string &newName) { 
    if (!userIdToName.count(userId)) {
        return;
    }
    userIdToName[userId] = newName;
}

nlohmann::json Lobby::serialize() const {
    nlohmann::json data;

    data["id"] = id;
    data["hostId"] = hostId;
    data["userIdToName"] = userIdToName;

    return data;
}

bool LobbyManager::hasLobby(RoomId roomId) {
    return lobbies.count(roomId);
}

// FIXME: unsure if addLobby needs to be public
void LobbyManager::addLobby(RoomId roomId, UserId hostId, const std::string &hostName) {
    if (hasLobby(roomId)) {
        return;
    }

    lobbies[roomId] = new Lobby(roomId, hostId, hostName);

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

void LobbyManager::addUser(RoomId roomId, UserId userId, const std::string &userName) {
    // Remove user from previous lobby
    if (userIdToRoomId.count(userId)) {
        removeUser(userId);
    }

    if (!hasLobby(roomId)) {
        addLobby(roomId, userId, userName);
    }

    lobbies[roomId]->addUser(userId, userName);
    userIdToRoomId[userId] = roomId;
}

void LobbyManager::addBot(RoomId roomId, UserId botId, const std::string &botName) {
    lobbies.at(roomId)->addBot(botId, botName);
}

void LobbyManager::removeUser(UserId userId) {
    RoomId roomId = userIdToRoomId.at(userId);

    auto lobby = lobbies[roomId];
    lobby->removeUser(userId);

    if (lobby->getNumUsers() == 0) {
        removeLobby(roomId);
    }

    userIdToRoomId.erase(userId);
}

void LobbyManager::changeUserName(RoomId roomId, UserId userId, const std::string &newName) {
    if (!hasLobby(roomId)) {
        return;
    }
    lobbies[roomId]->changeUserName(userId, newName);
}

std::vector<UserId> LobbyManager::getUserIds(RoomId roomId) {
    return lobbies.at(roomId)->getUserIds();
}

std::vector<UserId> LobbyManager::getBotIds(RoomId roomId) {
    return lobbies.at(roomId)->getBotIds();
}

std::vector<UserId> LobbyManager::getBotNames(RoomId roomId) {
    return lobbies.at(roomId)->getBotNames();
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

void GameManager::addGame(RoomId roomId, const std::vector<UserId> &userIds, const std::vector<std::string> &userNames, const std::vector<UserId> &botIds, const std::vector<std::string> &botNames) {
    if (hasGame(roomId)) {
        return;
    }

    games[roomId] = new Game(roomId, userIds, userNames, botIds, botNames);
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
