#include "renderer.h"
#include "image.h"
#include "config.h"
#include "map.h"
#include <iostream>

Renderer::Renderer(SDL_Window* window)
    : m_window(window), m_renderer(nullptr) {}

Renderer::~Renderer() {
    DEBUG("Renderer Destructor\n");
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
}

bool Renderer::init() {
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

void Renderer::clear() {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
}

void Renderer::present() {
    SDL_RenderPresent(m_renderer);
}

void Renderer::renderCopyImage(const Image& image, int x, int y, int w, int h) {
    SDL_Rect destRect = { x, y, w, h };
    SDL_RenderCopy(m_renderer, image.getTexture(), nullptr, &destRect);
}

SDL_Renderer* Renderer::getSDLRenderer() const {
    return m_renderer;
}

void Renderer::renderMap(const Map& map, int mapStartX, int mapStartY, const Image& tree, 
                    const Image& animal_left, const Image& animal_right) {
    for (int y = mapStartY; y < mapStartY + Config::MAP_RENDER_HEIGHT && y < Config::MAP_HEIGHT; ++y) {
        for (int x = mapStartX; x < mapStartX + Config::MAP_RENDER_WIDTH && x < Config::MAP_WIDTH; ++x) {
            Tile tile = map.getTile(x, y);
            int unitSize = Config::MAP_UNIT_SIZE;
            int renderX = (x - mapStartX) * Config::MAP_UNIT_SIZE + render_offsetX;
            int renderY = (y - mapStartY) * Config::MAP_UNIT_SIZE + render_offsetY;
            switch (tile.getType()) {
                case Tile::TREE:
                    renderCopyImage(tree, renderX, renderY, unitSize, unitSize);
                    break;
                case Tile::ANIMAL_LEFT:
                    renderCopyImage(animal_left, renderX, renderY, unitSize, unitSize);
                    break;
                case Tile::ANIMAL_RIGHT:
                    renderCopyImage(animal_right, renderX, renderY, unitSize, unitSize);
                    break;
                default:
                    break;
            }
        }
    }
}

void Renderer::setOffset(int offsetX, int offsetY) {
    render_offsetX = offsetX;
    render_offsetY = offsetY;
}

int Renderer::getOffsetX() const {
    return render_offsetX;
}

int Renderer::getOffsetY() const {
    return render_offsetY;
}   