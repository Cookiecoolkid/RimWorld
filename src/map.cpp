#include <iostream>
#include "map.h"
#include "config.h"

Tile::Tile(Type type)
    : m_type(type) {}

Tile::Type Tile::getType() const {
    return m_type;
}

void Tile::setType(Type type) {
    m_type = type;
}

Map::Map(int width, int height)
    : m_width(width), m_height(height) {
    m_tiles.resize(m_width);
    for (int x = 0; x < m_width; ++x) {
        m_tiles[x].resize(m_height);
    }
    srand(time(nullptr));
}

Tile Map::getTile(int x, int y) const {
    return m_tiles[x][y];
}

void Map::setTile(int x, int y, Tile::Type type) {
    m_tiles[x][y].setType(type);
}

void Map::placeRandomTrees(int count) {
    for (int i = 0; i < count; ++i) {
        int x = std::rand() % m_width;
        int y = std::rand() % m_height;

        setTile(x, y, Tile::TREE);
    }
}

bool Map::canMoveTo(int x, int y) {
    if (x < 0 || x >= Config::MAP_WIDTH || y < 0 || y >= Config::MAP_HEIGHT) {
        return false;
    }
    return getTile(x, y).getType() == Tile::EMPTY;
}

// 这里需要注意 Tile 是和 Animal 的 x, y 属性对应，因此只有更新 x,y 的时候才可以更新 Tile 以保持同步
void Map::tryUpdateAnimalTile(int index) {
    if (m_animal_entity[index].isMoving) {
        m_animal_entity[index].moveProgress++;
        if (m_animal_entity[index].moveProgress >= Config::ANIMAL_MOVE_FRAMES) {
            // 更新 x,y 的同时更新 Tile
            // 但由于 updateAnimalTile 的实现是根据 x,y 还没有更新的值来更新 Tile 的，因此要先调用 updateAnimalTile
            updateAnimalTile(index);
            m_animal_entity[index].updatePosition();
        }
    }
}

void Map::updateAnimalTile(int index) {
    setTile(m_animal_entity[index].x, m_animal_entity[index].y, Tile::EMPTY);
    setTile(m_animal_entity[index].targetX, m_animal_entity[index].targetY, Tile::ANIMAL);
}


const Animal& Map::getAnimalAt(int x, int y) const {
    // 这里的 x, y 与 tile 的 x, y 对应
    for (const auto& animal : m_animal_entity) {
        if (animal.x == x && animal.y == y) {
            return animal;
        }
    }
    throw std::runtime_error("Animal not found at the given position");
}