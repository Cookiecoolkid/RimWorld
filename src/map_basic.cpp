#include <iostream>
#include "map.h"
#include "config.h"
#include <queue>
#include <vector>
#include <unordered_map>
#include <functional>

Tile::Tile(Type type): m_type(type) {}

Tile::Type Tile::getType() const {
    return static_cast<Type>(m_type);
}

void Tile::addType(Type type) {
    m_type |= type;
    if (type & (TREE | WALL | ANIMAL | PLAYER | CUTED_TREE | WOOD)) {
        m_type |= OCCUPIED;
    }
}

void Tile::removeType(Type type) {
    m_type &= ~type;
    if (!(m_type & (TREE | WALL | ANIMAL | PLAYER | CUTED_TREE | WOOD))) {
        m_type &= ~OCCUPIED;
    }
}

bool Tile::hasType(Type type) const {
    return (m_type & type) != 0;
}

int Tile::getWoodCount() const {
    return m_wood_count;
}

void Tile::setWoodCount(int count) {
    m_wood_count = count;
}

void Tile::decreaseWoodCount(int amount) {
    m_wood_count -= amount;
    if (m_wood_count < 0) {
        m_wood_count = 0;
    }
}


Map::Map(int width, int height) : m_width(width), m_height(height) {
    srand(time(nullptr));
    
    m_tiles.resize(m_width);
    for (int x = 0; x < m_width; ++x) {
        m_tiles[x].resize(m_height);
    }

    // 初始化动物的位置
    for (int i = 0; i < Config::ANIMAL_NUMBERS; ++i) {
        int x, y;
        do {
            x = rand() % m_width;
            y = rand() % m_height;
        } while (isPositionOccupied(x, y));
        m_animal_entity[i].x = x;
        m_animal_entity[i].y = y;
        m_animal_entity[i].targetX = x;
        m_animal_entity[i].targetY = y;
        m_animal_entity[i].stepProgress = 0;
        m_animal_entity[i].isStepping = false;
        m_animal_entity[i].direction = Entity::LEFT;
        m_animal_entity[i].img_direction = Entity::LEFT;

        addTileType(m_animal_entity[i].x, m_animal_entity[i].y, Tile::ANIMAL);
    }

    // 初始化玩家的位置，确保不与任何动物重叠
    for (int i = 0; i < Config::PLAYER_NUMBERS; ++i) {
        int x, y;
        do {
            x = rand() % m_width;
            y = rand() % m_height;
        } while (isPositionOccupied(x, y));
        m_player_entity[i].x = x;
        m_player_entity[i].y = y;
        m_player_entity[i].direction = Entity::DOWN;
        m_player_entity[i].isFree = true;
        m_player_entity[i].stepProgress = 0;
        m_player_entity[i].isStepping = false;

        addTileType(m_player_entity[i].x, m_player_entity[i].y, Tile::PLAYER);
    }
}

Tile Map::getTile(int x, int y) const {
    return m_tiles[x][y];
}

void Map::addTileType(int x, int y, Tile::Type type) {
    m_tiles[x][y].addType(type);
}

void Map::removeTileType(int x, int y, Tile::Type type) {
    m_tiles[x][y].removeType(type);
}

void Map::placeRandomTrees(int count) {
    for (int i = 0; i < count; ++i) {
        int x = std::rand() % m_width;
        int y = std::rand() % m_height;

        if (isPositionOccupied(x, y)) {
            i--;
            continue;
        }

        addTileType(x, y, Tile::TREE);
    }
}

bool Map::canMoveTo(int x, int y) {
    if (x < 0 || x >= Config::MAP_WIDTH || y < 0 || y >= Config::MAP_HEIGHT) {
        return false;
    }
    return !isPositionOccupied(x, y);
}

bool Map::isAdjacentTypesReachCount(int x, int y, Tile::Type type, int count) const {
    int numStore = 0;
    std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    for (const auto& dir : directions) {
        int newX = x + dir.first;
        int newY = y + dir.second;
        if (newX >= 0 && newX < Config::MAP_WIDTH && newY >= 0 && newY < Config::MAP_HEIGHT) {
            if (getTile(newX, newY).hasType(type)) {
                numStore++;
            }
        }
    }
    return numStore >= count;
}

std::pair<int, int> Map::getAdjacentNonTargetedPosition(int x, int y, Tile::Type targetType) {
    std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    for (const auto& dir : directions) {
        int newX = x + dir.first;
        int newY = y + dir.second;
        if (newX >= 0 && newX < Config::MAP_WIDTH && newY >= 0 && newY < Config::MAP_HEIGHT) {
            if (getTile(newX, newY).hasType(targetType) && !getTile(newX, newY).hasType(Tile::TARGETED)) {
                return std::make_pair(newX, newY);
            }
        }
    }
    return std::make_pair(-1, -1);
}

bool Map::isPositionOccupied(int x, int y) const {
    Tile tile = getTile(x, y);
    return tile.hasType(Tile::OCCUPIED);
}

void Map::setStoreArea(int startX, int startY, int endX, int endY) {
    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            Tile tile = getTile(x, y);
            if (!tile.hasType(Tile::TREE) && !tile.hasType(Tile::CUTED_TREE) && 
                !tile.hasType(Tile::WALL)) {
                addTileType(x, y, Tile::STORE);
            }
        }
    }
}

void Map::setCutArea(int startX, int startY, int endX, int endY) {
    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            Tile tile = getTile(x, y);
            if (tile.hasType(Tile::TREE)) {
                addTileType(x, y, Tile::CUTED_TREE);
                removeTileType(x, y, Tile::TREE);
            }
        }
    }
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

const Player& Map::getPlayerAt(int x, int y) const {
    // 这里的 x, y 与 tile 的 x, y 对应
    for (const auto& player : m_player_entity) {
        if (player.x == x && player.y == y) {
            return player;
        }
    }
    throw std::runtime_error("Player not found at the given position");
}

void Map::playerActionReset(int index, int targetX, int targetY) {
    Player& player = m_player_entity[index];
    player.isFree = true;
    player.isMoving = false;
    player.isCutting = false;
    player.isStoring = false;
    player.isPickingUp = false;
    player.path.clear();

    if (targetX != -1 && targetY != -1) {
        removeTileType(targetX, targetY, Tile::TARGETED);
    }
}

bool Map::hasReachableCutTreeInMap() const {
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            Tile tile = getTile(x, y);
            if (tile.hasType(Tile::CUTED_TREE) && !tile.hasType(Tile::TARGETED)) {
                if (!isAdjacentTypesReachCount(x, y, Tile::OCCUPIED, 4)) {
                    DEBUG("Found CUTED_TREE at (%d, %d)\n", x, y);
                    return true;
                }
            }
        }
    }
    return false;
}

void Map::printMapTileType() const {
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            Tile tile = getTile(x, y);
            if (tile.hasType(Tile::ANIMAL)) {
                std::cout << " ";
            } else if (tile.hasType(Tile::PLAYER)) {
                std::cout << "P";
            } else if (tile.hasType(Tile::TARGETED)) {
                std::cout << "T";
            }else if (tile.hasType(Tile::TREE)) {
                std::cout << " ";
            } else if (tile.hasType(Tile::CUTED_TREE)) {
                std::cout << " ";
            } else if (tile.hasType(Tile::WALL)) {
                std::cout << " ";
            } else {
                std::cout << " ";
            }
            
        }
        std::cout << std::endl;
    }

    std::cout << "---------------------------------------------------------------------------------" << std::endl;
}