#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <cstdio>
#include <cassert>

#define DEBUG_MODE

#ifdef DEBUG_MODE
#define DEBUG(fmt, ...) do { printf(fmt, ##__VA_ARGS__); fflush(stdout); } while (0)
#else 
#define DEBUG(fmt, ...)
#endif

class Config {
public:
    // 标题
    static const std::string WINDOW_TITLE;
    // 窗口宽度以及高度
    static const int WINDOW_WIDTH;
    static const int WINDOW_HEIGHT;

    // 开始界面 开始按钮设置
    static const int START_BUTTON_X;
    static const int START_BUTTON_Y;
    static const int START_BUTTON_WIDTH;
    static const int START_BUTTON_HEIGHT;

    // 字体文件路径
    static const std::string FONT_PATH;

    // 随机生成的树木数量
    static const int INIT_TREE_COUNT;

    // 图片路径
    static const std::string IMAGE_BACKGROUND_PATH;
    static const std::string IMAGE_TREE_PATH;
    static const std::string IMAGE_CUTED_TREE_PATH;
    static const std::string IMAGE_WOOD_PATH;

    // 图片大小
    static const int IMAGE_BACKGROUND_WIDTH;
    static const int IMAGE_BACKGROUND_HEIGHT;
    static const int IMAGE_TREE_SIZE;
    static const int IMAGE_CUTED_TREE_SIZE;

    // 地图单位大小
    static const int MAP_UNIT_SIZE;
    // 地图放大比例
    static const int MAP_RATIO;
    // 地图实际宽度以及高度
    static const int MAP_WIDTH;
    static const int MAP_HEIGHT;
    // 地图渲染宽度以及高度
    static const int MAP_RENDER_WIDTH;
    static const int MAP_RENDER_HEIGHT;

    // 地图移动速度
    static const int MAP_MOVE_SPEED;

    // 一次移动所需帧数
    static const int MOVE_FRAMES;

    // Animal
    // 移动一次所需帧数
    static const int ANIMAL_MOVE_FRAMES;
    // 不同移动的图片总数
    static const int ANIMAL_IMAGE_NUMBERS;
    // 动物总数
    static const int ANIMAL_NUMBERS;


    // Player
    static const int PLAYER_NUMBERS;
    static const int PLAYER_IMAGE_NUMBERS;
    // 移动一次所需帧数
    static const int PLAYER_MOVE_FRAMES;
    static const int PLAYER_IMAGE_ROUNDS;
    static const int PLAYER_IMAGE_ONE_ROUND;

    // 砍树所需帧数
    static const int CUT_TREE_FRAMES;
};

#endif // CONFIG_H