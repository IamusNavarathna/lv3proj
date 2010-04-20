#ifndef TILE_H
#define TILE_H

#include "Anim.h"

enum TileType
{
    TILETYPE_STATIC,
    TILETYPE_ANIMATED
};


// TODO: this has to be extended & implemented. Add TileClass enum with custom values
enum TileFlags
{
    TILEFLAG_DEFAULT = 0x00, // autodetect colliding pixels
    TILEFLAG_SOLID   = 0x01, // the whole tile is solid, even if there are transparent pixels
};

union TileInfo
{
    struct 
    {
        uint8 flags;
        uint8 id;
    };
    uint16 raw;
};


// the basic tile. static by default, but can be overloaded to support animation
struct BasicTile
{
public:
    BasicTile() : surface(NULL), type(TILETYPE_STATIC) {}
    std::string filename;
    SDL_Surface *surface; // surface to be drawn
    uint8 type; // read-only!!

};


struct AnimatedTile : BasicTile
{
public:
    AnimatedTile();
    AnimatedTile(Anim *a, uint32 idx = 0, const char *startwith = NULL);
    Anim *ani;
    AnimFrame *curFrame;
    AnimFrameStore *curFrameStore;
    uint32 nextupdate; // the time when this tile will change its texture (Engine::GetCurrentFrameTime() + X)

    void SetupDefaults(uint32 idx = 0, const char *startwith = NULL);
    void SetName(char *name);
    void SetFrame(uint32 frame);
    const char *GetName(void) { return curFrameStore->name.c_str(); }
    uint32 GetFrame(void) { return curFrame->index; }
    void Init(uint32 curtime); // current system clock
    void Update(uint32 curtime);

};

#endif
