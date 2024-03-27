#ifndef USER_H
#define USER_H

#include <cstdint>
#include <string>

using UserId = std::string;


class User {
public:
    static UserId getNextId() {
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

        constexpr const int len = 8;
        UserId nextId;
        nextId.reserve(len); // 8 long UserIds

        for (int i = 0; i < len; i++) {
            nextId += alphanum[rand() % (sizeof(alphanum) - 1)];
        }
        
        return nextId;
    }

    // UserId id;
    // std::string name;

};


#endif
