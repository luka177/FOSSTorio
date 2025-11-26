#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <cmath>
#include <iostream>
#include <bits/stdc++.h>

#include <Camera/Camera.h>
#include <Game/Window.h>

class WindowLinux : public Engine::Window {
public:
    WindowLinux() : window(nullptr) {}

    bool create(int width, int height, const char* title) override {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW!" << std::endl;
            return false;
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        return window != nullptr;
    }

    void pollEvents() override {
        glfwPollEvents();
    }

    bool shouldClose() override {
        return glfwWindowShouldClose(window);
    }

    void destroy() override {
        if (window) glfwDestroyWindow(window);
        glfwTerminate();
    }

    void* getNativeHandle() override {
        return window;
    }

    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
        float zoom = cam->getZoom();
        zoom *= std::pow(1.1f, -yoffset);
        zoom = std::clamp(zoom, 0.1f, 4.0f);
        cam->setZoom(zoom);
    }

    void registerCallbacks(Camera *camera) override {
        glfwSetWindowUserPointer(window, camera);
        glfwSetScrollCallback(window, scrollCallback);
    }
private:
    GLFWwindow* window;
};
