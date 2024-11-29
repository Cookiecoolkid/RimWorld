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

void Renderer::renderTree(int renderX, int renderY, const Map& map, int x, int y, const Image& tree) {
    // // 检查上下左右是否Type STORE 大于等于2
    // if (map.isAdjacentTypesReachCount(x, y, Tile::STORE, 2)) {
    //     renderStore(renderX, renderY);
    // }
    renderCopyImage(tree, renderX, renderY, Config::MAP_UNIT_SIZE, Config::MAP_UNIT_SIZE);
}

void Renderer::renderCutedTree(int renderX, int renderY, const Map& map, int x, int y, const Image& cuted_tree) {
    // // 检查上下左右是否Type STORE 大于等于2
    // if (map.isAdjacentTypesReachCount(x, y, Tile::STORE, 2)) {
    //     renderStore(renderX, renderY);
    // }
    renderCopyImage(cuted_tree, renderX, renderY, Config::MAP_UNIT_SIZE, Config::MAP_UNIT_SIZE);
}

void Renderer::renderWood(int renderX, int renderY, const Map& map, int x, int y, const Image& wood) {
    renderCopyImage(wood, renderX, renderY, Config::MAP_UNIT_SIZE, Config::MAP_UNIT_SIZE);
    const Tile& tile = map.getTile(x, y);
    renderWoodCount(renderX, renderY, tile.getWoodCount());
}

void Renderer::renderAnimal(int renderX, int renderY, const Map& map, int x, int y, const Image& animal_left, const Image& animal_right) {
    const Animal& animal = map.getAnimalAt(x, y);
    int animalRenderX = renderX + (animal.targetX - animal.x) * Config::MAP_UNIT_SIZE * animal.stepProgress / Config::ANIMAL_MOVE_FRAMES;
    int animalRenderY = renderY + (animal.targetY - animal.y) * Config::MAP_UNIT_SIZE * animal.stepProgress / Config::ANIMAL_MOVE_FRAMES;
    if (animal.img_direction == Entity::LEFT) {
        renderCopyImage(animal_left, animalRenderX, animalRenderY, Config::MAP_UNIT_SIZE, Config::MAP_UNIT_SIZE);
    } else if (animal.img_direction == Entity::RIGHT) {
        renderCopyImage(animal_right, animalRenderX, animalRenderY, Config::MAP_UNIT_SIZE, Config::MAP_UNIT_SIZE);
    }
}

void Renderer::renderPlayer(int renderX, int renderY, const Map& map, int x, int y, 
                                                                    const std::array<Image, 4>& player_down,
                                                                    const std::array<Image, 4>& player_left, 
                                                                    const std::array<Image, 4>& player_right, 
                                                                    const std::array<Image, 4>& player_up) {
    const Player& player = map.getPlayerAt(x, y);
    int unitSize = Config::MAP_UNIT_SIZE;
    int playerRenderX = renderX + (player.targetX - player.x) * Config::MAP_UNIT_SIZE * player.stepProgress / Config::MOVE_FRAMES;
    int playerRenderY = renderY + (player.targetY - player.y) * Config::MAP_UNIT_SIZE * player.stepProgress / Config::MOVE_FRAMES;
    int stepProgress = player.stepProgress;
    int direction = player.direction;
    int oneRound = Config::PLAYER_IMAGE_ONE_ROUND;
    int framesPerImage = Config::PLAYER_IMAGE_ONE_ROUND / 4;
    // 根据方向选择不同的图片 且根据 stepProgress 处于 oneRound 的第几个图片
    int playerImageIndex = (stepProgress % oneRound) / framesPerImage;
    if (direction == Entity::DOWN) {
        renderCopyImage(player_down[playerImageIndex], playerRenderX, playerRenderY, unitSize, unitSize);
    } else if (direction == Entity::LEFT) {
        renderCopyImage(player_left[playerImageIndex], playerRenderX, playerRenderY, unitSize, unitSize);
    } else if (direction == Entity::RIGHT) {
        renderCopyImage(player_right[playerImageIndex], playerRenderX, playerRenderY, unitSize, unitSize);
    } else if (direction == Entity::UP) {
        renderCopyImage(player_up[playerImageIndex], playerRenderX, playerRenderY, unitSize, unitSize);
    }

    // Render progress bar
    if (player.isCutting) {
        renderProgressBar(playerRenderX, playerRenderY, player.cutProgress, Config::CUT_TREE_FRAMES);
    } else if (player.isBuilding) {
        renderProgressBar(playerRenderX, playerRenderY, player.buildProgress, Config::WALL_BUILD_FRAMES);
    }

    if (player.carryingWood > 0) {
        renderWoodCount(playerRenderX, playerRenderY, player.carryingWood);
    }
}

