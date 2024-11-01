#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "image.h"

class Renderer {
public:
    Renderer(SDL_Window* window);
    ~Renderer();

    bool init();
    void clear();
    void present();
    void renderCopyImage(const Image& image, int x, int y, int w, int h);
    SDL_Renderer* getSDLRenderer() const;

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
};

#endif // RENDERER_H