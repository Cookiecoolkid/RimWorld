#include "game.h"
#include "config.h"

void Game::updateMapPosition(int dx, int dy) {
    if (m_isMoving) {
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
    m_isMoving = true;
}

void Game::movingMap() {
    if (m_isMoving) {
        m_currentFrame++;
        if (m_currentFrame >= Config::MOVE_FRAMES) {
            m_isMoving = false;
            // targetOffsetX 以及 targetOffsetY 是负数
            m_mapStartX = std::max(0, std::min(m_mapStartX - m_targetOffsetX / Config::MAP_UNIT_SIZE,
                                                    Config::MAP_WIDTH - Config::MAP_RENDER_WIDTH));
            m_mapStartY = std::max(0, std::min(m_mapStartY - m_targetOffsetY / Config::MAP_UNIT_SIZE,
                                                    Config::MAP_HEIGHT - Config::MAP_RENDER_HEIGHT));
            m_renderer.setOffset(0, 0);
        } else {
            m_renderer.setOffset((m_targetOffsetX / Config::MOVE_FRAMES) * m_currentFrame, 
                                    (m_targetOffsetY / Config::MOVE_FRAMES) * m_currentFrame);
        }
    }
}