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

void Game::updateAnimalState() {
    for (int i = 0; i < Config::ANIMAL_NUMBERS; ++i) {
        Animal& animal = m_map.m_animal_entity[i];
        if (!animal.isStepping) {
            // 随机选择一个方向 但可能不移动
            std::pair<int, int> move = animal.action();

            if (move.first == -1 && move.second == -1) continue;

            if (m_map.canMoveTo(move.first, move.second)) {
                m_map.addTileType(move.first, move.second, Tile::OCCUPIED);
                animal.startStep(move.first, move.second);
            }
        }
        // 更新动物的 x,y 和 Tile 要保持同步
        m_map.tryUpdateAnimalTile(i);
    }
}

void Game::updatePlayerState() {
    for (int i = 0; i < Config::PLAYER_NUMBERS; ++i) {
        Player& player = m_map.m_player_entity[i];

        if (player.isFree) {
            Player::Action nextAction = m_map.findPlayerAction(i);
            m_map.startPlayerAction(i, nextAction);
        } else {
            // isFree = false 必然在执行某个动作, 推进玩家的动作
            m_map.tryProcessPlayerAction(i);
        }
    }
}

void Game::updateGameState() {
    // 移动地图
    movingMap();
    // 更新动物状态
    updateAnimalState();
    // 更新玩家状态
    updatePlayerState();
}