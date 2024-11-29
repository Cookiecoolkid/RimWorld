#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include <string>
#include "game.h"

void saveGame(const Game& game, const std::string& filename);
void loadGame(Game& game, const std::string& filename);

#endif // SAVE_LOAD_H