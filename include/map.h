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
#include <nlohmann/json.hpp>


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
        STORE = 1 << 6,
        OCCUPIED = 1 << 7,
        TARGETED = 1 << 8,
        WOOD = 1 << 9,
        BGWALL = 1 << 10,
    };

    Tile(Type type = EMPTY);

    Type getType() const;
    void addType(Type type);
    void removeType(Type type);
    bool hasType(Type type) const;

    void setWoodCount(int count);
    int getWoodCount() const;
    void decreaseWoodCount(int amount);
    void increaseWoodCount(int amount);

    friend void to_json(nlohmann::json& j, const Tile& t);
    friend void from_json(const nlohmann::json& j, Tile& t);

private:
    int m_type; // 使用位掩码来表示不同的 Tile 类型
    int m_wood_count = 0;
};


class Map {
public:
    Map(int width, int height);

    void placeRandomTrees(int count);
    void setStoreArea(int startX, int startY, int endX, int endY);
    void setCutArea(int startX, int startY, int endX, int endY);
    void setBGWall(int x, int y);
    bool isAdjacentTypesReachCount(int x, int y, Tile::Type type, int count) const;

    // Animal & Player
    Animal m_animal_entity[400];
    Player m_player_entity[16];

    bool canMoveTo(int x, int y);
    
    void tryUpdateAnimalTile(int index);
    void updateAnimalTile(int index);
    void updatePlayerTile(int index);
    Player::Action findPlayerAction(int index);
    void startPlayerAction(int index, Player::Action action);
    void tryProcessPlayerAction(int index);
    void playerActionReset(int index, int targetX, int targetY);

    bool isPositionOccupied(int x, int y) const;
    bool hasReachableTypeInMap(Tile::Type type, int index,
                                    std::function<bool(const Tile&)> extraCondition = nullptr);
    bool isReachable(int startX, int startY, int goalX, int goalY) const;

    std::pair<int, int> getAdjacentNonTargetedPosition(int x, int y, Tile::Type targetType);
    std::pair<int, int> getAdjacentNonOccupiedPosition(int x, int y);
    std::pair<int, int> getAdjacentNotFullStorePosition(int x, int y);
    std::pair<int, int> getAdjacentAtLeastNumStorePosition(int x, int y, int num);

    const Animal& getAnimalAt(int x, int y) const;
    const Player& getPlayerAt(int x, int y) const;
    std::vector<std::pair<int, int>> findPathToTarget(int startX, int startY, Tile::Type targetType,
                                    std::function<bool(const Tile&)> extraCondition = nullptr);

    // DEBUG
    void printMapTileType() const;
    
    Tile getTile(int x, int y) const;
    void addTileType(int x, int y, Tile::Type type);
    void removeTileType(int x, int y, Tile::Type type);

    friend void to_json(nlohmann::json& j, const Map& m);
    friend void from_json(const nlohmann::json& j, Map& m);

private:
    int m_width;
    int m_height;

    std::vector<std::vector<Tile>> m_tiles;
};

#endif // TILEMAP_H
