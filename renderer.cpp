#include "renderer.h"
#include "sprite.h"
#include "game.h"

#include <cmath>
#include <type_traits>
#include <wincon.h>

Renderer::Renderer(Console *cl) { 
  console = cl; 
  x_offset = 0;
  y_offset = 0;
  x_max = Console::WINDOW_WIDTH;
  y_max = Console::WINDOW_HEIGHT;
}

Renderer::Renderer(Console *cl, short xoff, short yoff, short xmax, short ymax)
: x_offset(xoff), y_offset(yoff), x_max(x_offset + xmax - 1), y_max(y_offset + ymax - 1)
{
  console =cl;
}

Renderer::~Renderer() {}

//TODO fix to relative drawing
void Renderer::draw(const Drawable &drawable) {

  Sprite *sprite = drawable.getSprite();

  short startX = drawable.getWinX();
  short startY = drawable.getWinY();
  short width = sprite->width();
  short height = sprite->height();
  short endX = width + startX;
  short endY = height + startY;

  for (short i = startX; i < endX; ++i) {
    for (short j = startY; j < endY; ++j) {
      COORD currentXY = {i, j};
      if (sprite->geSpriteTable()[(j - startY) * width + i - startX]
              .Char.AsciiChar == 0x0000)
        continue;
      
      console->updateBuffer(
          currentXY,
          sprite->geSpriteTable()[(j - startY) * width + i - startX]
              .Char.AsciiChar,
          sprite->geSpriteTable()[(j - startY) * width + i - startX]
              .Attributes);
    }
  }
}

//the following function works with game objects
//that are of size 1x1
//(their sprite is a single 'tile')
void Renderer::draw(const GameObject &gob) {
  Sprite *sprite = gob.getSprite();

  short startX = gob.getx() + x_offset;
  short startY = gob.gety() + y_offset;

  COORD current = {startX, startY};

  if(!(startX > x_max || startX < 0) && !(startY > y_max || startY < 0))
    console->updateBuffer(current, sprite->geSpriteTable()[0].Char.AsciiChar,
                          sprite->geSpriteTable()[0].Attributes);
}

void Renderer::drawSprite(const Sprite *sprite, short x, short y) {

  short width = sprite->width();
  short height = sprite->height();
  short endX = width + x;
  short endY = height + y;

  for (short i = x; i < endX; ++i) {
    for (short j = y; j < endY; ++j) {
      COORD currentXY = {i, j};
      if (sprite->geSpriteTable()[(j - y) * width + i - x].Char.AsciiChar ==
          0x0000)
        continue;
      
      currentXY.X += x_offset;
      currentXY.Y += y_offset;

      if(!(currentXY.X > x_max || currentXY.X < 0) && !(currentXY.Y > y_max || currentXY.Y < 0))
        console->updateBuffer(
            currentXY,
            sprite->geSpriteTable()[(j - y) * width + i - x].Char.AsciiChar,
            sprite->geSpriteTable()[(j - y) * width + i - x].Attributes);
    }
  }
}

void Renderer::drawTile(CHAR_INFO ch, short x, short y) {

  x+= x_offset;
  y+= y_offset;

  if(!(x > x_max || x < 0) && !(y > y_max || y < 0))
  console->updateBuffer({x, y}, ch.Char.AsciiChar, ch.Attributes);
}

void Renderer::fillBackground(CHAR_INFO c, short x, short y, short w, short h) {


  for (short i = x; i < w + x; ++i) {
    for (short j = y; j < h + y; ++j) {
      short currx = i + x_offset;
      short curry = j + y_offset;
      if(!(currx > x_max || currx < 0) && !(curry > y_max || curry < 0))
        console->updateBuffer({currx, curry}, c.Char.AsciiChar, c.Attributes);
    }
  }
}