void Renderer::renderStore(int renderX, int renderY, const Map& map, int x, int y) {
    SDL_SetRenderDrawColor(m_renderer, 173, 216, 230, 128);
    SDL_Rect storeRect = { renderX, renderY, Config::MAP_UNIT_SIZE, Config::MAP_UNIT_SIZE };
    SDL_RenderFillRect(m_renderer, &storeRect);

    if (map.getTile(x, y).getWoodCount() > 0) {
        renderWoodCount(renderX, renderY, map.getTile(x, y).getWoodCount());
    }
}

void Renderer::renderStoreAreaAndBlueGraphWall(const Map& map, int mapStartX, int mapStartY) {
    for (int y = mapStartY; y < mapStartY + Config::MAP_RENDER_HEIGHT && y < Config::MAP_HEIGHT; ++y) {
        for (int x = mapStartX; x < mapStartX + Config::MAP_RENDER_WIDTH && x < Config::MAP_WIDTH; ++x) {
            Tile tile = map.getTile(x, y);
            int unitSize = Config::MAP_UNIT_SIZE;
            int renderX = (x - mapStartX) * unitSize + mapMoving_offsetX;
            int renderY = (y - mapStartY) * unitSize + mapMoving_offsetY;
            if (tile.hasType(Tile::STORE)) {
                renderStore(renderX, renderY, map, x, y);
            }
            if (tile.hasType(Tile::BGWALL)) {
                renderBlueGraphWall(renderX, renderY, map, x, y);
            }
        }
    }
}

void Renderer::renderWall(int renderX, int renderY, const Map& map, int x, int y, const Image& wall) {
    renderCopyImage(wall, renderX, renderY, Config::MAP_UNIT_SIZE, Config::MAP_UNIT_SIZE);
}

void Renderer::renderBlueGraphWall(int renderX, int renderY, const Map& map, int x, int y) {
    SDL_SetRenderDrawColor(m_renderer, 210, 180, 140, 128); // 设置浅褐色 (210, 180, 140) 和透明度 (128)
    SDL_Rect wallRect = { renderX, renderY, Config::MAP_UNIT_SIZE, Config::MAP_UNIT_SIZE };
    SDL_RenderFillRect(m_renderer, &wallRect);
}

void Renderer::renderBackground(const Image& image, int offsetX, int offsetY) {
    int imageWidth = Config::IMAGE_BACKGROUND_WIDTH;
    int imageHeight = Config::IMAGE_BACKGROUND_HEIGHT;

    // 计算背景图片的裁剪区域
    int clipX = offsetX % imageWidth;
    int clipY = offsetY % imageHeight;

    SDL_Rect srcRect = { clipX, clipY, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT };
    SDL_Rect destRect = { 0, 0, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT };

    // 渲染背景图片
    SDL_RenderCopy(m_renderer, image.getTexture(), &srcRect, &destRect);
}

void Renderer::renderProgressBar(int renderX, int renderY, int progress, int maxProgress) {
    int barWidth = Config::MAP_UNIT_SIZE;
    int barHeight = 5;
    int completedWidth = (progress * barWidth) / maxProgress;
    int remainingWidth = barWidth - completedWidth;

    // Render completed part in green
    SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);
    SDL_Rect completedRect = { renderX, renderY - barHeight - 2, completedWidth, barHeight };
    SDL_RenderFillRect(m_renderer, &completedRect);

    // Render remaining part in white
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_Rect remainingRect = { renderX + completedWidth, renderY - barHeight - 2, remainingWidth, barHeight };
    SDL_RenderFillRect(m_renderer, &remainingRect);
}

