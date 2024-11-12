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

bool Game::init() {
    // SDL 初始化
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // IMG 初始化
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Window 初始化
    if (!m_window.init()) {
        return false;
    }

    // Renderer 初始化
    m_renderer = Renderer(m_window.getSDLWindow());
    if (!m_renderer.init()) {
        return false;
    }

    // EventManager 初始化
    m_eventManager.registerCallbacks();

    // load image 
    if (!init_load_image()) {
        return false;
    }

    // game settings
    m_map.placeRandomTrees(Config::INIT_TREE_COUNT);

    m_isRunning = true;

    // 设置地图渲染起始位置
    m_mapStartX = 0;
    m_mapStartY = 0;
    return true;
}

bool Game::init_load_image() {
    m_background = Image(Config::IMAGE_BACKGROUND_PATH);
    if (!m_background.load(m_renderer.getSDLRenderer())) {
        return false;
    }

    m_tree = Image(Config::IMAGE_TREE_PATH);
    if (!m_tree.load(m_renderer.getSDLRenderer())) {
        return false;
    }

    m_cuted_tree = Image(Config::IMAGE_CUTED_TREE_PATH);
    if (!m_cuted_tree.load(m_renderer.getSDLRenderer())) {
        return false;
    }

    return true;
}

void Game::run() {
    if (!init()) {
        std::cerr << "Game initialization failed" << std::endl;
    }

    while (m_isRunning) {
        // handle events
        m_eventManager.handleEvents();

        m_renderer.clear();

        // update render
        m_renderer.renderCopyImage(m_background, 0, 0, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);

        // render map
        for (int x = m_mapStartX; x < Config::MAP_RENDER_WIDTH; ++x) {
            for (int y = m_mapStartY; y < Config::MAP_RENDER_HEIGHT; ++y) {
                
                assert(m_mapStartX + Config::MAP_RENDER_WIDTH < Config::MAP_WIDTH);
                assert(m_mapStartY + Config::MAP_RENDER_WIDTH < Config::MAP_HEIGHT);

                Tile tile = m_map.getTile(x, y);
                if (tile.getType() == Tile::TREE) {
                    m_renderer.renderCopyImage(m_tree, x * Config::MAP_UNIT_SIZE, y * Config::MAP_UNIT_SIZE, Config::MAP_UNIT_SIZE, Config::MAP_UNIT_SIZE);
                }
            }
        }

        m_renderer.present();

        SDL_Delay(16);
    }
}