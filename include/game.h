#ifndef GAME_H
#define GAME_H

#include "window.h"
#include "renderer.h"
#include "image.h"
#include "map.h"
#include "eventManager.h"


class Game {
public:
    Game(const std::string& title, int width, int height);
    ~Game();

    bool init();
    void run();
    bool init_load_image();

private:
    Window m_window;
    Renderer m_renderer;
    
    bool m_isRunning;
    
    Image m_background;
    Image m_tree;
    Image m_cuted_tree;

    Map m_map;
    // 地图渲染起始位置(最左上角的坐标)
    int m_mapStartX;
    int m_mapStartY;
    EventManager m_eventManager;

    void onQuit(const SDL_Event& event);
    void onKeyDown(const SDL_Event& event);
    void onMouseButtonDown(const SDL_Event& event);

    void updateMapPosition(int dx, int dy);
    void registerCallbacks();
};

#endif