#include <SDL2/SDL.h>
#include <iostream>
#include "eventManager.h"
#include "eventCallback.h"

void onQuit(const SDL_Event& event) {
    std::cout << "Quit event received" << std::endl;
    SDL_Quit();
    exit(0);
}

void onKeyDown(const SDL_Event& event) {
    if (event.key.keysym.sym == SDLK_q) {
        onQuit(event);
    } else {
        std::cout << "Key down: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
    }
}

void onMouseButtonDown(const SDL_Event& event) {
    std::cout << "Mouse button down at (" << event.button.x << ", " << event.button.y << ")" << std::endl;
}
