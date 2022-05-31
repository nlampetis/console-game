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

const int& Sprite::height() const {
    return y;
}

const int& Sprite::width() const {
    return x;
}

const CHAR_INFO* Sprite::geSpriteTable() const {
  return spriteTable;
}

void Sprite::changePixel(int xx, int yy, CHAR_INFO ch){
  spriteTable[(yy)*x + xx] = ch;
}
