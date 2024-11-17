#include "entity.h"
#include "config.h"
#include <cstdlib>
#include <ctime>

// Map will init animal
Animal::Animal() {}

// Map will init player
Player::Player() {}

std::pair<int, int> Animal::action() {
    if (isMoving) {
        return std::make_pair(-1, -1);
    }

    // 随机选择一个方向
    Direction new_direction = static_cast<Direction>(std::rand() % 4);
    direction = new_direction;

    int new_x = x;
    int new_y = y;

    switch (new_direction) {
        case UP:
            new_y--;
            break;
        case DOWN:
            new_y++;
            break;
        case LEFT:
            new_x--;
            img_direction = LEFT;
            break;
        case RIGHT:
            new_x++;
            img_direction = RIGHT;
            break;
    }
    return std::make_pair(new_x, new_y);
}

void Entity::startMove(int new_x, int new_y) {
    targetX = new_x;
    targetY = new_y;
    moveProgress = 0;
    isMoving = true;
}

void Entity::updatePosition() {
    x = targetX;
    y = targetY;
    isMoving = false;
}

void Player::action_cut(int x, int y) {
    // 执行砍树动作
    // 设置 isFree = false，表示正在执行动作
    isFree = false;
    // 具体的砍树逻辑
}

void Player::action_store(int x, int y) {
    // 执行存储动作
    // 设置 isFree = false，表示正在执行动作
    isFree = false;
    // 具体的存储逻辑
}

void Player::action_pickup(int x, int y) {
    // 执行拾取动作
    // 设置 isFree = false，表示正在执行动作
    isFree = false;
    // 具体的拾取逻辑
}

void Player::updatePlayerDirection(int nextX, int nextY) {
    if (nextX > x) {
        direction = RIGHT;
    } else if (nextX < x) {
        direction = LEFT;
    } else if (nextY > y) {
        direction = DOWN;
    } else if (nextY < y) {
        direction = UP;
    }
}