#pragma once

#include <windows.h>

#include "console.h"
#include "drawable.h"
#include "gameobject.h"

class Renderer {
private:
  Console *console;
  short x_offset, y_offset, x_max, y_max;

public:
  Renderer(Console *);
  Renderer(Console *, short xoff,short  yoff,short  xmax,short  ymax);
  virtual ~Renderer();

  void draw(const Drawable &);
  void draw(const GameObject &);
  void drawTile(CHAR_INFO chinfo, short x, short y);
  void updateDisplay();

  void writeStringToConsole(const COORD &, const std::string &);
  void writeStringCentered(const short &ypos, const std::string &str);

  void fillBackground(CHAR_INFO c, short x, short y, short width, short height);

  void fillBackground(CHAR_INFO *c, short x, short y, short widt, short height);

  void drawSprite(const Sprite *, short x, short y);
  void drawLine(short x1, short y1, short x2, short y2, CHAR_INFO chinfo);
  void drawTriangle(short x1, short y1, short x2, short y2, short x3, short y3,
                    CHAR_INFO chinfo);
  void drawCircle(short x1, short x2, short radius, CHAR_INFO chinfo);

  void drawRectBorder(short x1, short y1, short x2, short y2, WORD colors);
};
