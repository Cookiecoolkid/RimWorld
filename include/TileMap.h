#ifndef TILEMAP_H
#define TILEMAP_H

#include <iostream>
#include <vector>

class Tile {
public:
    enum Type { EMPTY, TREE, WALL, DOOR };

    Tile(Type type = EMPTY);

    Type getType() const;
    void setType(Type type);

private:
    Type m_type;
};

class Map {
public:
    Map(int width, int height);

    Tile getTile(int x, int y) const;

    void setTile(int x, int y, Tile::Type type);

private:
    int m_width;
    int m_height;
    std::vector<std::vector<Tile>> m_tiles;
};

#endif // TILEMAP_H
