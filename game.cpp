#include "game.h"

short Game::game_area_height = 0;
short Game::game_area_width = 0;
short Game::game_area_x_offset = 0;
short Game::game_area_y_offset = 0;

Game::Game(){

  //TODO implement custom offset
  game_area_x_offset = 10;
  game_area_y_offset = 10;
  game_area_width = Console::WINDOW_WIDTH - 10;
  game_area_height = Console::WINDOW_HEIGHT - 10;
}


Game::Game(short x, short y, short w, short h){

  game_area_x_offset = x;
  game_area_y_offset = y;
  game_area_width = w;
  game_area_height = h;
}


Game::~Game(){

  for (auto &g : game_objects){
    delete g;
  }
}



void Game::addGameObject(GameObject * g){
  game_objects.push_back(g);
}

void Game::addUIObject(std::string s){
  ui_objects.push_back(s);
}

void Game::update(){

  const char inpt = console->handleKeyInput();
  player->move(inpt);
    if (inpt == '+') {
      // logMessageBox("i am here");
      console->fontResize(true);
    } else if (inpt == '-') {
      console->fontResize(false);
    } else if (inpt == 'q') {
      system("exit");
    } else if (inpt == 'p') {
      system("pause");
    } 
}

void Game::redraw(){
  for(auto &g : game_objects){
    renderer->draw(*g); 
  }
}
