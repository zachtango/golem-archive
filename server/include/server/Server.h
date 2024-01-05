#ifndef SERVER_H
#define SERVER_H

#include "nlohmann/json.hpp"
#include <string>

namespace Server {

    enum MessageType {
        Lobby = 0,
        Game = 1,
        // GameEnd = 2, don't need this state right now since the game end is in the serialized game state, may need in the future with a more complicated protocol
    };

    // Payload is a serialized lobby or game
    std::string createMessage(Server::MessageType messageType, nlohmann::json payload) {
        nlohmann::json data;

        data["messageType"] = messageType;
        data["payload"] = payload;

        return data.dump();
    }

}

#endif