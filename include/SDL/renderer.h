#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "map.h"
#include "image.h"

class Renderer {
public:
    Renderer(SDL_Window* window);
    ~Renderer();

    bool init();
    void clear();
    void present();
    void renderCopyImage(const Image& image, int x, int y, int w, int h);

    void setOffset(int offsetX, int offsetY);
    int getOffsetX() const;
    int getOffsetY() const;
    
    SDL_Renderer* getSDLRenderer() const;

    void renderMap(const Map& map, int mapStartX, int mapStartY, const Image& image);

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

    int render_offsetX = 0;
    int render_offsetY = 0;
};

#endif // RENDERER_H