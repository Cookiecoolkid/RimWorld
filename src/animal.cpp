#include "entity.h"
#include "config.h"
#include <cstdlib>
#include <ctime>

Animal::Animal() {
    direction = LEFT; // 初始方向
    x = rand() % Config::MAP_WIDTH;
    y = rand() % Config::MAP_HEIGHT;
}

std::pair<int, int> Animal::action() {
    static int frame_count = 0;
    if (frame_count < Config::ANIMAL_MOVE_FRAMES) {
        frame_count++;
        return std::make_pair(-1, -1);
    }
    frame_count = 0;

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
            break;
        case RIGHT:
            new_x++;
            break;
    }
    return std::make_pair(new_x, new_y);
}
