#include "entity.h"
#include "config.h"
#include <cstdlib>
#include <ctime>

Animal::Animal() {
    direction = LEFT; // 初始方向
    x = rand() % Config::MAP_WIDTH;
    y = rand() % Config::MAP_HEIGHT;
}

void Animal::action(Map& map) {
    static int frame_count = 0;
    if (frame_count < Config::ANIMAL_MOVE_FRAMES) {
        frame_count++;
        return;
    }
    frame_count = 0;

    // 随机选择一个方向
    Direction new_direction = static_cast<Direction>(std::rand() % 4);
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

    // 检查新位置是否可以移动
    if (canMoveTo(new_x, new_y, map)) {
        direction = new_direction;
        updateTile(map, new_x, new_y);
    }
}

bool Animal::canMoveTo(int x, int y, const Map& map) {
    if (x < 0 || x >= Config::MAP_WIDTH || y < 0 || y >= Config::MAP_HEIGHT) {
        return false;
    }
    return map.getTile(x, y).getType() == Tile::EMPTY;
}

void Animal::updateTile(Map& map, int new_x, int new_y) {
    Tile::Type new_type = map.getTile(x, y).getType();
    switch (direction) {
        case LEFT:
            new_type = Tile::ANIMAL_LEFT;
            break;
        case RIGHT:
            new_type = Tile::ANIMAL_RIGHT;
            break;
        default: // remain the same
            break;
    }
    map.setTile(x, y, Tile::EMPTY);
    map.setTile(new_x, new_y, new_type);
    x = new_x;
    y = new_y;
}

int Animal::getX() const {
    return x;
}

int Animal::getY() const {
    return y;
}

