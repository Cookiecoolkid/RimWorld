#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <SDL2/SDL.h>
#include <functional>
#include <unordered_map>

class EventManager {
public:
    enum EventType {
        QUIT,
        KEYDOWN,
        KEYUP,
        MOUSEBUTTONDOWN,
        MOUSEBUTTONUP,
        MOUSEMOTION
    };

    using EventCallback = std::function<void(const SDL_Event&)>;

    void registerCallback(EventType type, EventCallback callback);
    void registerCallbacks();
    void handleEvents();

private:
    std::unordered_map<EventType, EventCallback> callbacks;
};

#endif // EVENT_MANAGER_H