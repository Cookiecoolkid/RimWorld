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
    static const std::string WINDOW_TITLE;
    static const int WINDOW_WIDTH;
    static const int WINDOW_HEIGHT;

    // map
    static const int MAP_WIDTH;
    static const int MAP_HEIGHT;
    static const int MAP_UNIT_SIZE;
    static const int MAP_RATIO;
    static const int MAP_RENDER_WIDTH;
    static const int MAP_RENDER_HEIGHT;

    // Game setting
    static const int INIT_TREE_COUNT;

    // Image path
    static const std::string IMAGE_BACKGROUND_PATH;
    static const std::string IMAGE_TREE_PATH;
    static const std::string IMAGE_CUTED_TREE_PATH;

    // Image size
    static const int IMAGE_BACKGROUND_SIZE;
    static const int IMAGE_TREE_SIZE;
    static const int IMAGE_CUTED_TREE_SIZE;
};

#endif