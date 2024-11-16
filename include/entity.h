#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>
#include <string>
#include <utility>  
#include <vector>

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
    Direction img_direction;
};

class Animal : public Entity {
public:
    Animal();
    ~Animal() = default;
    std::pair<int, int> action();
    
    void startMove(int new_x, int new_y);
    void updatePosition();
    
    int x = 0;
    int y = 0;

    int targetX = x;
    int targetY = y;
    int moveProgress = 0;
    int isMoving = false;
};

class Player : public Entity {
public:
    Player();
    ~Player() = default;

    int x = 0;
    int y = 0;
    bool isFree = true; // 表示当前是否在执行一个动作
    std::vector<std::pair<int, int>> path; // 存储寻路路径
    int moveProgress = 0; // 移动进度
};

#endif // ENTITY_H