//TODO fix to relative rendering
void Renderer::fillBackground(CHAR_INFO *c, short x, short y, short w,
                              short h) {

  for (short i = x; i < w + x; ++i) {
    for (short j = y; j < h + y; ++j) {

      short cx = i - x;
      short cy = i - y;
      short currentpos = (cx - 1) * (w - x) + cy;
      console->updateBuffer({i, j}, c[currentpos].Char.AsciiChar,
                            c[currentpos].Attributes);
    }
  }
}

void Renderer::drawLine(short x1, short y1, short x2, short y2,
                        CHAR_INFO chinfo) {

  short dy = y2 - y1;
  short dx = x2 - x1;

  // first check for horizontal - vertical
  if (dy == 0) {
    // draw horizontal line
    if (x1 > x2)
      std::swap(x1, x2);
    for (short i = x1; i <= x2; ++i) {
      if(!(i + x_offset > x_max) && !(y1 + y_offset > y_max)){
        short relativex = i + x_offset;
        short relativey = y1 + y_offset;
        console->updateBuffer({relativex, relativey}, chinfo.Char.AsciiChar, chinfo.Attributes);
      }
    }
    return;
  }

  if (dx == 0) {
    // draw vertical line
    if (y1 > y2)
      std::swap(y1, y2);
    for (short i = y1; i <= y2; ++i) {
      if(!(x1 + x_offset > x_max) && !(i + y_offset > y_max)){
        short relativex = x1 + x_offset;
        short relativey = i + y_offset;
        console->updateBuffer({relativex, relativey}, chinfo.Char.AsciiChar, chinfo.Attributes);
      }
    }

    return;
  }

  float slope = (float)dy / (float)dx;
  if (std::abs(slope) <= 1) { // dx > dy

    if (x2 < x1) {

      std::swap(x1, x2);
      std::swap(y1, y2);
    }
    for (short i = x1; i <= x2; ++i) {
      short curr_y = std::round(slope * (i - x1) + y1);
      if(!(i + x_offset > x_max) && !(curr_y + y_offset > y_max)){
        short relativex = i + x_offset;
        short relativey = curr_y + y_offset;
        console->updateBuffer({relativex, relativey}, chinfo.Char.AsciiChar,
                            chinfo.Attributes);
      }
    }
  } else if (std::abs(slope) > 1) {
    if (y2 < y1) {

      std::swap(x1, x2);
      std::swap(y1, y2);
    }
    for (short i = y1; i <= y2; ++i) {
      short curr_x = std::round((i - y1) / slope + x1);
      if(!(curr_x + x_offset > x_max) && !(i + y_offset > y_max)){

        short relativex = curr_x + x_offset;
        short relativey = i + y_offset;
        console->updateBuffer({relativex, relativey}, chinfo.Char.AsciiChar,
                            chinfo.Attributes);
      }
    }
  }
}

void Renderer::drawTriangle(short x1, short y1, short x2, short y2, short x3,
                            short y3, CHAR_INFO chinfo) {

  drawLine(x1, y1, x2, y2, chinfo);
  drawLine(x2, y2, x3, y3, chinfo);
  drawLine(x3, y3, x1, y1, chinfo);
}

