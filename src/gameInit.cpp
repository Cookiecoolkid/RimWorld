#include "game.h"
#include "config.h"

bool Game::init() {
    DEBUG("Game Initialization\n");
    // SDL 初始化
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    DEBUG("SDL_Init Success\n");

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

    DEBUG("Renderer Init Success\n");

    // Game 的 callback 函数初始化
    registerCallbacks();

    // load image 
    if (!init_image_path()) {
        return false;
    }

    if (!init_load_image()) {
        return false;
    }

    DEBUG("Load Image Success\n");

    // map type 初始化
    for (int y = 0; y < Config::MAP_HEIGHT; ++y) {
        for (int x = 0; x < Config::MAP_WIDTH; ++x) {
            m_map.setTile(x, y, Tile::EMPTY);
        }
    }

    m_map.placeRandomTrees(Config::INIT_TREE_COUNT);

    // Animal 初始化
    for (int i = 0; i < Config::ANIMAL_NUMBERS; ++i) {
        m_map.setTile(m_map.m_animal_entity[i].x, m_map.m_animal_entity[i].y, Tile::ANIMAL);
    }

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

    m_animal_left = Image(m_animalPath[0]);
    if (!m_animal_left.load(m_renderer.getSDLRenderer())) {
        return false;
    }

    m_animal_right = Image(m_animalPath[1]);
    if (!m_animal_right.load(m_renderer.getSDLRenderer())) {
        return false;
    }

    return true;
}

bool Game::init_image_path() {
    for (int i = 0; i < 2; i++) {
        m_animalPath[i] = "image/animal_" + std::to_string(i) + ".png";
    }
    return true;
}