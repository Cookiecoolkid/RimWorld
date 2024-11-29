#include "game.h"
#include "config.h"
#include <iostream>
#include <SDL2/SDL.h>
#include "save_load.h"

void Game::onQuit(const SDL_Event& event) {
    std::cout << "Quitting..." << std::endl;
    m_isRunning = false;

    // Save game state
    saveGame(*this, Config::SAVE_FILE_NAME);

    SDL_Quit();
    exit(0);
}

void Game::onKeyDown(const SDL_Event& event) {
    int speed = Config::MAP_MOVE_SPEED * Config::MAP_UNIT_SIZE;
    switch (event.key.keysym.sym) {
        case SDLK_q:
            onQuit(event);
            break;
        case SDLK_w:
            updateMapPosition(0, -speed);
            break;
        case SDLK_a:
            updateMapPosition(-speed, 0);
            break;
        case SDLK_s:
            updateMapPosition(0, speed);
            break;
        case SDLK_d:
            updateMapPosition(speed, 0);
            break;
        case SDLK_1:
            m_mode = MODE_NONE;
            DEBUG("Mode: None\n");
            break;
        case SDLK_2:
            m_mode = MODE_STORE;
            DEBUG("Mode: Store\n");
            break;
        case SDLK_3:
            m_mode = MODE_CUT;
            DEBUG("Mode: Cut\n");
            break;
        case SDLK_4:
            m_mode = MODE_BUILD;
            DEBUG("Mode: Build\n");
            break;
        default:
            break;
    }
}

void Game::onMouseButtonDown(const SDL_Event& event) {
    int x = event.button.x / Config::MAP_UNIT_SIZE + m_mapStartX;
    int y = event.button.y / Config::MAP_UNIT_SIZE + m_mapStartY;

    if (m_mode == MODE_START && event.button.button == SDL_BUTTON_LEFT) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;
        SDL_Rect startButton = { Config::START_BUTTON_X, Config::START_BUTTON_Y, 
                                 Config::START_BUTTON_WIDTH, Config::START_BUTTON_HEIGHT };

        SDL_Rect continueButton = { Config::CONTINUE_BUTTON_X, Config::CONTINUE_BUTTON_Y, 
                                    Config::CONTINUE_BUTTON_WIDTH, Config::CONTINUE_BUTTON_HEIGHT };

        if (mouseX >= startButton.x && mouseX <= startButton.x + startButton.w &&
            mouseY >= startButton.y && mouseY <= startButton.y + startButton.h) {
            m_mode = MODE_NONE; // 切换到游戏状态
        } else if (mouseX >= continueButton.x && mouseX <= continueButton.x + continueButton.w &&
                   mouseY >= continueButton.y && mouseY <= continueButton.y + continueButton.h) {
            loadGame(*this, Config::SAVE_FILE_NAME); // 加载游戏进度
            m_mode = MODE_NONE; // 切换到游戏状态
        }
    } else if ((m_mode == MODE_STORE || m_mode == MODE_CUT) && event.button.button == SDL_BUTTON_LEFT) {
        m_storeStartX = x;
        m_storeStartY = y;
    } else if (m_mode == MODE_BUILD && event.button.button == SDL_BUTTON_LEFT) {
        m_map.setBGWall(x, y);
    }
}

void Game::onMouseButtonUp(const SDL_Event& event) {
    int x = event.button.x / Config::MAP_UNIT_SIZE + m_mapStartX;
    int y = event.button.y / Config::MAP_UNIT_SIZE + m_mapStartY;
    if ((m_mode == MODE_STORE || m_mode == MODE_CUT) && event.button.button == SDL_BUTTON_LEFT) {
        m_storeEndX = x;
        m_storeEndY = y;

        if (m_storeStartX > m_storeEndX) {
            std::swap(m_storeStartX, m_storeEndX);
        }

        if (m_storeStartY > m_storeEndY) {
            std::swap(m_storeStartY, m_storeEndY);
        }

        if (m_mode == MODE_STORE) {
            m_map.setStoreArea(m_storeStartX, m_storeStartY, m_storeEndX, m_storeEndY);
        } else if (m_mode == MODE_CUT) {
            m_map.setCutArea(m_storeStartX, m_storeStartY, m_storeEndX, m_storeEndY);
        }
    }
}
