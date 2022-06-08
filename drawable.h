#pragma once

#include "sprite.h"

#include <windows.h>

class Drawable {
public:
  virtual const short getWinX() const = 0;
  virtual const short getWinY() const = 0;

  // can be encapsulated in a new sprite class
  virtual Sprite *getSprite() const = 0;
};
