#include "Application.h"

#include <iostream>

Application::Application()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        std::cerr << "FATAL ERROR: SDL could not be initialised! SDL_Error: " << SDL_GetError() << std::endl;
    }

    uint32_t windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI;

    window = SDL_CreateWindow("NESBuddy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

    if (window == NULL) 
    {
        std::cerr << "FATAL ERROR: Window could not be created! SDL_Error: << " << SDL_GetError() << std::endl;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) 
    {
        std::cerr << "FATAL ERROR: Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

    if (texture == NULL) 
    {
        std::cerr << "FATAL ERROR: Screen Texture could not be created! SDL_Error: " << SDL_GetError() << std::endl;
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