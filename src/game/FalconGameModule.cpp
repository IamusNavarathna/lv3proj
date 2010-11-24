#include <stdarg.h>
#include <falcon/engine.h>
#include "common.h"
#include "GameEngine.h"
#include "AppFalconGame.h"
#include "PhysicsSystem.h"


#include "FalconBaseModule.h"
#include "FalconObjectModule.h"
#include "FalconGameModule.h"


class fal_TileLayer;

GameEngine *g_engine_ptr = NULL;

void FalconGameModule_SetEnginePtr(Engine *e)
{
    g_engine_ptr = (GameEngine*)e;
}


FALCON_FUNC fal_Physics_SetGravity(Falcon::VMachine *vm)
{
    FALCON_REQUIRE_PARAMS_EXTRA(1, "N");
    g_engine_ptr->physmgr->envPhys.gravity = float(vm->param(0)->forceNumeric());
}

FALCON_FUNC fal_Physics_GetGravity(Falcon::VMachine *vm)
{
    vm->retval(g_engine_ptr->physmgr->envPhys.gravity);
}


Falcon::Module *FalconGameModule_create(void)
{
    Falcon::Module *m = new Falcon::Module;
    m->name("GameModule");

    Falcon::Symbol *symPhysics = m->addSingleton("Physics");
    Falcon::Symbol *clsPhysics = symPhysics->getInstance();
    m->addClassMethod(clsPhysics, "SetGravity", fal_Physics_SetGravity);
    m->addClassMethod(clsPhysics, "GetGravity", fal_Physics_GetGravity);

    return m;
};


