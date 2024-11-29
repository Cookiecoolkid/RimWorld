#ifndef GAME_H
#define GAME_H

#include "window.h"
#include "renderer.h"
#include "image.h"
#include "map.h"
#include "timeController.h"
#include "eventManager.h"
#include "entity.h"
#include "config.h"
#include <SDL2/SDL_ttf.h>
#include <array>
#include <string>


#ifndef IMG_INIT_GIF
#define IMG_INIT_GIF 0x00000004
#endif

class Game {
public:
    Game(const std::string& title, int width, int height);
    ~Game();

    enum MODE {
        MODE_START,
        MODE_NONE,
        MODE_STORE,
        MODE_CUT,
        MODE_BUILD
    };

    bool init();
    void run();
    bool init_load_image();
    bool init_image_path();
    void updateGameState();
    void updateAnimalState();
    void updatePlayerState();

    // 友元函数声明
    friend void saveGame(const Game& game, const std::string& filename);
    friend void loadGame(Game& game, const std::string& filename);

private:
    // Basic Game Components 
    Window m_window;
    Renderer m_renderer;
    TimeController m_timer;

    // Game State
    MODE m_mode;
    bool m_isRunning;
    
    // Images
    Image m_background;
    Image m_start_background;
    Image m_tree;
    Image m_cuted_tree;
    Image m_wood;
    std::string m_animalPath[2];
    Image m_animal_left;
    Image m_animal_right;
    Image m_wall;
    Image m_BGwall;

    std::string m_playerPath[16];
    std::array<Image, 4> m_player_up;
    std::array<Image, 4> m_player_down;
    std::array<Image, 4> m_player_left;
    std::array<Image, 4> m_player_right;

    // Map
    Map m_map;
    // 地图渲染起始位置(最左上角的坐标)
    int m_mapStartX;
    int m_mapStartY;

    // 地图移动位置的 offset 以及此次移动当前的帧数
    int m_targetOffsetX = 0;
    int m_targetOffsetY = 0;
    int m_currentFrame = 0;
    bool m_isMapMoving = false;

    // Store Area
    int m_storeStartX = 0;
    int m_storeStartY = 0;
    int m_storeEndX = 0;
    int m_storeEndY = 0;


    EventManager m_eventManager;

    // Callbacks
    void registerCallbacks();
    void onQuit(const SDL_Event& event);
    void onKeyDown(const SDL_Event& event);
    void onMouseButtonDown(const SDL_Event& event);
    void onMouseButtonUp(const SDL_Event& event);

    // Game Logic
    void updateMapPosition(int dx, int dy);
    void movingMap();
};

#endif