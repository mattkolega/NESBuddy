#include <SDL.h>

#include "Application.h"
#include "Logger.h"
#include "NES.h"

int main(int argc, char *argv[])
{
    try {
        Application application;

        NES nes;

        bool isRunning = true;

        PatternTableTiles tiles = nes.getPatternTableTiles();
        application.windows.emplace("tile", std::make_unique<Window>("Pattern Table Tiles", TILE_WINDOW_WIDTH, TILE_WINDOW_HEIGHT));

        while (isRunning) {
            application.pollEvents(isRunning);
            application.queuePatternTableTileDrawing(tiles, "tile");
            application.updateWindows();
        }
    } catch (std::exception const &e) {
        Logger::printError(e.what());
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}