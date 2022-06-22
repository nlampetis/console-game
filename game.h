#pragma once

#include <vector>
#include <chrono>

#include "console.h"
#include "gameobject.h"
#include "player.h"
#include "renderer.h"
#include "input.h"

class Game {
private:
  // this will be the ui Objects menus etc...
  std::vector<std::string> ui_objects;
  std::vector<GameObject *> game_objects;
  Player *player;
  Console *console;
  Renderer *uirenderer;
  Renderer *gamerenderer;
  MyInputHandler *inputHandler;
  bool quitSignal;
  float current_fps;

public:

  Game();
  Game(short, short);
  ~Game();

  static short game_area_x_offset;
  static short game_area_y_offset;
  static short game_area_width;
  static short game_area_height;

  void addGameObject(GameObject *);
  // this for the ui
  void addUIObject(std::string);
  void addPlayer(Player *);

  void init();
  void update();
  void redraw();

  void set_quit();
  bool has_quit();

  void start();
};
