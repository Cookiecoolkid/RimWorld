#include <iostream>
#include "map.h"
#include "config.h"
#include <queue>
#include <vector>
#include <unordered_map>
#include <functional>

Tile::Tile(Type type)
    : m_type(type) {}

Tile::Type Tile::getType() const {
    return m_type;
}

void Tile::setType(Type type) {
    m_type = type;
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
        m_animal_entity[i].moveProgress = 0;
        m_animal_entity[i].isMoving = false;
        m_animal_entity[i].direction = Entity::LEFT;
        m_animal_entity[i].img_direction = Entity::LEFT;

        setTile(m_animal_entity[i].x, m_animal_entity[i].y, Tile::ANIMAL);
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

        setTile(m_player_entity[i].x, m_player_entity[i].y, Tile::PLAYER);
    }
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

bool Map::isPositionOccupied(int x, int y) const {
    if (getTile(x, y).getType() != Tile::EMPTY) {
        return true;
    }
    return false;
}

void Map::setStoreArea(int startX, int startY, int endX, int endY) {
    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            if (getTile(x, y).getType() == Tile::EMPTY) {
                setTile(x, y, Tile::STORE);
            }
        }
    }
}

void Map::setCutArea(int startX, int startY, int endX, int endY) {
    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            if (getTile(x, y).getType() == Tile::TREE) {
                setTile(x, y, Tile::CUTED_TREE);
            }
        }
    }
}

int Map::countAdjacentTypes(int x, int y, Tile::Type type) const {
    int numStore = 0;
    std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    for (const auto& dir : directions) {
        int newX = x + dir.first;
        int newY = y + dir.second;
        if (newX >= 0 && newX < Config::MAP_WIDTH && newY >= 0 && newY < Config::MAP_HEIGHT) {
            if (getTile(newX, newY).getType() == type) {
                numStore++;
            }
        }
    }
    return numStore;
}

// 通用的寻路函数
std::vector<std::pair<int, int>> Map::findPathToTarget(int startX, int startY, Tile::Type targetType, std::function<void(int, int)> onTargetFound) {
    std::queue<std::pair<int, int>> q;
    std::unordered_map<int, std::pair<int, int>> parent;
    std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    q.push({startX, startY});
    parent[startY * m_width + startX] = {-1, -1};

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        for (const auto& dir : directions) {
            int newX = x + dir.first;
            int newY = y + dir.second;

            if (newX >= 0 && newX < m_width && newY >= 0 && newY < m_height &&
                parent.find(newY * m_width + newX) == parent.end()) {
                parent[newY * m_width + newX] = {x, y};
                q.push({newX, newY});

                if (getTile(newX, newY).getType() == targetType) {
                    std::vector<std::pair<int, int>> path;
                    for (auto p = std::make_pair(newX, newY); p != std::make_pair(-1, -1); p = parent[p.second * m_width + p.first]) {
                        path.push_back(p);
                    }
                    std::reverse(path.begin(), path.end());
                    onTargetFound(newX, newY);
                    return path;
                }
            }
        }
    }

    return {};
}


void Map::printMapTileType() const {
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            Tile::Type type = getTile(x, y).getType();
            if (type == Tile::EMPTY) {
                std::cout << " ";
            } else if (type == Tile::TREE) {
                std::cout << " ";
            } else if (type == Tile::ANIMAL) {
                std::cout << "A";
            } else if (type == Tile::PLAYER) {
                std::cout << "P";
            } else if (type == Tile::STORE) {
                std::cout << "S";
            }
            
        }
        std::cout << std::endl;
    }

    std::cout << "----------------------------------------------------------------------------" << std::endl;
}