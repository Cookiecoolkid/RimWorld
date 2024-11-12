#include "game.h"
#include "config.h"
#include <iostream>
#include <SDL2/SDL.h>

void Game::onQuit(const SDL_Event& event) {
    std::cout << "Quitting..." << std::endl;
    m_isRunning = false;

    SDL_Quit();
    exit(0);
}

void Game::onKeyDown(const SDL_Event& event) {
    int speed = Config::MAP_MOVE_SPEED;
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
        default:
            std::cout << "Key down: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
            break;
    }
}

void Game::onMouseButtonDown(const SDL_Event& event) {
    std::cout << "Mouse button down at (" << event.button.x << ", " << event.button.y << ")" << std::endl;
}
