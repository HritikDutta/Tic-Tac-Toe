#include "ttt.h"

#include <cstring>
#include <string>
#include "universal/types.h"
#include "platform/application.h"
#include "engine/shader.h"
#include "engine/sprite.h"
#include "engine/ui.h"

static struct
{
    std::string text;
    bool isPaused;
    bool isEndScreen;
} pauseData;


void Game::Init(Application* app)
{
    font.Load("res/fonts/Inconsolata.ttf", 24.0f);

    const f32 boardSize = (f32) app->refScreenHeight - 100.0f;
    const f32 cellSize  = boardSize / 3.0f;

    spriteShader.LoadShader("res/shaders/sprite.vert", Shader::Type::VERTEX_SHADER);
    spriteShader.LoadShader("res/shaders/sprite.frag", Shader::Type::FRAGMENT_SHADER);
    spriteShader.Compile();

    atlas.Load("res/images/atlas.png");
    sprites[0].Set({ cellSize, cellSize }, { 0.0f, 0.0f, 0.5f, 1.0f });
    sprites[1].Set({ cellSize, cellSize }, { 0.0f, 0.5f, 1.0f, 1.0f });

    Reset();
}

void Game::Reset()
{
    for (int i = 0; i < 9; i++)
        board[i] = CellElement::EMPTY;
    playerScores[0] = 0;
    playerScores[1] = 0;
    playerIndex = 0;

    pauseData.isPaused = false;
    pauseData.isEndScreen = false;
    pauseData.text = "Game Paused...";
}

void Game::NextRound()
{
    for (int i = 0; i < 9; i++)
        board[i] = CellElement::EMPTY;

    pauseData.isPaused = false;
    pauseData.isEndScreen = false;
    pauseData.text = "Game Paused...";
}

bool Game::IsPaused()
{
    return pauseData.isPaused;
}

void Game::SetPause(bool value)
{
    if (pauseData.isEndScreen)
        NextRound();
    else
        pauseData.isPaused = value;
}

