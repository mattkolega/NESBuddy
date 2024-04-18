#include "Application.h"

#include <string>

#include "Logger.h"

Application::Application()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        Logger::printError("SDL could not be initialised! SDL_Error: " + std::string(SDL_GetError()));
    }

    uint32_t windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI;

    window = SDL_CreateWindow("NESBuddy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

    if (window == NULL) 
    {
        Logger::printError("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) 
    {
        Logger::printError("Renderer could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

    if (texture == NULL) 
    {
        Logger::printError("Screen Texture could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }
}

Application::~Application()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

void Application::pollEvents(bool &isRunning)
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                isRunning = false;
                break;
        }
    }
}

void Application::updateScreen()
{
    // Render to texture
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render to window
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}