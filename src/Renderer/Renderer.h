#pragma once

#include <Camera/Camera.h>
#include <EntitiesPrototypes/EntitiesPrototypesTypes.h>

#include <bgfx/bgfx.h>

namespace Engine {

class Window;

}

class Renderer {
  struct NormalColorVertex {
    float x, y;
    float u, v;
};
public:
    Renderer(Camera *camera);
    ~Renderer();

    bool initialize(Engine::Window* window);
    void shutdown();

    void beginFrame();
    void endFrame();
    static bgfx::ShaderHandle loadShader(const char* path);
    static bgfx::TextureHandle loadTexture(const char* filePath);

    bgfx::VertexBufferHandle vbo;
    bgfx::IndexBufferHandle ibo;
    bgfx::ProgramHandle program;

private:
    bgfx::Init init;
    bgfx::VertexLayout layout;
    int64_t startTime, lastTime;
    double freq;
    Camera *camera;

    float projMatrix[16];
    NormalColorVertex kTriangleVertices[4] =
    {
        { -0.5f, -0.5f, 0.0f, 1.0f },
        {  0.5f, -0.5f, 1.0f, 1.0f },
        {  0.5f,  0.5f, 1.0f, 0.0f },
        { -0.5f,  0.5f, 0.0f, 0.0f },
    };

  const uint16_t kTriangleIndices[6] =
      {
          0, 1, 2, 2, 3, 0,
      };

};
