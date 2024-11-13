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

void Map::updateAnimalTile(int index, int new_x, int new_y) {
    Tile::Type new_type = getTile(m_animal_entity[index].x, m_animal_entity[index].y).getType();
    switch (m_animal_entity[index].direction) {
        case Entity::LEFT:
            new_type = Tile::ANIMAL_LEFT;
            break;
        case Entity::RIGHT:
            new_type = Tile::ANIMAL_RIGHT;
            break;
        default: // remain the same
            break;
    }
    m_animal_entity[index].old_x = m_animal_entity[index].x;
    m_animal_entity[index].old_y = m_animal_entity[index].y;

    m_animal_entity[index].x = new_x;
    m_animal_entity[index].y = new_y;

    setTile(m_animal_entity[index].old_x, m_animal_entity[index].old_y, Tile::EMPTY);
    setTile(m_animal_entity[index].x, m_animal_entity[index].y, new_type);
}