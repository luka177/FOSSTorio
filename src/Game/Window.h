#pragma once

#include <vulkan/vulkan.h>

#include <Camera/Camera.h>

namespace Engine {

class Window {
public:
    virtual bool create(int width, int height, const char* title) = 0;
    virtual void pollEvents() = 0;
    virtual bool shouldClose() = 0;
    virtual void destroy() = 0;

    virtual void* getNativeHandle() = 0;
    virtual void registerCallbacks(Camera *camera) = 0;
    virtual ~Window() = default;
};
}
