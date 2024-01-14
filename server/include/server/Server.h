#ifndef SERVER_H
#define SERVER_H

#include "nlohmann/json.hpp"
#include <string>

namespace Server {

    enum MessageType {
        UserId = 0,
        Lobby = 1,
        Game = 2,
        // GameEnd = 2, don't need this state right now since the game end is in the serialized game state, may need in the future with a more complicated protocol
    };

    // Payload is a serialized lobby or game or userId
    std::string createMessage(Server::MessageType messageType, nlohmann::json payload) {
        nlohmann::json data;

        data["messageType"] = messageType;
        data["payload"] = payload;

        return data.dump();
    }

}

#endif