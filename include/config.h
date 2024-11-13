#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <cstdio>

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

    // 随机生成的树木数量
    static const int INIT_TREE_COUNT;

    // 图片路径
    static const std::string IMAGE_BACKGROUND_PATH;
    static const std::string IMAGE_TREE_PATH;
    static const std::string IMAGE_CUTED_TREE_PATH;

    // 图片大小
    static const int IMAGE_BACKGROUND_SIZE;
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
    // 每30帧移动一次
    static const int ANIMAL_MOVE_FRAMES;
    // 不同移动的图片总数
    static const int ANIMAL_IMAGE_NUMBERS;
    // 动物总数
    static const int ANIMAL_NUMBERS;
};

#endif // CONFIG_H