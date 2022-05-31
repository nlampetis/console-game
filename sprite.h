#pragma once

#include <windows.h>

class Sprite{
    private:
      CHAR_INFO * spriteTable;

      int x;
      int y;

    public:
      Sprite(int x, int y, const CHAR_INFO& fillchar);
      Sprite(int x, int y, CHAR_INFO * spriteTable);

      virtual ~Sprite();
      //TODO: implement load from file


      const int &width() const;
      const int &height() const;

      const CHAR_INFO * geSpriteTable() const;

      void changePixel(int x, int y, CHAR_INFO newchinfo);

};
