#include "game.h"
#include "config.h"
#include <utility>

void Game::updateMapPosition(int dx, int dy) {
    if (m_isMapMoving) {
        return;
    }

    // 边界情况处理
    if (m_mapStartX - Config::MAP_MOVE_SPEED < 0 && dx < 0) {
        dx = m_mapStartX;
    } else if (m_mapStartX + Config::MAP_RENDER_WIDTH + Config::MAP_MOVE_SPEED > Config::MAP_WIDTH && dx > 0) {
        dx = Config::MAP_WIDTH - m_mapStartX - Config::MAP_RENDER_WIDTH;
    }

    if (m_mapStartY - Config::MAP_MOVE_SPEED < 0 && dy < 0) {
        dy = m_mapStartY;
    } else if (m_mapStartY + Config::MAP_RENDER_HEIGHT + Config::MAP_MOVE_SPEED > Config::MAP_HEIGHT && dy > 0) {
        dy = Config::MAP_HEIGHT - m_mapStartY - Config::MAP_RENDER_HEIGHT;
    }
    
    // 设置新的目标偏移量
    m_targetOffsetX = -dx;
    m_targetOffsetY = -dy;
    m_currentFrame = 0;
    m_isMapMoving = true;
}

void Game::movingMap() {
    if (m_isMapMoving) {
        m_currentFrame++;
        if (m_currentFrame >= Config::MOVE_FRAMES) {
            m_isMapMoving = false;
            // targetOffsetX 以及 targetOffsetY 是负数
            m_mapStartX = std::max(0, std::min(m_mapStartX - m_targetOffsetX / Config::MAP_UNIT_SIZE,
                                                    Config::MAP_WIDTH - Config::MAP_RENDER_WIDTH));
            m_mapStartY = std::max(0, std::min(m_mapStartY - m_targetOffsetY / Config::MAP_UNIT_SIZE,
                                                    Config::MAP_HEIGHT - Config::MAP_RENDER_HEIGHT));
            m_renderer.setMapMovingOffset(0, 0);
        } else {
            m_renderer.setMapMovingOffset((m_targetOffsetX / Config::MOVE_FRAMES) * m_currentFrame, 
                                    (m_targetOffsetY / Config::MOVE_FRAMES) * m_currentFrame);
        }
    }
}

void Game::updateGameState() {
    // 移动地图
    movingMap();
    // 更新动物状态
    for (int i = 0; i < Config::ANIMAL_NUMBERS; ++i) {
        Animal& animal = m_map.m_animal_entity[i];
        if (!animal.isMoving) {
            // 随机选择一个方向 但可能不移动
            std::pair<int, int> move = animal.action();

            if (move.first == -1 && move.second == -1) continue;

            if (m_map.canMoveTo(move.first, move.second)) {
                m_map.addTileType(move.first, move.second, Tile::OCCUPIED);
                animal.startMove(move.first, move.second);
            }
        }
        // 更新动物的 x,y 和 Tile 要保持同步
        m_map.tryUpdateAnimalTile(i);
    }


    // TIP 设置 FREE 为 true 的同时一定要取消 TARGETED 标记 否则会影响其他 Player 的寻路
    for (int i = 0; i < Config::PLAYER_NUMBERS; ++i) {
        Player& player = m_map.m_player_entity[i];

        // 如果当前没有执行动作 且 遍历地图发现有 CUTED_TREE 且 CUTED_TREE 周围必然有没有被占据的位置
        if (player.isFree && m_map.hasReachableCutTreeInMap()) {
            // 此处计算量较大，不能每次都计算
            player.path = m_map.findPathToTarget(player.x, player.y, Tile::CUTED_TREE);
            
            // 寻路可能会失败
            if (player.path.empty()) {
                continue;
            }

            player.isFree = false;
            // 找到 起点->终点 的路径之后取出终点 加上 TARGETED 标记
            auto [targetX, targetY] = player.path.back();
            m_map.addTileType(targetX, targetY, Tile::TARGETED);

            // TODO 暂时去除起点
            player.path.erase(player.path.begin());
        }

        // 移动玩家
        if (!player.isMoving && !player.path.empty()) {
            auto [nextX, nextY] = player.path.front();

            if (player.path.size() == 1) {
                // 如果只剩下一个点，说明到达终点
                player.path.clear();
                // TODO 进行下一个动作

                continue;
            }

            // 检查下一步是否会与其他玩家或动物发生碰撞
            if (!m_map.isPositionOccupied(nextX, nextY)) {
                m_map.addTileType(nextX, nextY, Tile::OCCUPIED);
                player.path.erase(player.path.begin());
                player.startMove(nextX, nextY);
                player.updatePlayerDirection(nextX, nextY);
            } else {
                // 如果下一步会发生碰撞，则重新计算路径或暂停移动 同时需要删除 TARGETED 标记
                auto [targetX, targetY] = player.path.back();
                m_map.removeTileType(targetX, targetY, Tile::TARGETED);

                player.path.clear();
                player.isFree = true;
            }
        }

        // 更新玩家的 x,y 和 Tile 要保持同步
        m_map.tryUpdatePlayerTile(i);
    }
}