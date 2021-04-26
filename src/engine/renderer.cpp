#include "renderer.h"

#include <vector>
#include <glad/glad.h>
#include <stb/stb_image.h>
#include "universal/types.h"
#include "platform/application.h"
#include "shader.h"

void SpriteAtlas::Load(const char filepath[])
{
    Byte* pixels = stbi_load(filepath, &width, &height, &channels, 4);

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    stbi_image_free(pixels);
}

void SpriteAtlas::Bind(int index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, id);
    activeIndex = index;
}

void Sprite::Set(SpriteAtlas* a, Vec2 s, Vec4 tc, Shader sh)
{
    atlas = a;
    size = s;
    texCoord = tc;
    shader = sh;
}

void AnimatedSprite::Set(SpriteAtlas* a, Vec2 s, f32 fr, Shader sh)
{
    atlas = a;
    size = s;
    frameRate = fr;
    shader = sh;
}

void AnimatedSprite::AddFrame(Vec4 tc)
{
    texCoords.push_back(tc);
}

void Renderer::Render(Application* app, Sprite& sprite, Vec4 position)
{
    sprite.shader.Bind();
}

void Renderer::Render(Application* app, AnimatedSprite& animatedSprite, Vec4 position)
{

}