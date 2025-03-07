#include "config.h"
#include <string>

// 标题
const std::string Config::WINDOW_TITLE = "RimWorld";
// 窗口宽度以及高度
const int Config::WINDOW_WIDTH = 2560;
const int Config::WINDOW_HEIGHT = 1600;

// 开始界面 开始按钮设置
const int Config::START_BUTTON_X = Config::WINDOW_WIDTH / 2 - 200;
const int Config::START_BUTTON_Y = Config::WINDOW_HEIGHT / 2 - 200;
const int Config::START_BUTTON_WIDTH = 400;
const int Config::START_BUTTON_HEIGHT = 200;

// Continue button configurations
const int Config::CONTINUE_BUTTON_X = START_BUTTON_X;
const int Config::CONTINUE_BUTTON_Y = START_BUTTON_Y + START_BUTTON_HEIGHT + 20; // 20 pixels below the Start button
const int Config::CONTINUE_BUTTON_WIDTH = START_BUTTON_WIDTH;
const int Config::CONTINUE_BUTTON_HEIGHT = START_BUTTON_HEIGHT;

// 字体文件路径
const std::string Config::FONT_PATH = "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf";

// 随机生成的树木数量
const int Config::INIT_TREE_COUNT = 128;

// 图片路径
const std::string Config::IMAGE_BACKGROUND_PATH = "image/out_desert.png";
const std::string Config::IMAGE_TREE_PATH = "image/tree.png";
const std::string Config::IMAGE_CUTED_TREE_PATH = "image/cuted_tree.png";
const std::string Config::IMAGE_WOOD_PATH = "image/wood.png";
const std::string Config::IMAGE_WALL_PATH = "image/wall.png";
const std::string Config::IMAGE_BGWALL_PATH = "image/BGwall.png";
const std::string Config::IMAGE_START_BACKGROUND_PATH = "image/start_background.png";

// 图片大小
const int Config::IMAGE_BACKGROUND_WIDTH = 5120;
const int Config::IMAGE_BACKGROUND_HEIGHT = 3200;
const int Config::IMAGE_TREE_SIZE = 64;
const int Config::IMAGE_CUTED_TREE_SIZE = 64;

// 地图单位大小
const int Config::MAP_UNIT_SIZE = 64;
// 地图放大比例
const int Config::MAP_RATIO = 2;
// 地图实际宽度以及高度
const int Config::MAP_WIDTH = Config::MAP_RATIO * Config::WINDOW_WIDTH / Config::MAP_UNIT_SIZE;
const int Config::MAP_HEIGHT = Config::MAP_RATIO * Config::WINDOW_HEIGHT / Config::MAP_UNIT_SIZE;
// 地图渲染宽度以及高度
const int Config::MAP_RENDER_WIDTH = Config::WINDOW_WIDTH / Config::MAP_UNIT_SIZE;
const int Config::MAP_RENDER_HEIGHT = Config::WINDOW_HEIGHT / Config::MAP_UNIT_SIZE;

// 地图移动速度
const int Config::MAP_MOVE_SPEED = 4;

// 一次移动所需帧数
const int Config::MOVE_FRAMES = 32;

// Animal
// 移动一次帧数
const int Config::ANIMAL_MOVE_FRAMES = 32;
// 不同移动的图片总数
const int Config::ANIMAL_IMAGE_NUMBERS = 2;
// 动物总数
const int Config::ANIMAL_NUMBERS = 50;


// Player
const int Config::PLAYER_NUMBERS = 4;
const int Config::PLAYER_IMAGE_NUMBERS = 16;
// 移动一次帧数
const int Config::PLAYER_MOVE_FRAMES = 32;
const int Config::PLAYER_IMAGE_ROUNDS = 2;
const int Config::PLAYER_IMAGE_ONE_ROUND = Config::PLAYER_MOVE_FRAMES / Config::PLAYER_IMAGE_ROUNDS;
const int Config::PLAYER_MAX_CAPACITY = 75;

// 砍树所需帧数
const int Config::CUT_TREE_FRAMES = 64;

// 砍树掉落的木头数量
const int Config::WOOD_DROP_COUNT = 55;

// 存储区最大木头数量
const int Config::STORE_MAX_CAPACITY = 75;

// 建造墙壁所需木头数量
const int Config::WALL_BUILD_WOOD_COUNT = 5;

// 建造墙壁所需帧数
const int Config::WALL_BUILD_FRAMES = 64;

// 存档 JSON 文件名
const std::string Config::SAVE_FILE_NAME = "save.json";