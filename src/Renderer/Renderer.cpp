#include <iostream>
#include <fstream>

#include <Renderer/Renderer.h>
#include <Game/Window.h>
#include <Renderer/Vertex.h>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>
#include <bx/timer.h>
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_WAYLAND
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#define STB_IMAGE_IMPLEMENTATION
#include <bgfx/bimg/3rdparty/stb/stb_image.h>

Renderer::Renderer(Camera *camera)
  : camera(camera){}

Renderer::~Renderer() {
    shutdown();
}

using Engine::Window;



bool Renderer::initialize(Engine::Window* window) {
    init.resolution.width = 800;
    init.resolution.height = 600;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.platformData.type = bgfx::NativeWindowHandleType::Wayland;
    // TBD
   // init.platformData.ndt = glfwGetX11Display();
   // init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window((GLFWwindow*)window->getNativeHandle());

    init.platformData.ndt = glfwGetWaylandDisplay();
    init.platformData.nwh = glfwGetWaylandWindow((GLFWwindow*)window->getNativeHandle());

    std::cout << "[Renderer] platformData.ndt = " << init.platformData.ndt << std::endl;
    std::cout << "[Renderer] platformData.nwh = " << init.platformData.nwh << std::endl;


    if (!bgfx::init(init)) {
        std::cerr << "[Renderer] Failed to initialize BGFX!" << std::endl;
        return false;
    }

    std::cout << "[Renderer] bgfx init done" << std::endl;


    int64_t startTime = bx::getHPCounter();
    double freq = double(bx::getHPFrequency());

    layout = Vertex::init();

    vbo = bgfx::createVertexBuffer(bgfx::makeRef(kTriangleVertices, sizeof(kTriangleVertices)), layout);
    ibo = bgfx::createIndexBuffer(bgfx::makeRef(kTriangleIndices, sizeof(kTriangleIndices)));

    //TBD
    program = bgfx::createProgram(
      loadShader("assets/shaders/simple.vb"),
      loadShader("assets/shaders/simple.fg"),
      true
    );

    float view[16];
    bx::mtxTranslate(view, 0.f, 0.f, 0.f);

    bx::mtxOrtho(
        projMatrix,
        0.0f, 800.0f,
        0.0f, 600.0f,
        -1.0f, 1.0f,
        0.0f,
        false
    );

    bgfx::setViewTransform(0, view, projMatrix);
    bgfx::setViewTransform(1, view, projMatrix);


    return true;
}

void Renderer::shutdown() {
    bgfx::shutdown();
}

void Renderer::beginFrame() {
    int64_t now = bx::getHPCounter();
    double deltaTime = double(now - lastTime) / freq;
    lastTime = now;

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, 800, 600);
    bgfx::setViewRect(1, 0, 0, 800, 600);

    camera->updateCamera(deltaTime);
    bgfx::touch(0);
    bgfx::touch(1);

    bgfx::setState(
      BGFX_STATE_WRITE_RGB |
      BGFX_STATE_WRITE_A |
      BGFX_STATE_BLEND_ALPHA
    );

    float view[16];
    bx::mtxIdentity(view);

    bx::mtxScale(view, 1.0f / camera->getZoom(), 1.0f / camera->getZoom(), 1.0f);

    float camTranslate[16];
    bx::mtxTranslate(camTranslate, -camera->getX(), -camera->getY(), 0.0f);

    float centerOffset[16];
    bx::mtxTranslate(centerOffset, 800 * 0.5f, 600 * 0.5f, 0.0f);

    float temp1[16];
    bx::mtxMul(temp1, camTranslate, view);
    float finalView[16];
    bx::mtxMul(finalView, centerOffset, temp1);

    bgfx::setViewTransform(0, finalView, projMatrix);
    bgfx::setViewTransform(1, finalView, projMatrix);
}

void Renderer::endFrame() {
    bgfx::frame();
}

bgfx::TextureHandle Renderer::loadTexture(const char* filePath) {
    int width, height, channels;
    stbi_uc* data = stbi_load(filePath, &width, &height, &channels, 4);

    if (!data) {
        std::cerr << "[Renderer] Failed to load texture: " << filePath << std::endl;
        return BGFX_INVALID_HANDLE;
    }

    const bgfx::Memory* mem = bgfx::copy(data, width * height * 4);
    bgfx::TextureHandle tex = bgfx::createTexture2D(
        (uint16_t)width, (uint16_t)height, false, 1,
        bgfx::TextureFormat::RGBA8,  BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT, mem
    );

    stbi_image_free(data);
    return tex;
}

bgfx::ShaderHandle Renderer::loadShader(const char* path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    auto size = file.tellg();
    file.seekg(0);
    auto* mem = new char[size];
    file.read(mem, size);
    file.close();
    return bgfx::createShader(bgfx::makeRef(mem, size));
}
