#include "common.h"
#include "Objects.h"

BaseObject::BaseObject()
{
    _falObj = NULL;
    _id = 0;
}

void ActiveRect::Init(void)
{
    type = OBJTYPE_RECT;
}

uint8 ActiveRect::CollisionWith(ActiveRect *other)
{
    return false;
}

void ActiveRect::AlignToSideOf(ActiveRect *other, uint8 side)
{
}

void Object::Init(void)
{
    type = OBJTYPE_OBJECT;
    memset(&phys, 0, sizeof(PhysProps)); // TODO: apply some useful default values
    _physicsAffected = false;
}

void Object::SetBBox(uint32 x_, uint32 y_, uint32 w_, uint32 h_)
{
    this->x = x_;
    this->y = y_;
    this->w = w_;
    this->h = h_;
}

void Item::Init(void)
{
    type = OBJTYPE_ITEM;
}

void Unit::Init(void)
{
    type = OBJTYPE_UNIT;
}

void Unit::SetBBox(uint32 x_, uint32 y_, uint32 w_, uint32 h_)
{
    Object::SetBBox(x_,y_,w_,h_);
    anchor.x = (x_ + w_) / 2;
    anchor.y = y_ + h_;
}

void Player::Init(void)
{
    type = OBJTYPE_PLAYER;
}
