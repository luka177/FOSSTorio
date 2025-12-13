#include <iostream>
#include <memory>
#include <Lua/LuaEngine.h>
#include <Atlas/TextureAtlas.h>
#include <Atlas/TextureAtlasSystem.h>
#include <Tile/TileTextureManager.h>
#include <Entity/Coordinator.h>
#include <Prototype/PrototypeRegister.h>
#include <Animation/AnimationParameters.h>
#include <Simulation/BeltMovementSystem.h>
#include <misc.h>

// Global pointer TBD: put inside a GameManager or globalize
std::unique_ptr<LuaEngine> luaEngine;

#include <Renderer/Renderer.h>
#include <Camera/Camera.h>

using Engine::Window;

// platform-specific window
// TBD: Evryhing beside linux
#ifdef PLATFORM_LINUX
#include "platform/linux/WindowLinux.cpp"
#endif
#ifdef PLATFORM_WINDOWS
#include "platform/windows/WindowWin.cpp"
#endif
#ifdef PLATFORM_ANDROID
#include "platform/android/WindowAndroid.cpp"
#endif

#include <bx/timer.h>
#include <bx/thread.h>
#include <bx/os.h>

int main() {
    std::cout << "Starting factorio-oss Engine..." << std::endl;

    // TBD: Non linux
    std::unique_ptr<Window> window = std::make_unique<WindowLinux>();
    if (!window->create(800, 600, "factorio-oss Engine")) {
        std::cerr << "Failed to create window!" << std::endl;
        return -1;
    }

    Coordinator::Instance().Init();
    Coordinator::Instance().RegisterComponent<Vec2>();
    Coordinator::Instance().RegisterComponent<PrototypeID>();
    Coordinator::Instance().RegisterComponent<AnimationFrameComponent>();
    Coordinator::Instance().RegisterComponent<Direction>();
    Coordinator::Instance().RegisterComponent<BeltComponent>();

    // TBD: Globalize or put into GameManager
    std::unique_ptr<Camera> camera = std::make_unique<Camera>((GLFWwindow*)window->getNativeHandle());

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(camera.get());
    if (!renderer->initialize(window.get())) {
        std::cerr << "Failed to initialize renderer!" << std::endl;
        return -1;
    }

    std::unique_ptr<TileTextureManager> ttm = std::make_unique<TileTextureManager>();

    window->registerCallbacks(camera.get());
    std::unique_ptr<Game> game = std::make_unique<Game>(camera.get(), ttm.get());

    luaEngine = std::make_unique<LuaEngine>(game.get());
    luaEngine->initialize();
    luaEngine->loadScript("assets/scripts/control.lua");
    luaEngine->loadScript("data/core/data.lua");
    luaEngine->loadScript("data/base/data.lua");

    //TBD: Remove TextureAtlas all together in favour of some parser + TextureAtlasSystem
    std::unique_ptr<TextureAtlas> atlas = std::make_unique<TextureAtlas>();
    atlas->buildFromLua(luaEngine->getState(), "tile");
    //HACK: Just to test parsing
    atlas->buildFromLua(luaEngine->getState(), "furnace");
    atlas->buildFromLua(luaEngine->getState(), "transport-belt");
    atlas->buildFromLua(luaEngine->getState(), "inserter");
    luaEngine->callOnInit();
    ttm->loadFromLua(luaEngine->getState());
    TextureAtlasSystem::getInstance().bake();

    const double targetFrameTime = 1.0 / 60.0; // 60 FPS
    const int64_t freq = bx::getHPFrequency();

    // Main loop
    while (!window->shouldClose()) {
      //TBD: Proper main loop
        window->pollEvents();
        luaEngine->callOnTick(1.0 / 60.0);
       int64_t frameStart = bx::getHPCounter();
        renderer->beginFrame();
        luaEngine->getGame()->nauvis->draw(renderer->vbo, renderer->ibo, renderer->program);
        renderer->endFrame();
        int64_t frameEnd = bx::getHPCounter();
        double elapsedSec = double(frameEnd - frameStart) / double(freq);

        double remainingSec = targetFrameTime - elapsedSec;
        if (remainingSec > 0.0)
        {
            uint32_t toSleepMs = (uint32_t)(remainingSec * 1000.0);
            bx::sleep(toSleepMs); // sleeps in milliseconds
        }
    }

    //game->shutdown();
    luaEngine->shutdown();
    renderer->shutdown();
    window->destroy();

    std::cout << "Shutting down factorio-oss Engine..." << std::endl;
    return 0;
}
