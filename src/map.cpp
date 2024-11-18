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
    if (type & (TREE | WALL | ANIMAL | PLAYER | CUTED_TREE)) {
        m_type |= OCCUPIED;
    }
}

void Tile::removeType(Type type) {
    m_type &= ~type;
    if (!(m_type & (TREE | WALL | ANIMAL | PLAYER | CUTED_TREE))) {
        m_type &= ~OCCUPIED;
    }
}

bool Tile::hasType(Type type) const {
    return (m_type & type) != 0;
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

// 这里需要注意 Tile 是和 Animal 的 x, y 属性对应，因此只有更新 x,y 的时候才可以更新 Tile 以保持同步
void Map::tryUpdateAnimalTile(int index) {
    if (m_animal_entity[index].isStepping) {
        m_animal_entity[index].stepProgress++;
        if (m_animal_entity[index].stepProgress >= Config::ANIMAL_MOVE_FRAMES) {
            // 更新 x,y 的同时更新 Tile
            // 但由于 updateAnimalTile 的实现是根据 x,y 还没有更新的值来更新 Tile 的，因此要先调用 updateAnimalTile
            updateAnimalTile(index);
            m_animal_entity[index].updatePosition();
        }
    }
}

void Map::updateAnimalTile(int index) {
    removeTileType(m_animal_entity[index].x, m_animal_entity[index].y, Tile::ANIMAL);
    addTileType(m_animal_entity[index].targetX, m_animal_entity[index].targetY, Tile::ANIMAL);
}


void Map::updatePlayerTile(int index) {
    removeTileType(m_player_entity[index].x, m_player_entity[index].y, Tile::PLAYER);
    addTileType(m_player_entity[index].targetX, m_player_entity[index].targetY, Tile::PLAYER);
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

void Map::playerActionReset(int index) {
    Player& player = m_player_entity[index];
    player.isFree = true;
    player.isMoving = false;
    player.isCutting = false;
    player.isStoring = false;
    player.isPickingUp = false;
    player.path.clear();
}

Player::Action Map::findPlayerAction(int index) {
    // TODO
    if (hasReachableCutTreeInMap()) {
        return Player::Action::MOVE;
    }
    return Player::Action::NONE;
}

void Map::startPlayerAction(int index, Player::Action action) {
    Player& player = m_player_entity[index];
    switch (action) {
        case Player::Action::MOVE: {
            player.path = findPathToTarget(player.x, player.y, Tile::CUTED_TREE);

            if (!player.path.empty()) {
                player.isFree = false;
                player.isMoving = true;
                auto [targetX, targetY] = player.path.back();
                addTileType(targetX, targetY, Tile::TARGETED);
                player.path.erase(player.path.begin());
            } else {
                // STILL FREE, DO NOTHING
            }
            break;
        }
        case Player::Action::CUT:
            // TODO
            break;
        case Player::Action::STORE:
            // TODO
            break;
        case Player::Action::PICKUP:
            // TODO
            break;
        default:
            break;
    }
}
// TIP 设置 FREE 为 true 的同时一定要取消 TARGETED 标记 否则会影响其他 Player 的寻路
void Map::tryProcessPlayerAction(int index) {
    Player& player = m_player_entity[index];

    if (player.isMoving) {
        if (player.isStepping) {
            player.stepProgress++;
            if (player.stepProgress >= Config::PLAYER_MOVE_FRAMES) {
                // 更新 x,y 的同时更新 Tile
                // 但由于 updatePlayerTile 的实现是根据 x,y 还没有更新的值来更新 Tile 的，因此要先调用 updatePlayerTile
                updatePlayerTile(index);
                player.updatePosition();
            }
        } else {
            assert(!player.path.empty());

            auto [nextX, nextY] = player.path.front();
            if (player.path.size() == 1) {
                // 如果只剩下一个点，说明到达终点
                auto [targetX, targetY] = player.path.back();
                removeTileType(targetX, targetY, Tile::TARGETED);
                
                playerActionReset(index);
                return;
            }

            // 检查下一步是否会与其他玩家或动物发生碰撞
            if (!isPositionOccupied(nextX, nextY)) {
                addTileType(nextX, nextY, Tile::OCCUPIED);
                player.path.erase(player.path.begin());
                player.startStep(nextX, nextY);
                player.updatePlayerDirection(nextX, nextY);
            } else {
                // 如果下一步会发生碰撞，则重新计算路径或暂停移动 同时需要删除 TARGETED 标记
                assert(!player.path.empty());
                auto [targetX, targetY] = player.path.back();
                removeTileType(targetX, targetY, Tile::TARGETED);

                playerActionReset(index);
            }
        }
    } else if (player.isCutting) {
        // TODO
    } else if (player.isStoring) {
        // TODO
    } else if (player.isPickingUp) {
        // TODO
    }
}

// 通用的寻路函数
std::vector<std::pair<int, int>> Map::findPathToTarget(int startX, int startY, Tile::Type targetType) {
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

                bool isTarget = getTile(newX, newY).hasType(targetType) && 
                                !getTile(newX, newY).hasType(Tile::TARGETED);

                if (isPositionOccupied(newX, newY) && !isTarget) {
                    // 如果新位置被占据，则跳过该位置
                    continue;
                }

                parent[newY * m_width + newX] = {x, y};
                q.push({newX, newY});

                if (isTarget) {
                    std::vector<std::pair<int, int>> path;
                    for (auto p = std::make_pair(newX, newY); p != std::make_pair(-1, -1); p = parent[p.second * m_width + p.first]) {
                        path.push_back(p);
                    }
                    std::reverse(path.begin(), path.end());

                    DEBUG("Path found to (%d, %d)\n", newX, newY);
                    return path;
                }
            }
        }
    }

    DEBUG("No path found\n");
    return {};
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