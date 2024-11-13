#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>

class TimeController {
public:
    TimeController();
    ~TimeController();

    float getDeltaTicks();
private:
    Uint32 lastTime;
};

#endif // TIMER_H