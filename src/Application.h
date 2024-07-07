#pragma once

#include <memory>
#include <array>
#include <unordered_map>
#include <string>

#include <SDL.h>

#include "Window.h"

constexpr int GAME_WINDOW_WIDTH  { 512 };
constexpr int GAME_WINDOW_HEIGHT { 480 };
constexpr int TILE_WINDOW_WIDTH  { 128 };
constexpr int TILE_WINDOW_HEIGHT { 512 };

using PatternTableTiles = std::array<std::array<std::array<int, 8>, 8>, 512>;

class Application
{
public:
    Application();
    ~Application();

    void pollEvents(bool& isRunning);
    void handleWindowEvents(SDL_Event& event, bool& isRunning);
    void updateWindows();

    void queuePatternTableTileDrawing(const PatternTableTiles& tiles, std::string windowName);

    std::unordered_map<std::string, std::unique_ptr<Window>> windows {};
private:
    SDL_Event event;
};