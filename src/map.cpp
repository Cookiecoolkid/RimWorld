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

    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            if (getTile(i, j).getType() == Tile::TREE) {
                std::cout << "T" << " ";
            } else {
                std::cout << "-" << " ";
            }
        }
        std::cout << std::endl;
    }
}