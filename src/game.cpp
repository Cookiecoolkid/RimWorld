#include "game.h"
#include "config.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cassert>

Game::Game(const std::string& title, int width, int height)
    : m_window(title, width, height), m_renderer(nullptr), m_isRunning(false), m_background(""),
      m_tree(""), m_cuted_tree(""), m_map(Config::MAP_WIDTH, Config::MAP_HEIGHT) {}


Game::~Game() {
    DEBUG("Game Destructor\n");
    // Sequence is important
    IMG_Quit();
    SDL_Quit();
}

void Game::registerCallbacks() {
    m_eventManager.registerCallback(EventManager::QUIT, [this](const SDL_Event& event) { this->onQuit(event); });
    m_eventManager.registerCallback(EventManager::KEYDOWN, [this](const SDL_Event& event) { this->onKeyDown(event); });
    m_eventManager.registerCallback(EventManager::MOUSEBUTTONDOWN, [this](const SDL_Event& event) { this->onMouseButtonDown(event); });
}

void Game::updateMapPosition(int dx, int dy) {
    m_mapStartX = std::max(0, std::min(m_mapStartX + dx, Config::MAP_WIDTH - Config::MAP_RENDER_WIDTH - 1));
    m_mapStartY = std::max(0, std::min(m_mapStartY + dy, Config::MAP_HEIGHT - Config::MAP_RENDER_HEIGHT - 1));
}

void Game::run() {
    if (!init()) {
        std::cerr << "Game initialization failed" << std::endl;
        return;
    }

    // 注册事件回调
    registerCallbacks();

    while (m_isRunning) {
        // 处理事件
        m_eventManager.handleEvents();

        // 清屏
        m_renderer.clear();

        // 更新渲染
        m_renderer.renderCopyImage(m_background, 0, 0, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);

        // 渲染地图
        for (int y = m_mapStartY; y < m_mapStartY + Config::MAP_RENDER_HEIGHT && y < Config::MAP_HEIGHT; ++y) {
            for (int x = m_mapStartX; x < m_mapStartX + Config::MAP_RENDER_WIDTH && x < Config::MAP_WIDTH; ++x) {
                
                assert(m_mapStartX + Config::MAP_RENDER_WIDTH < Config::MAP_WIDTH);
                assert(m_mapStartY + Config::MAP_RENDER_HEIGHT < Config::MAP_HEIGHT);

                Tile tile = m_map.getTile(x, y);
                if (tile.getType() == Tile::TREE) {
                    m_renderer.renderCopyImage(m_tree, 
                                                (x - m_mapStartX) * Config::MAP_UNIT_SIZE, 
                                                (y - m_mapStartY) * Config::MAP_UNIT_SIZE, 
                                                Config::MAP_UNIT_SIZE, Config::MAP_UNIT_SIZE);
                
                }
            }
        }

        // 显示渲染内容
        m_renderer.present();

        // 控制帧率
        SDL_Delay(16);
    }
}