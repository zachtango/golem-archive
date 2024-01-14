#ifndef WEB_SOCKET_MANAGER_H
#define WEB_SOCKET_MANAGER_H

#include "App.h"
#include "server/User.h"
#include "server/Room.h"
#include <unordered_map>
#include <string_view>
#include <iostream>


/* ws->getUserData returns one of these */
struct PerSocketData {
    /* Fill with user data */

    UserId id;
    RoomId roomId;
};

class WebSocketManager {
using WebSocket = uWS::WebSocket<false, true, PerSocketData>;

public:
    WebSocketManager() : app(nullptr) {}

    void init(uWS::App *app) { this->app = app; }

    void addWebSocket(UserId id, WebSocket *ws) {
        if (sockets.count(id)) {
            return;
        }

        sockets[id] = ws;
    }

    void removeWebSocket(UserId id) {
        sockets.at(id)->close();
        sockets.erase(id);
    }

    // https://github.com/uNetworking/uWebSockets/blob/master/src/WebSocketContext.h#L268
    // no cleanup for channels because web socket automatically unsubscribes from the channels on close
    void subscribe(std::string_view roomId, UserId id) {
        sockets.at(id)->subscribe(roomId);
    }

    void broadcast(std::string_view channel, std::string_view message, uWS::OpCode opCode) {
        app->publish(channel, message, opCode);
    }

    void send(UserId id, std::string_view message, uWS::OpCode opCode) {
        sockets.at(id)->send(message, opCode);
    }

    void printState() {
        std::cout << "----WebSockets----\n";
        for (const auto &[userId, _] : sockets) {
            std::cout << '\t' << userId << '\n';
        }
        std::cout << '\n';
    }

private:
    uWS::App *app;
    std::unordered_map<UserId, WebSocket*> sockets;

};

#endif