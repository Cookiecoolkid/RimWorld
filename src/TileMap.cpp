#include <iostream>
#include "TileMap.h"

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
    m_tiles.resize(m_height);
    for (int x = 0; x < m_height; ++x) {
        m_tiles[x].resize(m_width);
    }
}

Tile Map::getTile(int x, int y) const {
    return m_tiles[x][y];
}

void Map::setTile(int x, int y, Tile::Type type) {
    m_tiles[x][y].setType(type);
}