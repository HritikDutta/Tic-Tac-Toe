#pragma once

#include "platform/application.h"
#include "engine/ui.h"
#include "engine/shader.h"
#include "engine/sprite.h"

enum class CellElement
{
    CROSS,
    CIRCLE,
    EMPTY,
};

struct Game
{
    UI::Font font;
    SpriteAtlas atlas;
    Sprite sprites[2];
    Shader spriteShader;

    CellElement board[9];
    int playerScores[2];
    int playerIndex;
    bool vsComputer;

    void Init(Application* app);
    void Reset();
    void NextRound();
    bool IsPaused();
    void SetPause(bool value);

    bool IsDraw();
    bool PlayerWon();

    void Update();
    void Render(Application* app);
    void DrawCell(Application* app, int i, int j);

    void PlaceElement(int index);
    void PlaceElementComp();
};