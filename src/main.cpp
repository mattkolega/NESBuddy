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

        while (isRunning) {
            application.pollEvents(isRunning);
            application.updateScreen();
        }
    } catch (std::exception const &e) {
        Logger::printError(e.what());
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}