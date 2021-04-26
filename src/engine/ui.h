#pragma once

#include <string>
#include <stb/stb_truetype.h>
#include "platform/application.h"
#include "universal/types.h"
#include "shader.h"

namespace UI {

void Init();

struct ID
{
    s32 primary;
    s32 secondary;

    bool operator==(const ID& other) const;
    bool operator!=(const ID& other) const;
};

struct Font
{
    GLuint bitmapTexID;
    u32 bitmapWidth, bitmapHeight;
    f32 fontHeight;

    stbtt_bakedchar charData[128 - ' '];

    void Load(const char filepath[], f32 height);
    void Free();
};

struct Rect
{
    Vec2 topLeft;
    Vec2 size;
};

struct Image
{
    u8* pixels;
    s32 width, height, bitsPP;
};

Vec2 GetRenderedTextSize(const std::string& text, const Font& font);
void RenderText(Application* app, const std::string& text, const Font& font,
                Vec4 color, Vec2 topLeft, f32 layer);
void RenderRect(Application* app, const Rect& rect, Vec4 color, f32 layer);
bool RenderButton(Application* app, ID id, const Rect& rect, Vec4 defaultColor,
                  Vec4 hoverColor, Vec4 pressedColor, f32 layer);

// Using default colors
bool RenderTextButton(Application* app, ID id, const std::string& text, const Font& font,
                    Vec2 padding, Vec2 topLeft, f32 layer);
}

#define GenUIID() (UI::ID { __LINE__, 0 })
#define GenUIIDWithSec(x) (UI::ID { __LINE__, (x) })
#define UIInvalid() (UI::ID { -1, -1 })