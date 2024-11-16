#ifndef TILEMAP_H
#define TILEMAP_H

#include <iostream>
#include <vector>
#include <random>
#include <functional>
#include <queue>
#include <unordered_map>
#include "entity.h"
#include <algorithm>


class Tile {
public:
    enum Type { EMPTY, TREE, WALL, DOOR, ANIMAL, PLAYER, CUTED_TREE, STORE };  

    Tile(Type type = EMPTY);

    Type getType() const;
    void setType(Type type);

private:
    Type m_type;
};

class Map {
public:
    Map(int width, int height);

    void placeRandomTrees(int count);
    void setStoreArea(int startX, int startY, int endX, int endY);
    void setCutArea(int startX, int startY, int endX, int endY);
    int countAdjacentTypes(int x, int y, Tile::Type type) const;

    // Animal & Player
    Animal m_animal_entity[32];
    Player m_player_entity[10];

    bool canMoveTo(int x, int y);
    void tryUpdateAnimalTile(int index);
    void updateAnimalTile(int index);
    bool isPositionOccupied(int x, int y) const;
    const Animal& getAnimalAt(int x, int y) const;
    std::vector<std::pair<int, int>> findPathToTarget(int startX, int startY, Tile::Type targetType, std::function<void(int, int)> onTargetFound);

    // DEBUG
    void printMapTileType() const;
    
    Tile getTile(int x, int y) const;
    void setTile(int x, int y, Tile::Type type);

private:
    int m_width;
    int m_height;

    std::vector<std::vector<Tile>> m_tiles;
};

#endif // TILEMAP_H
