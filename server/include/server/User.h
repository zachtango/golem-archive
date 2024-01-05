#ifndef USER_H
#define USER_H

#include <cstdint>
#include <string>

using UserId = uint64_t;


class User {
public:
    static void initId() { nextId = 0; }
    static UserId getNextId() { return nextId++; }

private:
    static UserId nextId;

    // UserId id;
    // std::string name;

};


#endif
