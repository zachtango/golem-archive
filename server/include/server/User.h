#ifndef USER_H
#define USER_H

#include <cstdint>
#include <string>

using UserId = uint64_t;


class User {
public:
    static UserId getNextId() {
        static UserId nextId = 0;
        return nextId++;
    }

    // UserId id;
    // std::string name;

};


#endif