void Renderer::renderWoodCount(int renderX, int renderY, int woodCount) {
    SDL_Color textColor = { 255, 255, 255, 255 }; // White color
    std::string woodCountText = std::to_string(woodCount);
    SDL_Rect textRect = { renderX, renderY, Config::MAP_UNIT_SIZE, Config::MAP_UNIT_SIZE };
    renderText(woodCountText, textRect, textColor);
}


void Renderer::renderMap(const Map& map, int mapStartX, int mapStartY, const Image& background,
                         const Image& tree, const Image& cuted_tree, const Image& wood,
                         const Image& wall, const Image& BGwall,
                         const Image& animal_left, const Image& animal_right, std::array<Image, 4>& player_down,
                         std::array<Image, 4>& player_left, std::array<Image, 4>& player_right,
                         std::array<Image, 4>& player_up) {

    // renderCopyImage(background, 0, 0, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    int backgroundOffsetX = mapStartX * Config::MAP_UNIT_SIZE - mapMoving_offsetX;
    int backgroundOffsetY = mapStartY * Config::MAP_UNIT_SIZE - mapMoving_offsetY;

    renderBackground(background, backgroundOffsetX, backgroundOffsetY);

    // 先渲染 STORE 图层 保证 STORE 在最下层完整显示
    renderStoreAreaAndBlueGraphWall(map, mapStartX, mapStartY);

    for (int y = mapStartY; y < mapStartY + Config::MAP_RENDER_HEIGHT && y < Config::MAP_HEIGHT; ++y) {
        for (int x = mapStartX; x < mapStartX + Config::MAP_RENDER_WIDTH && x < Config::MAP_WIDTH; ++x) {
            Tile tile = map.getTile(x, y);

            int unitSize = Config::MAP_UNIT_SIZE;
            int renderX = (x - mapStartX) * unitSize + mapMoving_offsetX;
            int renderY = (y - mapStartY) * unitSize + mapMoving_offsetY;


            // 再渲染其他类型的 Tile
            if (tile.hasType(Tile::TREE)) {
                renderTree(renderX, renderY, map, x, y, tree);
            }
            if (tile.hasType(Tile::CUTED_TREE)) {
                renderCutedTree(renderX, renderY, map, x, y, cuted_tree);
            }
            if (tile.hasType(Tile::ANIMAL)) {
                renderAnimal(renderX, renderY, map, x, y, animal_left, animal_right);
            }
            if (tile.hasType(Tile::PLAYER)) {
                renderPlayer(renderX, renderY, map, x, y, player_down, player_left, player_right, player_up);
            } 
            if (tile.hasType(Tile::WOOD)) {
                renderWood(renderX, renderY, map, x, y, wood);
            }
            if (tile.hasType(Tile::WALL)) {
                renderWall(renderX, renderY, map, x, y, wall);
            }
            if (tile.hasType(Tile::DOOR)) {
                // TODO
            }
            if (tile.hasType(Tile::EMPTY)) {
                // Do nothing
            }
        }
    }
}

void Renderer::renderStartScreen(const Image& start_background) {
    // SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255); // 设置背景颜色为黑色
    // SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, start_background.getTexture(), nullptr, nullptr);

    // 设置开始按钮的颜色和位置
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 128); // 设置按钮颜色为白色
    SDL_Rect startButton = { Config::START_BUTTON_X, Config::START_BUTTON_Y, 
                             Config::START_BUTTON_WIDTH, Config::START_BUTTON_HEIGHT };
    SDL_RenderFillRect(m_renderer, &startButton);

    // 在按钮上渲染文字“Start”
    SDL_Color textColor = { 0, 0, 0, 255 }; // 黑色文字
    renderText("Start", startButton, textColor);

    // 设置继续按钮的颜色和位置
    SDL_Rect continueButton = { Config::CONTINUE_BUTTON_X, Config::CONTINUE_BUTTON_Y, 
                                Config::CONTINUE_BUTTON_WIDTH, Config::CONTINUE_BUTTON_HEIGHT };
    SDL_RenderFillRect(m_renderer, &continueButton);

    // 在按钮上渲染文字“Continue”
    renderText("Continue", continueButton, textColor);
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