#ifndef TILEMAP_H
#define TILEMAP_H

#include <iostream>
#include <vector>
#include <random>
#include "entity.h"


class Tile {
public:
    enum Type { EMPTY, TREE, WALL, DOOR, ANIMAL, PLAYER_UP,
                PLAYER_DOWN, PLAYER_LEFT, PLAYER_RIGHT, CUTED_TREE };  

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

    void placeRandomTrees(int count);

    // Animal
    Animal m_animal_entity[128];
    bool canMoveTo(int x, int y);
    void tryUpdateAnimalTile(int index);
    void updateAnimalTile(int index);

    const Animal& getAnimalAt(int x, int y) const;

private:
    int m_width;
    int m_height;

    std::vector<std::vector<Tile>> m_tiles;
};

#endif // TILEMAP_H
