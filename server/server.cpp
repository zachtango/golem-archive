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


int main() {
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
        .upgrade = [](auto *res, auto *req, auto *context) {
       	    std::cout << "Connection\n";	    
            /* You may read from req only here, and COPY whatever you need into your PerSocketData.
             * PerSocketData is valid from .open to .close event, accessed with ws->getUserData().
             * HttpRequest (req) is ONLY valid in this very callback, so any data you will need later
             * has to be COPIED into PerSocketData here. */

            res->template upgrade<PerSocketData>(
                {
                    .id = User::getNextId(),
                    .roomId = ""
                },
                req->getHeader("sec-websocket-key"),
                req->getHeader("sec-websocket-protocol"),
                req->getHeader("sec-websocket-extensions"),
                context);
        },

        /* Socket just opened */
        .open = [&wsManager](auto *ws) {
            PerSocketData *socketData = ws->getUserData();
            UserId userId = socketData->id;

            wsManager.addWebSocket(userId, ws);

            nlohmann::json data;
            data["userId"] = userId;

            wsManager.send(
                userId,
                Server::createMessage(Server::MessageType::UserId, data),
                uWS::OpCode::TEXT
            );

            std::cout << "User " << userId << " connected\n";
        },

        /* Client communicating a message to server */
        .message = [&lobbyManager, &gameManager, &wsManager](auto *ws, std::string_view message, uWS::OpCode /* opCode */) {
            PerSocketData *socketData = ws->getUserData();
            UserId userId = socketData->id;

            std::string messageString = static_cast<std::string>(message);
            
            std::cout << "Message: " << messageString << '\n';

            switch (messageString[0] - '0') {
                case Client::MessageType::JoinLobby: {
                    RoomId roomId = messageString.substr(1, 8);
                    socketData->roomId = roomId;

                    // Add user to given lobby
                    lobbyManager.addUser(roomId, userId);
                    
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

                    auto userIds = lobbyManager.getUserIds(roomId);
                    
                    if (userIds.size() < 2) {
                        // Can't start a game with less than 2 people
                        break;
                    }

                    gameManager.addGame(roomId, userIds);
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
            }

            lobbyManager.printState();
            gameManager.printState();
            wsManager.printState();

        },

        /* Cleanup resources with socket closes */
        .close = [&lobbyManager, &gameManager, &wsManager](auto *ws, int /*code*/, std::string_view /*message*/) {
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
                gameManager.endGame(roomId);

                // Broadcast to web socket channel game is finished because user left
                wsManager.broadcast(
                    roomId,
                    Server::createMessage(Server::MessageType::Game, gameManager.serializeGame(roomId)),
                    uWS::OpCode::TEXT
                );

                gameManager.removeGame(roomId);
            }

            std::cout << "User " << userId << " disconnected\n";

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
