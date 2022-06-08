#pragma once

#include "drawable.h"

class GameObject : public Drawable{
  private:
  public:
    GameObject();
    virtual ~GameObject();
    virtual const short& getx() const = 0;
    virtual const short& gety() const = 0;
    virtual bool isPassable() = 0;
    virtual void onUpdate() = 0;
};

