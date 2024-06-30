#pragma once

#include <array>
#include <queue>
#include <string>
#include <vector>

#include <SDL.h>

struct DrawCommand
{
    std::vector<SDL_Rect> rects;
    std::array<int, 4> fillColour; // 8-bit RGBA
};

class Window
{
public:
    Window(std::string title, int width, int height, bool isPrimaryWindow = false);
    ~Window();

    void handleEvents(SDL_Event& event, bool& isRunning);
    void updateScreen();

    void addToDrawQueue(std::vector<SDL_Rect> rects, std::array<int, 4> fillColour);
private:
    int id;
    bool isPrimaryWindow;
    bool isShown { true };

    SDL_Window *window      { nullptr };
    SDL_Renderer *renderer  { nullptr };
    SDL_Texture *texture    { nullptr };

    std::queue<DrawCommand> drawQueue;
};