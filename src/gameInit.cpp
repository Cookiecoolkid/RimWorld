#include "game.h"
#include "config.h"

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

    // Game 的 callback 函数初始化
    registerCallbacks();

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
