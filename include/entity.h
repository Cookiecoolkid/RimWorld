#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>
#include <string>
#include "map.h"

class Entity {
public:
    enum Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    virtual ~Entity() = default;
    virtual void action(Map& map) = 0;

protected:
    Direction direction;
};

class Animal : public Entity {
public:
    Animal();
    ~Animal() = default;
    void action(Map& map) override;
    int getX() const;
    int getY() const;

private:
    bool canMoveTo(int x, int y, const Map& map);
    void updateTile(Map& map, int new_x, int new_y);
    int x, y; // 当前动物的位置
};

class Player : public Entity {
public:
    Player();
    ~Player() = default;
    void action(Map& map) override;
};

#endif // ENTITY_H