#include "ui.h"

#include <cstdlib>
#include <string>
#include <vector>
#include <stb/stb_truetype.h>
#include <glad/glad.h>
#include "universal/types.h"
#include "platform/fileio.h"
#include "shader.h"

namespace UI {

static struct {
    u32 rectVAO;
    Shader rectShader;
    Shader fontShader;
    ID hot, active;
} data;

static ID invalid { -1, -1 };

bool ID::operator==(const ID& other) const
{
    return primary == other.primary && secondary == other.secondary;
}

bool ID::operator!=(const ID& other) const
{
    return primary != other.primary || secondary != other.secondary;
}

void Init()
{
    static bool initialized = false;

    if (initialized)
        return;

    glGenVertexArrays(1, &data.rectVAO);

    data.rectShader.LoadShader("res/shaders/rect.vert", Shader::Type::VERTEX_SHADER);
    data.rectShader.LoadShader("res/shaders/rect.frag", Shader::Type::FRAGMENT_SHADER);
    data.rectShader.Compile();

    data.fontShader.LoadShader("res/shaders/font.vert", Shader::Type::VERTEX_SHADER);
    data.fontShader.LoadShader("res/shaders/font.frag", Shader::Type::FRAGMENT_SHADER);
    data.fontShader.Compile();

    initialized = true;
}

void Font::Load(const char filepath[], f32 height)
{
    auto contents = LoadBinaryFile(filepath);

    Byte* bitmap = (Byte*) malloc(512 * 512);
    stbtt_BakeFontBitmap(contents.data(), 0, height, bitmap, 512, 512, ' ', 128 - ' ', charData);

    glGenTextures(1, &bitmapTexID);
    glBindTexture(GL_TEXTURE_2D, bitmapTexID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);

    bitmapWidth = bitmapHeight = 512;
    fontHeight  = height;

    free(bitmap);
}

void Font::Free()
{
    glDeleteTextures(1, &bitmapTexID);
}

Vec2 GetRenderedTextSize(const std::string& text, const Font& font)
{
    Vec2 size { 0.0f, font.fontHeight * 0.75f };
    f32 lineX = 0.0f;

    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] == '\n')
        {
            size.y += font.fontHeight;
            size.x = std::max(lineX, size.x);
            lineX = 0.0f;
            continue;
        }

        stbtt_aligned_quad quad;
        stbtt_GetBakedQuad(font.charData, font.bitmapWidth, font.bitmapHeight, text[i] - ' ',
                           &lineX, &size.y, &quad, 1);
    }

    size.x = std::max(lineX, size.x);
    return size;
}

