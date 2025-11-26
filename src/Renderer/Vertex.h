#pragma once
#include <bgfx/bgfx.h>

struct Vertex {
    float x, y;
    float u, v;

    static bgfx::VertexLayout init() {
        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
        bgfx::createVertexLayout(layout);
        return layout;
    }
};
