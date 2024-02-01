#include <SDL.h>

#include "Application.h"
#include "NES.h"

int main(int argc, char *argv[])
{
    Application application;

    NES nes;

    bool isRunning = true;

    while (isRunning)
    {
        application.pollEvents(isRunning);
        application.updateScreen();
    }

    return 0;
}