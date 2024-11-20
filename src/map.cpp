#include <iostream>
#include "map.h"
#include "config.h"
#include <queue>
#include <vector>
#include <unordered_map>
#include <functional>

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

Player::Action Map::findPlayerAction(int index) {
    // TODO
    Player& player = m_player_entity[index];

    if (player.carryingWood > 0 && getAdjacentNotFullStorePosition(player.x, player.y).first != -1) {
        return Player::Action::STORE;
    }
    if (player.carryingWood > 0 && hasReachableTypeInMap(Tile::STORE)) {
        return Player::Action::MOVE_STORE;
    }
    if (player.carryingWood > 0 && !hasReachableTypeInMap(Tile::STORE)) {
        if (getAdjacentNonOccupiedPosition(player.x, player.y).first != -1) {
            return Player::Action::DROP;
        } else {
            return Player::Action::MOVE_DROP;
        }
    }
    if (player.carryingWood < Config::PLAYER_MAX_CAPACITY && 
        hasReachableTypeInMap(Tile::STORE) && 
        hasReachableTypeInMap(Tile::WOOD)) {
            if (isAdjacentTypesReachCount(player.x, player.y, Tile::WOOD, 1)) {
                return Player::Action::PICKUP;
            } else {
                return Player::Action::MOVE_PICKUP;
            }
    }
    if (isAdjacentTypesReachCount(player.x, player.y, Tile::CUTED_TREE, 1)) {
        return Player::Action::CUT;
    }
    if (hasReachableTypeInMap(Tile::CUTED_TREE)) {
        return Player::Action::MOVE_CUT;
    }
    return Player::Action::NONE;
}

void Map::startPlayerAction(int index, Player::Action action) {
    Player& player = m_player_entity[index];
    switch (action) {
        case Player::Action::MOVE_CUT: {
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
        case Player::Action::CUT: {
            player.isFree = false;
            player.isCutting = true;
            player.cutProgress = 0;
            std::pair<int, int> optionTarget = getAdjacentNonTargetedPosition(player.x, player.y, Tile::CUTED_TREE);
            if (optionTarget.first != -1 && optionTarget.second != -1) {
                player.optionTargetX = optionTarget.first;
                player.optionTargetY = optionTarget.second;
                addTileType(optionTarget.first, optionTarget.second, Tile::TARGETED);
            } else {
                playerActionReset(index, optionTarget.first, optionTarget.second);
            }
            break;
        }
        case Player::Action::MOVE_STORE: {
            player.path = findPathToTarget(player.x, player.y, Tile::STORE, [](const Tile& tile) {
                return tile.getWoodCount() < Config::STORE_MAX_CAPACITY;
            });

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
        case Player::Action::STORE: {
            player.isFree = false;
            player.isStoring = true;
            std::pair<int, int> optionTarget = getAdjacentNotFullStorePosition(player.x, player.y);
            if (optionTarget.first != -1 && optionTarget.second != -1) {
                player.optionTargetX = optionTarget.first;
                player.optionTargetY = optionTarget.second;
                addTileType(optionTarget.first, optionTarget.second, Tile::TARGETED);
            } else {
                playerActionReset(index, optionTarget.first, optionTarget.second);
            }
            break;
        }
        case Player::Action::MOVE_PICKUP: {
            player.path = findPathToTarget(player.x, player.y, Tile::WOOD);

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
        case Player::Action::PICKUP: {
            player.isFree = false;
            player.isPickingUp = true;
            std::pair<int, int> optionTarget = getAdjacentNonTargetedPosition(player.x, player.y, Tile::WOOD);
            if (optionTarget.first != -1 && optionTarget.second != -1) {
                player.optionTargetX = optionTarget.first;
                player.optionTargetY = optionTarget.second;

                addTileType(optionTarget.first, optionTarget.second, Tile::TARGETED);
            } else {
                playerActionReset(index, optionTarget.first, optionTarget.second);
            }
            break;
        }
        case Player::Action::MOVE_DROP: {
            player.path = findPathToTarget(player.x, player.y, Tile::EMPTY, [](const Tile& tile) {
                return !tile.hasType(Tile::OCCUPIED);
            });

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
        case Player::Action::DROP: {
            player.isFree = false;
            player.isDropping = true;
            std::pair<int, int> optionTarget = getAdjacentNonOccupiedPosition(player.x, player.y);
            if (optionTarget.first != -1 && optionTarget.second != -1) {
                player.optionTargetX = optionTarget.first;
                player.optionTargetY = optionTarget.second;
                addTileType(optionTarget.first, optionTarget.second, Tile::TARGETED);
            } else {
                playerActionReset(index, optionTarget.first, optionTarget.second);
            }
            break;
        }
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
                // removeTileType(targetX, targetY, Tile::TARGETED); // 此处去除 TARGET 由 ActionReset 负责
                playerActionReset(index, targetX, targetY);
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
                
                // 此处去除 TARGET 由 ActionReset 负责
                playerActionReset(index, targetX, targetY);
            }
        }
    } else if (player.isCutting) {
        player.cutProgress++;
        if (player.cutProgress >= Config::CUT_TREE_FRAMES) {
            int optionTargetX = player.optionTargetX;
            int optionTargetY = player.optionTargetY;
            Tile& tile = m_tiles[optionTargetX][optionTargetY];
            assert(tile.hasType(Tile::CUTED_TREE));

            tile.removeType(Tile::CUTED_TREE);
            tile.addType(Tile::WOOD);
            tile.setWoodCount(Config::WOOD_DROP_COUNT);

            // 此处去除 TARGET 由 ActionReset 负责
            playerActionReset(index, optionTargetX, optionTargetY);
        }
    } else if (player.isStoring) {
        int optionTargetX = player.optionTargetX;
        int optionTargetY = player.optionTargetY;
        Tile& tile = m_tiles[optionTargetX][optionTargetY];
        int woodToStore = std::min(Config::STORE_MAX_CAPACITY - tile.getWoodCount(), player.carryingWood);
        player.dropWood(woodToStore);
        tile.increaseWoodCount(woodToStore);

        playerActionReset(index, optionTargetX, optionTargetY);
    } else if (player.isPickingUp) {
        int optionTargetX = player.optionTargetX;
        int optionTargetY = player.optionTargetY;
        Tile& tile = m_tiles[optionTargetX][optionTargetY];
        int woodToPickup = std::min(Config::PLAYER_MAX_CAPACITY - player.carryingWood, tile.getWoodCount());
        player.pickupWood(woodToPickup);
        tile.decreaseWoodCount(woodToPickup);

        if (tile.getWoodCount() == 0) {
            tile.removeType(Tile::WOOD);
        }

        playerActionReset(index, optionTargetX, optionTargetY);
    } else if (player.isDropping) {
        int optionTargetX = player.optionTargetX;
        int optionTargetY = player.optionTargetY;
        Tile& tile = m_tiles[optionTargetX][optionTargetY];
        int woodToDrop = player.carryingWood;
        player.dropWood(woodToDrop);
        tile.increaseWoodCount(woodToDrop);
        tile.addType(Tile::WOOD);

        playerActionReset(index, optionTargetX, optionTargetY);
    }
}

// 通用的寻路函数
std::vector<std::pair<int, int>> Map::findPathToTarget(int startX, int startY, Tile::Type targetType, 
                                 std::function<bool(const Tile&)> extraCondition) {
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
                                !getTile(newX, newY).hasType(Tile::TARGETED) &&
                                (!extraCondition || extraCondition(getTile(newX, newY)));

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
                    return path;
                }
            }
        }
    }
    return {};
}