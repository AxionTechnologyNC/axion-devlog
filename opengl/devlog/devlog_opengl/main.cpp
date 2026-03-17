#include "pch.h"

#include "app/App.h"
#include "scenes/CubeScene.h"

int main()
{
    App app(1280, 720, "Axion Devlog - OpenGL");
    if (!app.init()) return -1;

    app.setScene(std::make_unique<CubeScene>());

	return app.run();
}