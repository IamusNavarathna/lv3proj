#include <SDL/SDL.h>
#include "common.h"

#include "EditorEngine.h"


int main(int argc, char *argv[])
{
    uint32 loglevel = 1;
    DEBUG(loglevel = 3);
    log_setloglevel(loglevel);

    EditorEngine::RelocateWorkingDir(); // a pity that this has to be done before opening the log file ...

    log_prepare("editor_log.txt", "w");

    EditorEngine::PrintSystemSpecs();

    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    atexit(log_close);
    atexit(SDL_Quit);

    mtRandSeed(time(NULL));

    EditorEngine editor;
    editor.HookSignals();

    try
    {
        editor.InitScreen(320,240,0,SDL_RESIZABLE);
        editor.SetTitle("Lost Vikings 3 Project - Level Editor");
        if(!editor.Setup())
        {
            logerror("Failed to setup editor. Exiting.");
            editor.UnhookSignals();
            return 1;
        }
        editor.Run();
        
    }
    catch(gcn::Exception ex)
    {
        editor.UnhookSignals();
        logerror("An unhandled gcn::Exception occurred! Infos:");
        logerror("File: %s:%u", ex.getFilename().c_str(), ex.getLine());
        logerror("Function: %s", ex.getFunction().c_str());
        logerror("Message: %s", ex.getMessage().c_str());
    }

    editor.UnhookSignals();
    editor.Shutdown();

    return 0;
}
