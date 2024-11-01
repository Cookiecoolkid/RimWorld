#include "config.h"
#include <string>

const std::string Config::WINDOW_TITLE = "RimWorld";
const int Config::WINDOW_WIDTH = 1024;
const int Config::WINDOW_HEIGHT = 1024;

const int Config::INIT_TREE_COUNT = 32;

const std::string Config::IMAGE_BACKGROUND_PATH = "image/purebackground.png";
const std::string Config::IMAGE_TREE_PATH = "image/tree.png";
const std::string Config::IMAGE_CUTED_TREE_PATH = "image/cuted_tree.png";

const int Config::IMAGE_BACKGROUND_SIZE = 1024;
const int Config::IMAGE_TREE_SIZE = 64;
const int Config::IMAGE_CUTED_TREE_SIZE = 64;

const int Config::MAP_UNIT_SIZE = 64;
const int Config::MAP_WIDTH = WINDOW_WIDTH / MAP_UNIT_SIZE;
const int Config::MAP_HEIGHT = WINDOW_HEIGHT / MAP_UNIT_SIZE;