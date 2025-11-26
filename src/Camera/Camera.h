#pragma once

#include <GLFW/glfw3.h>

class Camera {
private:
    float zoom = 1.0f;
    float x = 0.0f;
    float y = 0.0f;
    GLFWwindow* window;
public:
    Camera(GLFWwindow* window);
    float getX() const { return x; }
    void setX(float value) { x = value; }

    float getY() const { return y; }
    void setY(float value) { y = value; }

    float getZoom() const { return zoom; }
    void setZoom(float value) {
        if (value > 0.1f && value < 10.0f) // prevent crazy zooms
            zoom = value;
    }
    void updateCamera(float deltaTime);
};
