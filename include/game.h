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
#include <array>
#include <string>


class Game {
public:
    Game(const std::string& title, int width, int height);
    ~Game();

    bool init();
    void run();
    bool init_load_image();
    bool init_image_path();
    void updateGameState();

private:
    Window m_window;
    Renderer m_renderer;
    TimeController m_timer;
    
    bool m_isRunning;
    
    Image m_background;
    Image m_tree;
    Image m_cuted_tree;
    std::string m_animalPath[2];
    Image m_animal_left;
    Image m_animal_right;

    std::string m_playerPath[16];
    std::array<Image, 4> m_player_up;
    std::array<Image, 4> m_player_down;
    std::array<Image, 4> m_player_left;
    std::array<Image, 4> m_player_right;


    Map m_map;
    // 地图渲染起始位置(最左上角的坐标)
    int m_mapStartX;
    int m_mapStartY;

    // 地图移动位置的 offset 以及此次移动当前的帧数
    int m_targetOffsetX = 0;
    int m_targetOffsetY = 0;
    int m_currentFrame = 0;
    bool m_isMapMoving = false;


    EventManager m_eventManager;

    // Callbacks
    void registerCallbacks();
    void onQuit(const SDL_Event& event);
    void onKeyDown(const SDL_Event& event);
    void onMouseButtonDown(const SDL_Event& event);

    // Game Logic
    void updateMapPosition(int dx, int dy);
    void movingMap();
};

#endif