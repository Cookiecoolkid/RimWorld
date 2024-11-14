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
            int renderX = (x - mapStartX) * unitSize + mapMoving_offsetX;
            int renderY = (y - mapStartY) * unitSize + mapMoving_offsetY;

            switch (tile.getType()) {
                case Tile::TREE:
                    renderCopyImage(tree, renderX, renderY, unitSize, unitSize);
                    break;
                case Tile::ANIMAL: {
                    const Animal& animal = map.getAnimalAt(x, y);
                    

                    int animalRenderX = renderX + (animal.targetX - animal.x) * unitSize * animal.moveProgress / Config::ANIMAL_MOVE_FRAMES;
                    int animalRenderY = renderY + (animal.targetY - animal.y) * unitSize * animal.moveProgress / Config::ANIMAL_MOVE_FRAMES;
                    if (animal.img_direction == Entity::LEFT) {
                        renderCopyImage(animal_left, animalRenderX, animalRenderY, unitSize, unitSize);
                    } else if (animal.img_direction == Entity::RIGHT) {
                        renderCopyImage(animal_right, animalRenderX, animalRenderY, unitSize, unitSize);
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void Renderer::setMapMovingOffset(int offsetX, int offsetY) {
    mapMoving_offsetX = offsetX;
    mapMoving_offsetY = offsetY;
}

int Renderer::getMapMovingOffsetX() const {
    return mapMoving_offsetX;
}

int Renderer::getMapMovingOffsetY() const {
    return mapMoving_offsetY;
}   