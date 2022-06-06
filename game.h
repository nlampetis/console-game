#pragma once

#include <vector>

#include "gameobject.h"
#include "console.h"
#include "player.h"
#include "renderer.h"

class GameObject;
class Player;

class Game{
  private:
    //this will be the ui Objects menus etc...
    std::vector<std::string> ui_objects; 
    std::vector<GameObject * > game_objects;
    Player * player;
    Console * console;
    Renderer * renderer;
  public:

    Game();
    Game(short, short, short, short);
    ~Game();

    static short game_area_x_offset;
    static short game_area_y_offset;
    static short game_area_width;
    static short game_area_height;

    void addGameObject(GameObject *);
    void addUIObject(std::string); //this for the ui
    
    void update();
    void redraw();

};