void Renderer::drawCircle(short x1, short y1, short r, CHAR_INFO chinfo) {

  for (short i = x1; i <= r + x1; ++i) {
    short curr_y =
        std::round(std::sqrt(std::pow(r, 2) - std::pow(i - x1, 2)) + y1);

    short relativex = i + x_offset;
    short relativey = curr_y + y_offset;

    if(!(relativex > x_max || relativex < x_offset) && !(relativey > y_max || relativey < y_offset))
      console->updateBuffer({relativex, relativey}, chinfo.Char.AsciiChar,
                            chinfo.Attributes);

    short mirror_y = 2 * y1 - curr_y;
    short mirror_x = 2 * x1 - i;

    relativex =mirror_x + x_offset;
    relativey =curr_y + y_offset;
    if(!(relativex > x_max || relativex < x_offset) && !(relativey > y_max || relativey < y_offset))
      console->updateBuffer({relativex, relativey}, chinfo.Char.AsciiChar,
                            chinfo.Attributes);

    relativey = mirror_y + y_offset;
    if(!(relativex > x_max || relativex < x_offset) && !(relativey > y_max || relativey < y_offset))
      console->updateBuffer({relativex, relativey}, chinfo.Char.AsciiChar,
                            chinfo.Attributes);

    relativex = i + x_offset;
    if(!(relativex > x_max || relativex < x_offset) && !(relativey > y_max || relativey < y_offset))
      console->updateBuffer({relativex, relativey}, chinfo.Char.AsciiChar,
                            chinfo.Attributes);

  }

  // TODO this is bad efficiency
  // because it paints some pixels multiple times
  // rewrite correct
  for (short i = y1; i <= r + y1; ++i) {
    short curr_x =
        std::round(std::sqrt(std::pow(r, 2) - std::pow(i - y1, 2)) + x1);

    short relativex = curr_x + x_offset;
    short relativey = i + y_offset;

    if(!(relativex > x_max || relativex < x_offset) && !(relativey > y_max || relativey < y_offset))
      console->updateBuffer({relativex, relativey}, chinfo.Char.AsciiChar,
                            chinfo.Attributes);
    short mirror_y = 2 * y1 - i;
    short mirror_x = 2 * x1 - curr_x;

    relativex = mirror_x + x_offset;
    if(!(relativex > x_max || relativex < x_offset) && !(relativey > y_max || relativey < y_offset))
      console->updateBuffer({relativex, relativey}, chinfo.Char.AsciiChar,
                            chinfo.Attributes);

    relativey = mirror_y + y_offset;
    if(!(relativex > x_max || relativex < x_offset) && !(relativey > y_max || relativey < y_offset))
      console->updateBuffer({relativex, relativey}, chinfo.Char.AsciiChar,
                            chinfo.Attributes);

    relativex = curr_x + x_offset;
    if(!(relativex > x_max || relativex < x_offset) && !(relativey > y_max || relativey < y_offset))
      console->updateBuffer({relativex, relativey}, chinfo.Char.AsciiChar,
                            chinfo.Attributes);

  }
}

void Renderer::writeStringToConsole(const COORD &pos, const std::string &str) {
  SHORT currx = pos.X /*+ x_offset*/; //i am leaving this here to fix elsewhere
  SHORT curry = pos.Y + y_offset;
  for (char c : str) {
    if(!(currx > x_max || currx < 0) && !(curry > y_max || curry < 0))
    console->updateBuffer({currx++, curry}, c, 0x12);
  }
}

void Renderer::writeStringCentered(const short &ypos, const std::string &str) {
  short center = Console::WINDOW_WIDTH / 2 - str.size() / 2;
  COORD pos = {center, ypos};
  writeStringToConsole(pos, str);
}

void Renderer::drawRectBorder(short x1, short y1, short x2, short y2,
                              WORD color) {
  // vertical left
  drawLine(x1 - 1, y1 - 1, x1 - 1, y1 + y2, {'|', color});

  // horizontal down
  drawLine(x1 - 1, y1 + y2, x1 + x2, y1 + y2, {'-', color});

  // horizontal up
  drawLine(x1 - 1, y1 - 1, x1 + x2, y1 - 1, {'-', color});

  // vertical right
  drawLine(x1 + x2, y1 - 1, x1 + x2, y1 + y2, {'|', color});

  // corners
  drawTile({192, color}, x1 - 1, y1 + y2);

  drawTile({217, color}, x1 + x2, y1 + y2);
  drawTile({218, color}, x1 - 1, y1 - 1);
  drawTile({191, color}, x1 + x2, y1 - 1);
}

void Renderer::updateDisplay() { console->dumpBufferToConsole(); }
