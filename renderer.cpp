#include "renderer.h"
#include "sprite.h"
#include "game.h"

#include <cmath>
#include <type_traits>
#include <wincon.h>

Renderer::Renderer(Console *cl) { console = cl; }

Renderer::~Renderer() {}

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

void Renderer::draw(const GameObject &gob) {
  Sprite *sprite = gob.getSprite();

  short startX = gob.getx() + Game::game_area_x_offset;
  short startY = gob.gety() + Game::game_area_y_offset;

  COORD current = {startX, startY};
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
      console->updateBuffer(
          currentXY,
          sprite->geSpriteTable()[(j - y) * width + i - x].Char.AsciiChar,
          sprite->geSpriteTable()[(j - y) * width + i - x].Attributes);
    }
  }
}

void Renderer::drawTile(CHAR_INFO ch, short x, short y) {
  console->updateBuffer({x, y}, ch.Char.AsciiChar, ch.Attributes);
}

void Renderer::fillBackground(CHAR_INFO c, short x, short y, short w, short h) {

  for (short i = x; i < w + x; ++i) {
    for (short j = y; j < h + y; ++j) {
      console->updateBuffer({i, j}, c.Char.AsciiChar, c.Attributes);
    }
  }
}

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
    // horizontal
    // TODO handle horizontal
    if (x1 > x2)
      std::swap(x1, x2);
    for (short i = x1; i <= x2; ++i) {
      console->updateBuffer({i, y1}, chinfo.Char.AsciiChar, chinfo.Attributes);
    }
    return;
  }

  if (dx == 0) {
    // vertical
    // TODO handle vertical
    if (y1 > y2)
      std::swap(y1, y2);
    for (short i = y1; i <= y2; ++i) {
      console->updateBuffer({x1, i}, chinfo.Char.AsciiChar, chinfo.Attributes);
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
      console->updateBuffer({i, curr_y}, chinfo.Char.AsciiChar,
                            chinfo.Attributes);
    }
  } else if (std::abs(slope) > 1) {
    if (y2 < y1) {

      std::swap(x1, x2);
      std::swap(y1, y2);
    }
    for (short i = y1; i <= y2; ++i) {
      short curr_x = std::round((i - y1) / slope + x1);
      console->updateBuffer({curr_x, i}, chinfo.Char.AsciiChar,
                            chinfo.Attributes);
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
    console->updateBuffer({i, curr_y}, chinfo.Char.AsciiChar,
                          chinfo.Attributes);
    short mirror_y = 2 * y1 - curr_y;
    short mirror_x = 2 * x1 - i;

    console->updateBuffer({mirror_x, curr_y}, chinfo.Char.AsciiChar,
                          chinfo.Attributes);
    console->updateBuffer({mirror_x, mirror_y}, chinfo.Char.AsciiChar,
                          chinfo.Attributes);
    console->updateBuffer({i, mirror_y}, chinfo.Char.AsciiChar,
                          chinfo.Attributes);
  }

  // TODO this is bad efficiency
  // because it paints some pixels multiple times
  // rewrite correct
  for (short i = y1; i <= r + y1; ++i) {
    short curr_x =
        std::round(std::sqrt(std::pow(r, 2) - std::pow(i - y1, 2)) + x1);
    console->updateBuffer({curr_x, i}, chinfo.Char.AsciiChar,
                          chinfo.Attributes);
    short mirror_y = 2 * y1 - i;
    short mirror_x = 2 * x1 - curr_x;

    console->updateBuffer({mirror_x, i}, chinfo.Char.AsciiChar,
                          chinfo.Attributes);
    console->updateBuffer({mirror_x, mirror_y}, chinfo.Char.AsciiChar,
                          chinfo.Attributes);
    console->updateBuffer({curr_x, mirror_y}, chinfo.Char.AsciiChar,
                          chinfo.Attributes);
  }
}

void Renderer::writeStringToConsole(const COORD &pos, const std::string &str) {
  SHORT currx = pos.X;
  for (char c : str) {
    console->updateBuffer({currx++, pos.Y}, c, 0x12);
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
