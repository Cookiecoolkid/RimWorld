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
    if (isAdjacentTypesReachCount(player.x, player.y, Tile::CUTED_TREE, 1)) {
        return Player::Action::CUT;
    }
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
                playerActionReset(index);
            }
            break;
        }
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
        player.cutProgress++;
        if (player.cutProgress >= Config::CUT_TREE_FRAMES) {
            Tile& tile = m_tiles[player.optionTargetX][player.optionTargetY];
            assert(tile.hasType(Tile::CUTED_TREE));

            tile.removeType(Tile::CUTED_TREE);
            tile.addType(Tile::WOOD);
            tile.setWoodCount(Config::WOOD_DROP_COUNT);
        
            playerActionReset(index);
        }
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