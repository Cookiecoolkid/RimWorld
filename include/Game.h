#ifndef GAME_H
#define GAME_H

#include "Window.h"
#include "Renderer.h"
#include "Image.h"

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
};

#endif