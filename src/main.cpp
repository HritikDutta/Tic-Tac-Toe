#include <glad/glad.h>
#include "platform/application.h"
#include "platform/clargs.h"
#include "game/ttt.h"

Game game;

int main(void)
{
    Application app("Tic Tac Toe", 400, 400, false, false);
    app.SetWindowIcon("res/icons/icon.png");
    app.SetVsync(true);

    app.onInit = [](Application* app)
    {
        game.Init(app);
    };

    app.onUpdate = [](Application* app)
    {
        if (app->GetKeyDown(KEY(ESCAPE)))
            game.SetPause(!game.IsPaused());

        game.Update();
    };

    app.onRender = [](Application* app)
    {
        game.Render(app);
    };

    app.Run();
}