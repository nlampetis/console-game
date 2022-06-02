#pragma once

#include <windows.h>

#include "console.h"
#include "drawable.h"

class Renderer {
private:
  Console *console;

public:
  Renderer(Console *);
  virtual ~Renderer();
  void draw(const Drawable &);
  void updateDisplay();
  void writeStringToConsole(const COORD &, const std::string &);
  void writeStringCentered(const short &ypos, const std::string &str);

  void drawLine(short x1, short y1, short x2, short y2, CHAR_INFO chinfo);
  void drawTriangle(short x1, short y1, short x2, short y2, short x3, short y3, CHAR_INFO chinfo);
  void drawCircle(short x1, short x2, short radius, CHAR_INFO chinfo);
};

