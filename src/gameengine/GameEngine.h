#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "Engine.h"
#include "Objects.h"

class AppFalconGame;
class ObjectMgr;
class PhysicsMgr;


class GameEngine : public Engine
{
public:
    GameEngine();
    virtual ~GameEngine();

    inline static GameEngine *GetInstance(void) { return (GameEngine*)Engine::GetInstance(); }

    virtual void OnMouseEvent(uint32 type, uint32 button, uint32 state, uint32 x, uint32 y, int32 rx, int32 ry);
    virtual void OnKeyDown(SDLKey key, SDLMod mod);
    virtual void OnKeyUp(SDLKey key, SDLMod mod);
    virtual void OnJoystickEvent(uint32 type, uint32 device, uint32 id, int32 val);
    //virtual void OnWindowEvent(bool active);
    virtual void OnObjectCreated(BaseObject *obj);
    virtual const char *GetName(void) { return "game"; } // must be overloaded

    virtual bool Setup(void);
    virtual void Shutdown(void);


protected:

    virtual void _Process(void);
    virtual void _Render(void);
    virtual void _PostRender(void);
    virtual void _Reset(void);
    virtual bool _InitFalcon(void);
    virtual void _Idle(uint32 ms);

    bool _wasInit;
};

#endif
