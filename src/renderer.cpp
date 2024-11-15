#include "renderer.h"
#include "image.h"
#include "config.h"
#include "map.h"
#include "SDL2/SDL.h"
#include <iostream>

Renderer::Renderer(SDL_Window* window): m_window(window), m_renderer(nullptr) {}

Renderer::~Renderer() {
    if (m_renderer) {
        DEBUG("Renderer Destructor\n");
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;

        if (m_font) {
            TTF_CloseFont(m_font);
            m_font = nullptr;
        }
    }
}   

bool Renderer::init() {
    DEBUG("Renderer Initialization\n");

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND); // 设置混合模式

    // 加载字体
    m_font = TTF_OpenFont(Config::FONT_PATH.c_str(), 24);
    if (!m_font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        return false;
    }

    DEBUG("Font init success\n");

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

void Renderer::renderMap(const Map& map, int mapStartX, int mapStartY, const Image& tree, const Image& cuted_tree,
                    const Image& animal_left, const Image& animal_right, std::array<Image, 4>& player_down,
                    std::array<Image, 4>& player_left, std::array<Image, 4>& player_right,
                    std::array<Image, 4>& player_up) {

    for (int y = mapStartY; y < mapStartY + Config::MAP_RENDER_HEIGHT && y < Config::MAP_HEIGHT; ++y) {
        for (int x = mapStartX; x < mapStartX + Config::MAP_RENDER_WIDTH && x < Config::MAP_WIDTH; ++x) {
            Tile tile = map.getTile(x, y);

            int unitSize = Config::MAP_UNIT_SIZE;
            int renderX = (x - mapStartX) * unitSize + mapMoving_offsetX;
            int renderY = (y - mapStartY) * unitSize + mapMoving_offsetY;

            switch (tile.getType()) {
                case Tile::TREE: {
                    renderCopyImage(tree, renderX, renderY, unitSize, unitSize);
                    break;
                }
                case Tile::CUTED_TREE: {
                    renderCopyImage(cuted_tree, renderX, renderY, unitSize, unitSize);
                    break;
                }
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
                case Tile::PLAYER: {
                    renderCopyImage(player_down[0], renderX, renderY, unitSize, unitSize);
                    break;
                }
                case Tile::STORE: {
                    // 设置渲染透明淡蓝色
                    SDL_SetRenderDrawColor(m_renderer, 173, 216, 230, 128);
                    SDL_Rect storeRect = { renderX, renderY, unitSize, unitSize };
                    SDL_RenderFillRect(m_renderer, &storeRect);
                    break;
                }
                case Tile::WALL:
                    break;
                case Tile::DOOR:
                    break;
                case Tile::EMPTY:
                    break;
                default:
                    break;
            }
        }
    }
}

void Renderer::renderStartScreen() {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255); // 设置背景颜色为黑色
    SDL_RenderClear(m_renderer);

    // 设置开始按钮的颜色和位置
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255); // 设置按钮颜色为白色
    SDL_Rect startButton = { Config::START_BUTTON_X, Config::START_BUTTON_Y, 
                             Config::START_BUTTON_WIDTH, Config::START_BUTTON_HEIGHT };
    SDL_RenderFillRect(m_renderer, &startButton);

    // 在按钮上渲染文字“Start”
    SDL_Color textColor = { 0, 0, 0, 255 }; // 黑色文字
    renderText("Start", startButton, textColor);
}

void Renderer::renderText(const std::string& text, const SDL_Rect& rect, SDL_Color color) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, text.c_str(), color);
    if (textSurface) {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
        if (textTexture) {
            int textWidth = textSurface->w;
            int textHeight = textSurface->h;
            SDL_Rect textRect = { rect.x + (rect.w - textWidth) / 2,
                                  rect.y + (rect.h - textHeight) / 2,
                                  textWidth, textHeight };
            SDL_RenderCopy(m_renderer, textTexture, nullptr, &textRect);
            SDL_DestroyTexture(textTexture);
        }
        SDL_FreeSurface(textSurface);
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