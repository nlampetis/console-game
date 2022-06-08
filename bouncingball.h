#pragma once

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <math.h>

#include "console.h"

struct myCoord {
  float x;
  float y;
};

class Bouncy {
private:
  myCoord pos;
  myCoord vel;
  CHAR_INFO chInfo;

public:
  Bouncy();
  virtual ~Bouncy();

  const myCoord &getVel() const;
  const myCoord &getPos() const;
  void setVel(float &newx, float &newy);
  void setPos(float &newx, float &newy);
  const CHAR_INFO &getBouncyCharInfo() const;

  void move(Console &, float delta);
  void draw(Console &);
};
