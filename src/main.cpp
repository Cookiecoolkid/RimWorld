#include <SDL2/SDL.h>
#include <iostream>
#include "game.h"
#include "config.h"

int main(int argc, char* argv[]) {
    DEBUG("Begin main\n");

    Game game(Config::WINDOW_TITLE, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);

    DEBUG("Create Game\n");

    game.run();

    DEBUG("End main\n");

    return 0;
}