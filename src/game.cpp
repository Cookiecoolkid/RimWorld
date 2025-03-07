#include "game.h"
#include "config.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cassert>
#include "save_load.h"

Game::Game(const std::string& title, int width, int height)
    : m_window(title, width, height), m_renderer(nullptr), m_timer(), m_mode(MODE_START), m_isRunning(false), 
      m_background(""), m_tree(""), m_cuted_tree(""), m_map(Config::MAP_WIDTH, Config::MAP_HEIGHT) {}

Game::~Game() {
    DEBUG("Game Destructor\n");

    saveGame(*this, Config::SAVE_FILE_NAME);

    // Sequence is important
    IMG_Quit(); 
    TTF_Quit();
    SDL_Quit();
}

void Game::registerCallbacks() {
    m_eventManager.registerCallback(EventManager::QUIT, [this](const SDL_Event& event) { this->onQuit(event); });
    m_eventManager.registerCallback(EventManager::KEYDOWN, [this](const SDL_Event& event) { this->onKeyDown(event); });
    m_eventManager.registerCallback(EventManager::MOUSEBUTTONDOWN, [this](const SDL_Event& event) { this->onMouseButtonDown(event); });
    m_eventManager.registerCallback(EventManager::MOUSEBUTTONUP, [this](const SDL_Event& event) { this->onMouseButtonUp(event); });
}


void Game::run() {
    if (!init()) {
        std::cerr << "Game initialization failed" << std::endl;
        return;
    }

    // 注册事件回调
    registerCallbacks();

    while (m_isRunning) {
        // DEBUG 地图TILE类型 打开此打印的时候 渲染会很卡掉帧
        // m_map.printMapTileType();
        
        // 定时器计算时间
        // float deltaTime = m_timer.getDeltaTicks();

        // 处理事件
        m_eventManager.handleEvents();

        // 清屏
        m_renderer.clear();

        // 更新游戏状态
        updateGameState();

        // 更新渲染
        if (m_mode == MODE_START) {
            m_renderer.renderStartScreen(m_start_background);
        } else {
            // 渲染地图
            m_renderer.renderMap(m_map, m_mapStartX, m_mapStartY, m_background, m_tree, m_cuted_tree, 
                                 m_wood, m_wall, m_BGwall, m_animal_left, m_animal_right, 
                                 m_player_down, m_player_left, m_player_right, m_player_up);
        }
        // 显示渲染内容
        m_renderer.present();

        // 控制帧率
        SDL_Delay(16);
    }
}