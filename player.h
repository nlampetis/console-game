#pragma once

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>

#include "console.h"
#include "drawable.h"

class Player : public Drawable {
private:
  int x;
  int y;
  CHAR_INFO *chInfo;
  float internalTimeDelta;

public:
  Player();
  Player(int, int);
  virtual ~Player();

  const int &getX() const;
  const int &getY() const;

  void setX(int &newx);
  void setY(int &newy);

  void setPos(COORD newpos);

  // WCHAR getChar() const;
  // WORD GetAttr() const;

  COORD getPos() const;
  short getWidth() const;
  short getHeight() const;
  CHAR_INFO *getSprite() const;

  // const CHAR_INFO& getPlayerCharInfo() const;

  void move(const char &);
  // void draw(Console&);

  void incrementDelta(float toAddelta);
  void automove1s();
};
