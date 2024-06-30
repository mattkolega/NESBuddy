#include "Window.h"

#include "Logger.h"

Window::Window(std::string title, int width, int height, bool isPrimaryWindow)
{
    constexpr uint32_t WINDOW_FLAGS = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI;

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, WINDOW_FLAGS);
    if (window == NULL) {
        throw std::runtime_error("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }

    id = SDL_GetWindowID(window);
    this->isPrimaryWindow = isPrimaryWindow;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        throw std::runtime_error("Renderer could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, width, height);
    if (texture == NULL) {
        throw std::runtime_error("Texture could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }
}

Window::~Window()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Window::handleEvents(SDL_Event& event, bool& isRunning)
{
    if (isShown && event.window.windowID == id) {
        switch(event.window.event) {
            case SDL_WINDOWEVENT_CLOSE:
                SDL_HideWindow(window);
                isShown = false;
                if (isPrimaryWindow) isRunning = false;
                break;
        }
    }
}

void Window::updateScreen()
{
    if (!isShown) return;

    // Set texture render target
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw rects
    while (!drawQueue.empty()) {
        auto drawCommand = drawQueue.front();

        SDL_SetRenderDrawColor(renderer, drawCommand.fillColour[0], drawCommand.fillColour[1], drawCommand.fillColour[2], drawCommand.fillColour[3]);
        SDL_RenderFillRects(renderer, drawCommand.rects.data(), drawCommand.rects.size());

        drawQueue.pop();
    }

    // Render to window
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void Window::addToDrawQueue(std::vector<SDL_Rect> rects, std::array<int, 4> fillColour)
{
    if (!isShown) return;

    DrawCommand drawCommand;
    drawCommand.rects = rects;
    drawCommand.fillColour = fillColour;

    drawQueue.push(drawCommand);
}