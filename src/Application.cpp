#include "Application.h"

#include <iostream>
#include <vector>

#include "Logger.h"

Application::Application()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        Logger::printError("SDL could not be initialised! SDL_Error: " + std::string(SDL_GetError()));
    }

    windows.emplace("main", std::make_unique<Window>("NESBuddy", GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, true));
}

Application::~Application()
{
    SDL_Quit();
}

void Application::pollEvents(bool& isRunning)
{
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }

        Application::handleWindowEvents(event, isRunning);
    }
}

void Application::handleWindowEvents(SDL_Event& event, bool& isRunning)
{
    for (auto& [name, window] : windows) {
        window->handleEvents(event, isRunning);
    }
}

void Application::updateWindows()
{
    for (auto& [name, window] : windows) {
        window->updateScreen();
    }
}

void Application::queuePatternTableTileDrawing(const PatternTableTiles& tiles, std::string windowName)
{
    static std::vector<SDL_Rect> black;
    static std::vector<SDL_Rect> darkGrey;
    static std::vector<SDL_Rect> grey;
    static std::vector<SDL_Rect> white;

    int startingY = 0;
    int startingX = 0;

    if (black.size() > 0) {  // Skip filling vectors if already filled
        windows[windowName]->addToDrawQueue(black,    {0x00, 0x00, 0x00, 255});
        windows[windowName]->addToDrawQueue(grey,     {0x59, 0x59, 0x59, 255});
        windows[windowName]->addToDrawQueue(darkGrey, {0xA6, 0xA6, 0xA6, 255});
        windows[windowName]->addToDrawQueue(white,    {0xFF, 0xFF, 0xFF, 255});
        return;
    }

    for (int tile = 0; tile < 512; tile++) {
        if ((tile % 16) == 0 || tile == 0) {
            startingX = 0;
        } else {
            startingX += 16;  // Move to next column of tiles
        }

        for (int row = 0; row < 8; row++) {
            for (int column = 0; column < 8; column++) {
                SDL_Rect rect;
                rect.h = 2;
                rect.w = 2;
                rect.x = startingX + 2*column;
                rect.y = startingY + 2*row;

                switch (tiles[tile][row][column]) {
                    case 0:
                        black.push_back(rect);
                        break;
                    case 1:
                        darkGrey.push_back(rect);
                        break;
                    case 2:
                        grey.push_back(rect);
                        break;
                    case 3:
                        white.push_back(rect);
                        break;
                    default:
                        break;
                };
            }
        }

        if ((tile % 16) == 0 && tile != 0) {
            startingY += 16; // Move to next row of tiles
        }
    }

    windows[windowName]->addToDrawQueue(black,    {0x00, 0x00, 0x00, 255});
    windows[windowName]->addToDrawQueue(grey,     {0x59, 0x59, 0x59, 255});
    windows[windowName]->addToDrawQueue(darkGrey, {0xA6, 0xA6, 0xA6, 255});
    windows[windowName]->addToDrawQueue(white,    {0xFF, 0xFF, 0xFF, 255});
}