#include "game.h"
#include "config.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

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
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        return false;
    }

    if (!m_window.init()) {
        return false;
    }

    m_renderer = Renderer(m_window.getSDLWindow());
    if (!m_renderer.init()) {
        return false;
    }

    // load image 
    if (!init_load_image()) {
        return false;
    }

    // game settings
    m_map.placeRandomTrees(Config::INIT_TREE_COUNT);

    m_isRunning = true;
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

    SDL_Event* event = new SDL_Event();

    while (m_isRunning) {
        
        while (SDL_PollEvent(event)) {
            if (event->type == SDL_QUIT) {
                m_isRunning = false;
            }
        }

        m_renderer.clear();

        // update render
        m_renderer.renderCopyImage(m_background, 0, 0, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);

        // render map
        for (int x = 0; x < Config::MAP_WIDTH; ++x) {
            for (int y = 0; y < Config::MAP_HEIGHT; ++y) {
                Tile tile = m_map.getTile(x, y);
                if (tile.getType() == Tile::TREE) {
                    m_renderer.renderCopyImage(m_tree, x * Config::MAP_UNIT_SIZE, y * Config::MAP_UNIT_SIZE, Config::MAP_UNIT_SIZE, Config::MAP_UNIT_SIZE);
                }
            }
        }

        m_renderer.present();
    }

    delete event;
}