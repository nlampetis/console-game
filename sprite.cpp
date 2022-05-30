#include "sprite.h"


Sprite::Sprite(int wx, int wy, const CHAR_INFO& fillchar)
: x(wx), y(wy)
{
  spriteTable = new CHAR_INFO[wx*wy];
  for(int i = 0; i < wx*wy; ++i){
    spriteTable[i].Attributes = fillchar.Attributes;
    spriteTable[i].Char.AsciiChar = fillchar.Char.AsciiChar;
  }
}

Sprite::Sprite(int wx, int wy, CHAR_INFO* sprite)
: x(wx), y(wy), spriteTable(sprite)
{
  spriteTable = new CHAR_INFO[wx*wy];
  spriteTable = sprite;
  
}

Sprite::~Sprite(){
  delete[] spriteTable;
}

const int& Sprite::getx() const {
    return x;
}

const int& Sprite::gety() const {
    return y;
}

const CHAR_INFO* Sprite::geSpriteTable() const {
  return spriteTable;
}
