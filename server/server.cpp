/* We simply call the root header file "App.h", giving you uWS::App and uWS::SSLApp */
#include "App.h"
#include "game/Game.h"
#include "server/Client.h"
#include "server/User.h"
#include "server/Room.h"
#include "server/Server.h"
#include "server/WebSocketManager.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdlib.h>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include <unistd.h>


int main() {
    srand((unsigned)time(NULL) * getpid());

    std::ifstream adjectives_file("models/Adjectives.json");
    std::ifstream animals_file("models/Animals.json");
    
    nlohmann::json adjectives = nlohmann::json::parse(adjectives_file);
    nlohmann::json animals = nlohmann::json::parse(animals_file);

    // FIXME change to a UserManager class later on?
    std::unordered_map<UserId, std::string> users;

    LobbyManager lobbyManager;
    GameManager gameManager;
    WebSocketManager wsManager;

    /* Keep in mind that uWS::SSLApp({options}) is the same as uWS::App() when compiled without SSL support.
     * You may swap to using uWS:App() if you don't need SSL */
    auto app = uWS::App({
        /* There are example certificates in uWebSockets.js repo */
	    .key_file_name = "misc/key.pem",
	    .cert_file_name = "misc/cert.pem",
        .passphrase = "1234"
	}).ws<PerSocketData>("/*", {
        /* Settings */
        // .compression = uWS::CompressOptions(uWS::DEDICATED_COMPRESSOR_4KB | uWS::DEDICATED_DECOMPRESSOR),
        // .maxPayloadLength = 100 * 1024 * 1024,
        .idleTimeout = 0,
        // .maxBackpressure = 100 * 1024 * 1024,
        // .closeOnBackpressureLimit = false,
        // .resetIdleTimeoutOnSend = false,
        // .sendPingsAutomatically = true,
        /* Handlers */

        /* Client Initiating Socket Connection */
        .upgrade = [&wsManager](auto *res, auto *req, auto *context) {
       	    std::cout << "Connection\n";	    
            /* You may read from req only here, and COPY whatever you need into your PerSocketData.
             * PerSocketData is valid from .open to .close event, accessed with ws->getUserData().
             * HttpRequest (req) is ONLY valid in this very callback, so any data you will need later
             * has to be COPIED into PerSocketData here. */

            UserId userId = std::string(req->getQuery("userId"));
            RoomId roomId = std::string(req->getQuery("gameId"));


            // Don't allow more than one websocket connection with same user id
            if (userId.size() != 0 && wsManager.hasWebSocket(userId)) {
                res->writeStatus("403 Forbidden");
                res->end("WebSocket conneciton for this user already exists");
                return;
            }

            res->template upgrade<PerSocketData>(
                {
                    .id = userId,
                    .roomId = roomId
                },
                req->getHeader("sec-websocket-key"),
                req->getHeader("sec-websocket-protocol"),
                req->getHeader("sec-websocket-extensions"),
                context);
        },

        /* Socket just opened */
        .open = [&adjectives, &animals, &users, &wsManager, &gameManager](auto *ws) {
            PerSocketData *socketData = ws->getUserData();
            UserId userId = socketData->id;
            RoomId roomId = socketData->roomId;
            
            // Generate user id if client doesn't provide one
            if (userId.size() == 0) {
                userId = User::getNextId();
                socketData->id = userId;
            }

            // Form user name
            int adjectivesIndex = rand() % adjectives.size();
            int animalsIndex = rand() % animals.size();
            std::string adjective = adjectives[adjectivesIndex].dump();
            adjective = adjective.substr(1, adjective.size() - 2);
            std::string animal = animals[animalsIndex].dump();
            animal = animal.substr(1, animal.size() - 2);
            std::string userName = adjective + animal;

            // Add user to users map
            users[userId] = userName;
            
            // Add websocket to websocket manager
            wsManager.addWebSocket(userId, ws);

            // Send user info to client
            nlohmann::json data;
            data["userId"] = userId;
            data["userName"] = userName;
            wsManager.send(
                userId,
                Server::createMessage(Server::MessageType::UserId, data),
                uWS::OpCode::TEXT
            );

            // Join user back into existing game
            if (gameManager.hasGame(roomId)) {
                Game* game = gameManager.getGame(roomId);
                
                if (game->hasUser(userId)) {
                    wsManager.send(
                        userId,
                        Server::createMessage(Server::MessageType::Game, game->serialize()),
                        uWS::OpCode::TEXT
                    );

                    wsManager.subscribe(roomId, userId);
                }
            }

            std::cout << "User " << userId << " connected\n";
        },

        /* Client communicating a message to server */
        .message = [&lobbyManager, &gameManager, &wsManager, &users, &adjectives, &animals](auto *ws, std::string_view message, uWS::OpCode /* opCode */) {
            PerSocketData *socketData = ws->getUserData();
            UserId userId = socketData->id;
            RoomId roomId = socketData->roomId;

            std::string messageString = static_cast<std::string>(message);
            
            std::cout << "Message: " << messageString << '\n';

            switch (messageString[0] - '0') {
                case Client::MessageType::JoinLobby: {
                    // Don't join lobby if user is in game
                    if (gameManager.hasGame(roomId) &&
                        gameManager.getGame(roomId)->hasUser(userId)) {
                        break;
                    }

                    // Join lobby
                    roomId = messageString.substr(1, 8);
                    socketData->roomId = roomId;

                    // Add user to given lobby
                    lobbyManager.addUser(roomId, userId, users.at(userId));

                    // Subscribe to web socket channel corresponding to lobby
                    wsManager.subscribe(roomId, userId);

                    // Broadcast user joined to web socket channel
                    wsManager.broadcast(
                        roomId,
                        Server::createMessage(Server::MessageType::Lobby, lobbyManager.serializeLobby(roomId)),
                        uWS::OpCode::TEXT
                    );

                    std::cout << "User " << userId << " joined " << roomId << '\n';

                    break;
                }
                case Client::MessageType::StartGame: {
                    RoomId roomId = socketData->roomId;

                    int adjectivesIndex = rand() % adjectives.size();
                    int animalsIndex = rand() % animals.size();
                    std::string adjective = adjectives[adjectivesIndex].dump();
                    adjective = adjective.substr(1, adjective.size() - 2);
                    std::string animal = animals[animalsIndex].dump();
                    animal = animal.substr(1, animal.size() - 2);

                    lobbyManager.addBot(roomId, User::getNextId(), adjective + animal);

                    auto userIds = lobbyManager.getUserIds(roomId);
                    auto botIds = lobbyManager.getBotIds(roomId);

                    if (userIds.size() + botIds.size() < 2) {
                        // Can't start a game with less than 2 people
                        break;
                    }

                    std::vector<std::string> botNames = lobbyManager.getBotNames(roomId);
                    std::vector<std::string> userNames;

                    std::transform(userIds.begin(), userIds.end(), std::back_inserter(userNames), [&users](UserId i) {
                        return users[i];
                    });

                    gameManager.addGame(roomId, userIds, userNames, botIds, botNames);
                    lobbyManager.removeLobby(roomId);

                    // Broadcast game started to web socket channel
                    wsManager.broadcast(
                        roomId,
                        Server::createMessage(Server::MessageType::Game, gameManager.serializeGame(roomId)),
                        uWS::OpCode::TEXT
                    );

                    break;
                }
                case Client::MessageType::Move: {
                    std::cout << "User " << userId << " requests to move " << message << '\n';

                    RoomId roomId = socketData->roomId;
                    Game *game = gameManager.getGame(roomId);

                    Client::handleMoveMessage(*game, userId, messageString.substr(1));

                    // Broadcast move to web socket channel
                    wsManager.broadcast(
                        roomId,
                        Server::createMessage(Server::MessageType::Game, gameManager.serializeGame(roomId)),
                        uWS::OpCode::TEXT
                    );

                    break;
                }
                case Client::MessageType::RemoveCrystalOverflow: {
                    std::cout << "User " << userId << " removes crystal overflow " << message << '\n';
                    
                    RoomId roomId = socketData->roomId;
                    Game *game = gameManager.getGame(roomId);

                    Client::handleRemoveCrystalOverflowMessage(*game, userId, messageString.substr(1));

                    // Broadcast move to web socket channel
                    wsManager.broadcast(
                        roomId,
                        Server::createMessage(Server::MessageType::Game, gameManager.serializeGame(roomId)),
                        uWS::OpCode::TEXT
                    );

                    break;
                }
                case Client::MessageType::Chat: {
                    std::cout << "User " << userId << " chats " << message << '\n';
                    
                    RoomId roomId = socketData->roomId;
                    Game *game = gameManager.getGame(roomId);

                    Client::handleChatMessage(*game, userId, messageString.substr(1));

                    // Broadcast chat to web socket channel
                    wsManager.broadcast(
                        roomId,
                        Server::createMessage(Server::MessageType::Game, gameManager.serializeGame(roomId)),
                        uWS::OpCode::TEXT
                    );

                    break;
                }
                case Client::MessageType::ChangeName: {
                    std::cout << "User " << userId << " chats " << message << '\n';
                    
                    std::string newName = messageString.substr(1);

                    Client::handleChangeNameMessage(users, userId, newName);

                    nlohmann::json data;
                    data["userId"] = userId;
                    data["userName"] = newName;

                    // Send user name update to web socket
                    wsManager.send(
                        userId,
                        Server::createMessage(Server::MessageType::UserId, data),
                        uWS::OpCode::TEXT
                    );

                    RoomId roomId = socketData->roomId;
                    if (!lobbyManager.hasLobby(roomId)) {
                        break;
                    }

                    lobbyManager.changeUserName(roomId, userId, newName);

                    // Broadcast user name change to web socket channel
                    wsManager.broadcast(
                        roomId,
                        Server::createMessage(Server::MessageType::Lobby, lobbyManager.serializeLobby(roomId)),
                        uWS::OpCode::TEXT
                    );

                    break;
                }
                
            }

            lobbyManager.printState();
            gameManager.printState();
            wsManager.printState();

        },

        /* Cleanup resources with socket closes */
        .close = [&lobbyManager, &gameManager, &wsManager, &users](auto *ws, int /*code*/, std::string_view /*message*/) {
            // /* You may access ws->getUserData() here, but sending or
            //  * doing any kind of I/O with the socket is not valid. */
            PerSocketData *socketData = ws->getUserData();
            UserId userId = socketData->id;
            RoomId roomId = socketData->roomId;

            if (lobbyManager.hasLobby(roomId)) {
                lobbyManager.removeUser(userId);

                std::cout << "User " << userId << " removed from " << roomId << '\n';

                if (lobbyManager.hasLobby(roomId)) {
                    std::cout << "Removal of user " << userId << " broadcasted\n";
                    // Broadcast to web socket channel of user removal
                    wsManager.broadcast(
                        roomId,
                        Server::createMessage(Server::MessageType::Lobby, lobbyManager.serializeLobby(roomId)),
                        uWS::OpCode::TEXT
                    );
                }
            } else if (gameManager.hasGame(roomId)) {
                
                Game* game = gameManager.getGame(roomId);
                auto userIds = game->getUserIds();
                
                if (std::all_of(userIds.begin(), userIds.end(), [&users, &userId](UserId id) { return !users.count(id) || userId == id; })) {
                    gameManager.removeGame(roomId);
                }
            }

            std::cout << "User " << userId << " disconnected\n";

            users.erase(userId);
            wsManager.removeWebSocket(userId);

            lobbyManager.printState();
            gameManager.printState();
            wsManager.printState();
        }

    }).listen(9001, [](auto *listen_socket) {
        if (listen_socket) {
            std::cout << "Listening on port " << 9001 << std::endl;
        }
    });

    wsManager.init(&app);
    
    app.run();

}
