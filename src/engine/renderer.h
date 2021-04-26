#pragma once

#include <vector>
#include "universal/types.h"
#include "platform/application.h"
#include "shader.h"

struct SpriteAtlas
{
    void Load(const char filepath[]);
    void Bind(int index = 0);
    
    u32 id;
    s32 width, height, channels;
    int activeIndex;
};

struct Sprite
{
    void Set(SpriteAtlas* a, Vec2 s, Vec4 tc, Shader sh);

    SpriteAtlas* atlas;
    Vec2 size;
    Vec4 texCoord;
    Shader shader;
};

struct AnimatedSprite
{
    void Set(SpriteAtlas* a, Vec2 s, f32 fr, Shader sh);
    void AddFrame(Vec4 tc);

    SpriteAtlas* atlas;
    Vec2 size;
    Shader shader;
    f32 frameRate;
    std::vector<Vec4> texCoords;
};

struct Renderer
{
    static void Render(Application* app, Sprite& sprite, Vec4 position);
    static void Render(Application* app, AnimatedSprite& animatedSprite, Vec4 position);
};