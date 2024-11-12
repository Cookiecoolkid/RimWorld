#include "config.h"
#include <string>

// 标题
const std::string Config::WINDOW_TITLE = "RimWorld";
// 窗口宽度以及高度
const int Config::WINDOW_WIDTH = 2560;
const int Config::WINDOW_HEIGHT = 1600;

// 随机生成的树木数量
const int Config::INIT_TREE_COUNT = 128;

// 图片路径
const std::string Config::IMAGE_BACKGROUND_PATH = "image/purebackground.png";
const std::string Config::IMAGE_TREE_PATH = "image/tree.png";
const std::string Config::IMAGE_CUTED_TREE_PATH = "image/cuted_tree.png";

// 图片大小
const int Config::IMAGE_BACKGROUND_SIZE = 2560;
const int Config::IMAGE_TREE_SIZE = 64;
const int Config::IMAGE_CUTED_TREE_SIZE = 64;

// 地图单位大小
const int Config::MAP_UNIT_SIZE = 64;
// 地图放大比例
const int Config::MAP_RATIO = 2;
// 地图实际宽度以及高度
const int Config::MAP_WIDTH = MAP_RATIO * WINDOW_WIDTH / MAP_UNIT_SIZE;
const int Config::MAP_HEIGHT = MAP_RATIO * WINDOW_HEIGHT / MAP_UNIT_SIZE;
// 地图渲染宽度以及高度
const int Config::MAP_RENDER_WIDTH = WINDOW_WIDTH / MAP_UNIT_SIZE;
const int Config::MAP_RENDER_HEIGHT = WINDOW_HEIGHT / MAP_UNIT_SIZE;

// 地图移动速度
const int Config::MAP_MOVE_SPEED = 1;