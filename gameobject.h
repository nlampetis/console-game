#pragma once

#include "game.h"
#include "drawable.h"

class Game;

class GameObject : public Drawable{
  private:
    short x;
    short y;
  public:

    GameObject();
    virtual ~GameObject();
    

    virtual short& getx() = 0;
    virtual short& gety() = 0;

    
};

