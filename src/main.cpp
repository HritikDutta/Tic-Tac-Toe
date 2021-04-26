#include <glad/glad.h>
#include "platform/application.h"
#include "platform/clargs.h"
#include "game/ttt.h"

// #define DEBUG

Game game;

#ifdef DEBUG
int main(void)
{
    int argc = 2;
    const char* argv[] = {
        "game",
        "-debug"
    };

#else
int main(int argc, const char* argv[])
{
#endif
    CL_Args args = ParseCommandLineArguments(argc, argv);

    Application app("Tic Tac Toe", 400, 400, args.fullscreen, args.debug);
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

        if (app->GetKeyDown(KEY(Q)))
            game.playerScores[0]++;

        if (app->GetKeyDown(KEY(E)))
            game.playerScores[1]++;
    };

    app.onRender = [](Application* app)
    {
        game.Render(app);
    };

    app.Run();
}