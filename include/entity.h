#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>
#include <string>
#include <utility>  

class Entity {
public:
    enum Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    virtual ~Entity() = default;
    Direction direction;
};

class Animal : public Entity {
public:
    Animal();
    ~Animal() = default;
    std::pair<int, int> action();
    
    int old_x = 0;
    int old_y = 0;
    int x = 0;
    int y = 0;
};

class Player : public Entity {
public:
    Player();
    ~Player() = default;
    void action();
};

#endif // ENTITY_H