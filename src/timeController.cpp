#include "timeController.h"
#include <SDL2/SDL.h>

TimeController::TimeController(): lastTime(SDL_GetTicks()) {}

TimeController::~TimeController() {}

float TimeController::getDeltaTicks() {
    Uint32 currentTime = SDL_GetTicks();
    Uint32 deltaTicks = currentTime - lastTime;
    lastTime = currentTime;
    return deltaTicks / 1000.0f;
}