void Game::Render(Application* app)
{
    static const f32 boardSize = (f32) app->refScreenHeight - 100.0f;
    static const f32 cellSize  = boardSize / 3.0f;

    static Vec4 playerColors[] = {
        { 1.0f, 0.7f, 0.7f, 1.0f }, // Highlighted Cross
        { 0.7f, 0.7f, 1.0f, 1.0f }, // Highlighted Circle
    };

    {   // Title
        std::string title = "Tic Tac Toe";
        Vec2 size = UI::GetRenderedTextSize(title, font);
        Vec2 position = { (app->refScreenWidth - boardSize) / 2.0f, 10.0 };
        UI::RenderText(app, title, font, { 1.0f, 1.0f, 1.0f, 1.0f },
                        position, 0.0f);
    }

    {   // Board
        static Vec4 colors[] = {
            { 1.0f, 0.4f, 0.5f, 1.0f }, // Cross
            { 0.5f, 0.4f, 1.0f, 1.0f }, // Circle
            { 1.0f, 1.0f, 1.0f, 1.0f }, // Empty
        };

        f32 xOffset = (app->refScreenWidth - boardSize) / 2.0f;
        f32 yOffset = 50.0f;

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                f32 y = (f32) app->refScreenHeight - ((f32) (i + 1)) * cellSize - yOffset;
                f32 x = j * cellSize + xOffset;

                UI::Rect r;
                r.topLeft = { x, y };
                r.size    = { cellSize, cellSize };

                if (!pauseData.isPaused && board[i * 3 + j] == CellElement::EMPTY)
                {
                    Vec4 color = colors[(int) board[i * 3 + j]];
                    if (UI::RenderButton(app, GenUIIDWithSec(i + j), r,
                                            color, playerColors[playerIndex], colors[playerIndex],
                                            0.0f))
                    {
                        PlaceElement(i * 3 + j);
                    }
                }
                else
                {
                    UI::RenderRect(app, r, { 1.0f, 1.0f, 1.0f, 1.0f }, 0.0f);
                }
            }

        // @Todo: Inefficient and clunky
        {   // Draw all sprites
            for (int i = 0; i < 9; i++)
            {
                if (board[i] != CellElement::EMPTY)
                {
                    atlas.Bind(0);

                    spriteShader.Bind();
                    spriteShader.SetUniform1i("u_atlas", 0);

                    f32 scale = cellSize / (f32) atlas.height;
                    f32 x = 2.0f * ((i % 3) - 1) * cellSize / app->refScreenWidth;
                    f32 y = 2.0f * ((i / 3) - 1) * cellSize / app->refScreenHeight;
                    Mat4 mat = Mat4::Scaling({ scale, scale, 1.0f }).Translate({ x, y, -0.01f });
                    spriteShader.SetUniformMat4("u_mat", false, mat);

                    sprites[(int) board[i]].Draw();
                }
            }
        }
    }

    {   // Player Scores
        char buffer[4];

        Vec2 right = { (app->refScreenWidth + boardSize) / 2.0f , 10.0f };
        int numPlayers = sizeof(playerScores) / sizeof(playerScores[0]);        

        for (int i = numPlayers - 1; i >= 0; i--)
        {
            sprintf(buffer, "%d", playerScores[i]);
            Vec2 topLeft = right - Vec2 { UI::GetRenderedTextSize(buffer, font).x, 0.0f };
            UI::RenderText(app, buffer, font, playerColors[i],
                           topLeft, 0.0f);

            right.x = topLeft.x - 10.0f;
        }
    }

    {   // Reset Button
        std::string resetText = "Reset";
        Vec2 size = UI::GetRenderedTextSize(resetText, font);
        Vec2 topLeft = { (app->refScreenWidth - (size.x + 20.0f)) / 2.0f, app->refScreenHeight - ((50.0f + size.y + 10.0f) / 2.0f) };
        if (UI::RenderTextButton(app, GenUIID(), resetText, font,
                                 { 10.0f, 5.0f }, topLeft, 0.0f))
        {
            Reset();
        }
    }

    {
        if (pauseData.isPaused)
        {
            f32 y = (f32) app->refScreenHeight - boardSize - 50.0f;
            f32 x = (app->refScreenWidth - boardSize) / 2.0f;

            UI::Rect rect;
            rect.topLeft = { x, y };
            rect.size = { 3 * cellSize, 3 * cellSize };
            UI::RenderRect(app, rect, { 0.0f, 0.0f, 0.0f, 0.7f }, -0.02f);

            {   // Pause Text
                Vec2 textSize = UI::GetRenderedTextSize(pauseData.text, font);
                Vec2 textTopLeft = rect.topLeft + Vec2 { (boardSize - textSize.x) / 2.0f, (boardSize - textSize.y) / 2.0f };
                UI::RenderText(app, pauseData.text, font, { 1.0f, 1.0f, 1.0f, 1.0f }, textTopLeft, -0.03f);
            }

            { // Continue Button
                std::string continueText = "Press Esc To Continue";
                Vec2 size = UI::GetRenderedTextSize(continueText, font);
                Vec2 topLeft = rect.topLeft + Vec2 { (boardSize - size.x) / 2.0f , boardSize - (size.y + font.fontHeight) };
                UI::RenderText(app, continueText, font, { 1.0f, 1.0f, 1.0f, 1.0f }, topLeft, -0.06);
            }
        }
    }
}

void Game::PlaceElement(int index)
{
    if (board[index] == CellElement::EMPTY)
    {
        board[index] = (CellElement) playerIndex;
        
        if (PlayerWon())
        {
            char buffer[32];
            sprintf(buffer, "Player %d Wins!", playerIndex + 1);
            pauseData.text     = buffer;
            pauseData.isPaused = pauseData.isEndScreen = true;
            playerScores[playerIndex]++;
        }
        else if (IsDraw())
        {
            pauseData.text     = "Draw...";
            pauseData.isPaused = pauseData.isEndScreen = true;
        }

        playerIndex = 1 - playerIndex;
    }
}

bool Game::IsDraw()
{
    for (int i = 0; i < 9; i++)
    {
        if (board[i] == CellElement::EMPTY)
            return false;
    }

    return true;
}

bool Game::PlayerWon()
{
    static const int winLines[8][3] = {
        // Horizontal
        { 0, 1, 2 },
        { 3, 4, 5 },
        { 6, 7, 8 },

        // Vertical
        { 0, 3, 6 },
        { 1, 4, 7 },
        { 2, 5, 8 },

        // Diagonal
        { 0, 4, 8 },
        { 2, 5, 6 },
    };

    CellElement currElem = (CellElement) playerIndex;

    for (int i = 0; i < 8; i++)
    {
        bool complete = true;
        for (int j = 0; j < 3; j++)
        {
            if (board[winLines[i][j]] != currElem)
            {
                complete = false;
                break;
            }
        }

        if (complete)
            return true;
    }

    return false;
}