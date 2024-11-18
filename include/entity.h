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
    int x = 0;
    int y = 0;

    int targetX = x;
    int targetY = y;
    int stepProgress = 0;
    int isStepping = false;
    Direction direction;

    void startStep(int new_x, int new_y);
    void updatePosition();
};

class Animal : public Entity {
public:
    Animal();
    ~Animal() = default;
    std::pair<int, int> action();
    
    Direction img_direction;
};


class Player : public Entity {
public:
    Player();
    ~Player() = default;

    int optionTargetX = 0;
    int optionTargetY = 0;
    bool isFree = true; // 表示当前是否在执行一个动作
    bool isMoving = false; // 表示当前是否在移动
    bool isCutting = false; // 表示当前是否在砍树
    bool isStoring = false; // 表示当前是否在存储
    bool isPickingUp = false; // 表示当前是否在捡东西
    int cutProgress = 0; // 砍树进度
    std::vector<std::pair<int, int>> path; // 存储寻路路径
    void updatePlayerDirection(int nextX, int nextY);

    enum Action {
        NONE,
        MOVE,
        CUT,
        STORE,
        PICKUP
    };
};

#endif // ENTITY_H