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
    enum Type {
        EMPTY = 0,
        TREE = 1 << 0,
        WALL = 1 << 1,
        DOOR = 1 << 2,
        ANIMAL = 1 << 3,
        PLAYER = 1 << 4,
        CUTED_TREE = 1 << 5,
        STORE = 1 << 6
    };

    Tile(Type type = EMPTY);

    Type getType() const;
    void setType(Type type);
    void addType(Type type);
    void removeType(Type type);
    bool hasType(Type type) const;

private:
    int m_type; // 使用位掩码来表示不同的 Tile 类型
};


class Map {
public:
    Map(int width, int height);

    void placeRandomTrees(int count);
    void setStoreArea(int startX, int startY, int endX, int endY);
    void setCutArea(int startX, int startY, int endX, int endY);
    bool isAdjacentTypesReachCount(int x, int y, Tile::Type type, int count) const;

    // Animal & Player
    Animal m_animal_entity[32];
    Player m_player_entity[10];

    bool canMoveTo(int x, int y);
    void tryUpdateAnimalTile(int index);
    void updateAnimalTile(int index);
    bool isPositionOccupied(int x, int y) const;
    const Animal& getAnimalAt(int x, int y) const;
    std::vector<std::pair<int, int>> findPathToTarget(int startX, int startY, 
                                                    Tile::Type targetType, 
                                                    std::function<bool(int, int)> condition, 
                                                    std::function<void(int, int)> onTargetFound);

    // DEBUG
    void printMapTileType() const;
    
    Tile getTile(int x, int y) const;
    void addTileType(int x, int y, Tile::Type type);
    void removeTileType(int x, int y, Tile::Type type);

private:
    int m_width;
    int m_height;

    std::vector<std::vector<Tile>> m_tiles;
};

#endif // TILEMAP_H
