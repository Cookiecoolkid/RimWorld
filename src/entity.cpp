#include "entity.h"
#include "config.h"
#include <cstdlib>
#include <ctime>

// Map will init animal
Animal::Animal() {}

// Map will init player
Player::Player() {}

std::pair<int, int> Animal::action() {
    if (isStepping) {
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

void Entity::startStep(int new_x, int new_y) {
    targetX = new_x;
    targetY = new_y;
    stepProgress = 0;
    isStepping = true;
}

void Entity::updatePosition() {
    x = targetX;
    y = targetY;
    isStepping = false;
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


