#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "map.h"
#include "image.h"
#include <array>

class Renderer {
public:
    Renderer(SDL_Window* window);
    ~Renderer();

    bool init();
    void clear();
    void present();
    void renderCopyImage(const Image& image, int x, int y, int w, int h);
    void renderStartScreen();   

    void setMapMovingOffset(int offsetX, int offsetY);
    int getMapMovingOffsetX() const;
    int getMapMovingOffsetY() const;
    
    SDL_Renderer* getSDLRenderer() const;

    void renderMap(const Map& map, int mapStartX, int mapStartY, const Image& image, 
        const Image& animal_left, const Image& animal_right, std::array<Image, 4>& player_down,
        std::array<Image, 4>& player_left, std::array<Image, 4>& player_right, 
        std::array<Image, 4>& player_up);

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

    int mapMoving_offsetX = 0;
    int mapMoving_offsetY = 0;
};

#endif // RENDERER_H