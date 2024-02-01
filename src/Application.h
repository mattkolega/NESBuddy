#pragma once

#include <SDL.h>

constexpr int SCREEN_WIDTH { 512 };
constexpr int SCREEN_HEIGHT { 480 };

class Application
{
public:
    Application();
    ~Application();

    void pollEvents(bool &isRunning);
    void updateScreen();
private:
    SDL_Window *window {};
    SDL_Renderer *renderer {};
    SDL_Texture *texture {};

    SDL_Event event;
};