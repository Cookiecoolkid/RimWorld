#include "entity.h"
#include "config.h"
#include <cstdlib>
#include <ctime>

// Map will init animal
Animal::Animal() {}

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

void Animal::startMove(int new_x, int new_y) {
    targetX = new_x;
    targetY = new_y;
    moveProgress = 0;
    isMoving = true;
}

void Animal::updatePosition() {
    x = targetX;
    y = targetY;
    isMoving = false;
}