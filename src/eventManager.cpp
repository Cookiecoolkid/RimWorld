#include "eventManager.h"
#include <functional>
#include <SDL2/SDL.h>
#include "game.h"

void EventManager::registerCallback(EventType type, EventCallback callback) {
    callbacks[type] = callback;
}

void EventManager::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                if (callbacks.find(QUIT) != callbacks.end()) {
                    callbacks[QUIT](event);
                }
                break;
            case SDL_KEYDOWN:
                if (callbacks.find(KEYDOWN) != callbacks.end()) {
                    callbacks[KEYDOWN](event);
                }
                break;
            case SDL_KEYUP:
                if (callbacks.find(KEYUP) != callbacks.end()) {
                    callbacks[KEYUP](event);
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (callbacks.find(MOUSEBUTTONDOWN) != callbacks.end()) {
                    callbacks[MOUSEBUTTONDOWN](event);
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (callbacks.find(MOUSEBUTTONUP) != callbacks.end()) {
                    callbacks[MOUSEBUTTONUP](event);
                }
                break;
            case SDL_MOUSEMOTION:
                if (callbacks.find(MOUSEMOTION) != callbacks.end()) {
                    callbacks[MOUSEMOTION](event);
                }
                break;
            default:
                break;
        }
    }
}