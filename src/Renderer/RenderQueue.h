#pragma once
#include <vector>
#include <atomic>
#include <mutex>

#include <Camera/Camera.h>
#include <EntitiesPrototypes/EntitiesPrototypesTypes.h>
#include <Atlas/TextureAtlasSystem.h>

#include <bgfx/bgfx.h>
#include <bx/math.h>

struct RenderObject
{
    AtlasUV tex;
    //position and size
    float x, y;
    float w, h;
    float rotation;

    uint64_t state;
    RenderLayer layer;
};

class RenderQueue
{
public:
    const std::vector<RenderObject>& getFrontList() const;
    std::vector<RenderObject>& getBackList();
    void swapBuffers();

private:
    std::vector<RenderObject> m_lists[2];
    std::atomic<int> m_frontIndex{0};
    std::mutex m_backMutex;
};