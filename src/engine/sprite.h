#pragma once

#include <vector>
#include "universal/types.h"
#include "shader.h"

struct SpriteAtlas
{
    s32 width, height;
    u32 id;

    void Load(const char* filepath);
    void Bind(u32 activeTextureIndex);
    void Free();
};

struct Sprite
{
    Vec2 size;
    Vec4 uvs;
    u32  vao, ibo;

    void Set(Vec2 pixelSize, Vec4 texCoords);

    // All the shader uniforms have to be set before calling this
    // The sprite atlas must also be bound before calling this
    void Draw();
};