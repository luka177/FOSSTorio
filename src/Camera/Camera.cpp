#include <cmath>
#include <iostream>
#include <bits/stdc++.h>

#include <Camera/Camera.h>

Camera::Camera(GLFWwindow* window) : window(window){}
void Camera::updateCamera(float deltaTime) {
    const float moveSpeed = 0.1f;
    const float zoomSpeed = 1.0f;

    float dx = 0.0f;
    float dy = 0.0f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) dy += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) dy -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) dx -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) dx += 1.0f;

    // Normalize diagonal motion (optional)
    if (dx != 0.0f && dy != 0.0f) {
        float norm = 1.0f / std::sqrt(2.0f);
        dx *= norm;
        dy *= norm;
    }

    setX(getX() + dx * moveSpeed * deltaTime / getZoom());
    setY(getY() + dy * moveSpeed * deltaTime / getZoom());
}