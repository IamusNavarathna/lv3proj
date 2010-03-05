#ifndef OBJECTS_H
#define OBJECTS_H

#include <SDL/SDL.h>
#include <falcon/engine.h>
#include "SharedStructs.h"

/*
 * NOTE: The OnEnter(), OnLeave(), OnWhatever() functions are defined in FalconGameModule.cpp !!
 */

class Object;
class ObjectMgr;
class BaseObject;
class FalconProxyObject;

enum ObjectType
{
    OBJTYPE_RECT    = 0,
    OBJTYPE_OBJECT  = 1,
    OBJTYPE_ITEM    = 2,
    OBJTYPE_UNIT    = 3,
    OBJTYPE_PLAYER  = 4
};

class BaseObject
{
    friend class ObjectMgr;

public:
    BaseObject();
    virtual void Init(void) = 0;
    FalconProxyObject *_falObj;
    void unbind(void); // clears bindings from falcon, should be called before deletion
    inline uint32 GetId(void) { return _id; }
    inline uint8 GetType(void) { return type; }

protected:
    uint32 _id;
    uint8 type;
};

// the base of everything
class ActiveRect : public BaseObject
{
public:
    virtual void Init(void);

    virtual void OnEnter(uint8 side, Object *who);
    virtual void OnLeave(uint8 side, Object *who);
    virtual void OnTouch(uint8 side, Object *who);

    int x, y, w, h;

    /*ActiveRect( int nx=0, int ny=0, int nw=0, int nh=0 ):
    x(nx), y(ny), w(nw), h(nh)
    {}*/

    // a Copy constructor
    /*ActiveRect( const ActiveRect& r ):
    x(r.x), y(r.y), w(r.w), h(r.h)
    {}*/

    // Method to calculate the second X corner
    inline int x2() const { return x+w; }
    // Method to calculate the second Y corner
    inline int y2() const { return y+h; }

    inline SDL_Rect AsSDLRect(void)
    {
        SDL_Rect rect;
        rect.x = x;
        rect.y = y;
        rect.w = w;
        rect.h = h;
        return rect;
    }

};


// a normal "Sprite" object
class Object : public ActiveRect
{
public:
    virtual void Init(void);
    virtual void SetBBox(uint32 x, uint32 y, uint32 w, uint32 h);

    virtual void OnUpdate(uint32 ms);

};

// an item a player carries around in the inventory
class Item : public Object
{
public:
    virtual void Init(void);

    virtual void OnUse(Object *who);
};

// unit, most likely some NPC, enemy, or player
class Unit : public Object
{
public:
    virtual void Init(void);
    virtual void SetBBox(uint32 x, uint32 y, uint32 w, uint32 h);

protected:
    Point anchor; // where this unit stands on the ground (center of object)

};

// the Player class, specialized for playable characters like these vikings this is all about
class Player : public Unit
{
public:
    virtual void Init(void);

};

#endif
