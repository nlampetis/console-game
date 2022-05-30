#pragma once

#include <windows.h>
#include "sprite.h"

class Drawable
{
public:
    
    virtual COORD getPos() const  = 0;
    //virtual WCHAR getChar() const = 0;
    //virtual WORD GetAttr() const = 0;

    //can be encapsulated in a new sprite class
    virtual Sprite * getSprite() const = 0;
    virtual short getWidth() const = 0;
    virtual short getHeight() const = 0;

};