void RenderText(Application* app, const std::string& text, const Font& font,
                Vec4 color, Vec2 topLeft, f32 layer)
{
    std::vector<Vec4> vertices(text.length() * 4);
    int verticesFilled = 0;
    std::vector<u32>  indices(text.length() * 6);
    int indicesFilled  = 0;

    Vec2 position = topLeft;
    position.y += font.fontHeight * 0.75f;

    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] == '\n')
        {
            position.y += font.fontHeight;
            position.x = topLeft.x;
            continue;
        }

        stbtt_aligned_quad quad;
        stbtt_GetBakedQuad(font.charData, font.bitmapWidth, font.bitmapHeight, text[i] - ' ',
                           &position.x, &position.y, &quad, 1);

        f32 top    = 1.0f - 2.0f * (quad.y0 / app->refScreenHeight);
        f32 left   = 2.0f * (quad.x0 / app->refScreenWidth) - 1.0f;
        f32 right  = 2.0f * (quad.x1 / app->refScreenWidth) - 1.0f;
        f32 bottom = 1.0f - 2.0f * (quad.y1 / app->refScreenHeight);

        vertices[verticesFilled + 0] = Vec4 {  left, bottom, quad.s0, quad.t1 };
        vertices[verticesFilled + 1] = Vec4 { right, bottom, quad.s1, quad.t1 };
        vertices[verticesFilled + 2] = Vec4 { right,    top, quad.s1, quad.t0 };
        vertices[verticesFilled + 3] = Vec4 {  left,    top, quad.s0, quad.t0 };

        indices[indicesFilled + 0] = verticesFilled + 0;
        indices[indicesFilled + 1] = verticesFilled + 1;
        indices[indicesFilled + 2] = verticesFilled + 2;

        indices[indicesFilled + 3] = verticesFilled + 2;
        indices[indicesFilled + 4] = verticesFilled + 3;
        indices[indicesFilled + 5] = verticesFilled + 0;

        verticesFilled += 4;
        indicesFilled  += 6;
    }

    u32 vao, vbo, ibo;

    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glCreateBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec4) * verticesFilled, vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4), (void*) 0);

    glCreateBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * indicesFilled, indices.data(), GL_STATIC_DRAW);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font.bitmapTexID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    data.fontShader.Bind();
    data.fontShader.SetUniform1i("u_font",  0);
    data.fontShader.SetUniform4f("u_color", color.r, color.g, color.b, color.a);
    data.fontShader.SetUniform1f("u_layer", layer);

    glDrawElements(GL_TRIANGLES, indicesFilled, GL_UNSIGNED_INT, NULL);

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);
}

void RenderRect(Application* app, const Rect& rect, Vec4 color, f32 layer)
{
    f32 top   = 1.0f - 2.0f * (rect.topLeft.y / app->refScreenHeight);
    f32 left  = 2.0f * (rect.topLeft.x / app->refScreenWidth) - 1.0f;

    f32 sizeX = 2.0f * (rect.size.x / app->refScreenWidth);
    f32 sizeY = 2.0f * (rect.size.y / app->refScreenHeight);

    data.rectShader.Bind();
    data.rectShader.SetUniform2f("u_topLeft", left, top);
    data.rectShader.SetUniform2f("u_size",    sizeX, sizeY);
    data.rectShader.SetUniform1f("u_layer",   layer);
    data.rectShader.SetUniform4f("u_color",   color.r, color.g, color.b, color.a);

    glBindVertexArray(data.rectVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

bool RenderButton(Application* app, ID id, const Rect& rect, Vec4 defaultColor,
                  Vec4 hoverColor, Vec4 pressedColor, f32 layer)
{
    bool result = false;

    Vec4 color = defaultColor;
    if (app->mouseX >= rect.topLeft.x && app->mouseX <= rect.topLeft.x + rect.size.x &&
        app->mouseY >= rect.topLeft.y && app->mouseY <= rect.topLeft.y + rect.size.y)
    {
        if (data.hot != id)
            data.hot = id;

        if (app->GetMouseButton(MOUSE(1)))
        {
            result = data.active != id;
            data.active = id;
            color = pressedColor;
        }
        else
        {
            data.active = UIInvalid();
            color = hoverColor;
        }
    }
    else
    {
        if (data.hot == id)
            data.hot = UIInvalid();
    }

    RenderRect(app, rect, color, layer);

    return result;
}


bool RenderTextButton(Application* app, ID id, const std::string& text, const Font& font,
                    Vec2 padding, Vec2 topLeft, f32 layer)
{
    Vec2 size = GetRenderedTextSize(text, font);
    Rect btnRect;
    btnRect.size = size + (padding * 2.0f);
    btnRect.topLeft = topLeft;

    bool res = RenderButton(app, id, btnRect,
                            { 0.8f, 0.8f, 0.8f, 1.0f },
                            { 1.0f, 1.0f, 1.0f, 1.0f },
                            { 0.6f, 0.6f, 0.6f, 1.0f },
                            layer + 0.01f);

    RenderText(app, text, font, { 0.2f, 0.2f, 0.2f, 1.0f },
               topLeft + padding, layer);

    return res;
}
}