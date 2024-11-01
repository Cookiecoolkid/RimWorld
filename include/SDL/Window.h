#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <string>

class Window {
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    bool init();
    SDL_Window* getSDLWindow() const;

private:
    std::string m_title;
    int m_width;
    int m_height;
    SDL_Window* m_window;
};

#endif