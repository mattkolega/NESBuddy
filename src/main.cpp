#include <SDL.h>

#include "Application.h"

int main(int argc, char *argv[])
{
    Application application;

    bool isRunning = true;

    while (isRunning)
    {
        application.pollEvents(isRunning);
        application.updateScreen();
    }

    return 0;
}