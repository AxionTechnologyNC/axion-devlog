#pragma once

class IScene
{
public:
    virtual ~IScene() = default;

    virtual bool init() = 0;
    virtual void shutdown() {}

    virtual void update(float time, float aspectRatio) = 0;
    virtual void render() = 0;

    virtual void onResize(int width, int height) {}
};