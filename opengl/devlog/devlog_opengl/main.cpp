#include "pch.h"
#include "app/App.h"

int main()
{
    App app(1280, 720, "Axion Devlog - OpenGL");
    if (!app.init())
        return -1;

    while (app.isRunning())
    {
        app.beginFrame();
        app.endFrame();
    }

	return 0;
}