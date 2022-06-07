#include "game.h"
#include "console.h"

short Game::game_area_height = 0;
short Game::game_area_width = 0;
short Game::game_area_x_offset = 0;
short Game::game_area_y_offset = 0;

Game::Game(){

  quitSignal = false;
  console = new Console{};
  console->init();
  renderer = new Renderer{console};
  //TODO implement custom offset
  game_area_x_offset = 0;
  game_area_y_offset = 10;
  game_area_width = Console::WINDOW_WIDTH - game_area_x_offset;
  game_area_height = Console::WINDOW_HEIGHT - game_area_y_offset;


}


Game::Game(short x, short y){

  quitSignal = false;
  console = new Console{};
  console->init();
  renderer = new Renderer{console};

  game_area_x_offset = x;
  game_area_y_offset = y;
  game_area_width = Console::WINDOW_WIDTH - 2*game_area_x_offset;
  game_area_height = Console::WINDOW_HEIGHT - 2*game_area_y_offset;
}


Game::~Game(){

  for (auto &g : game_objects){
    delete g;
  }

  delete player;
  delete renderer;
  delete console;
}

void Game::init(){
}


void Game::addGameObject(GameObject * g){
  game_objects.push_back(g);
}

void Game::addUIObject(std::string s){
  ui_objects.push_back(s);
}

void Game::addPlayer(Player * p){
  player = p;
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
    set_quit();
  } else if (inpt == 'p') {
    system("pause");
  } 
}

void Game::redraw(){

  //whole screen
  renderer->fillBackground({'.', 0x00},
      0,
      0,
      Console::WINDOW_WIDTH,
      Console::WINDOW_HEIGHT);

  //game area
 renderer->fillBackground({'.', 0xff},
      Game::game_area_x_offset,
      Game::game_area_y_offset,
      Game::game_area_width,
      Game::game_area_height);


  if(!game_objects.empty()){
    for(auto &g : game_objects){
      renderer->draw(*g); 
    }
  }


  //play area borders
  
  //vertical left
  renderer->drawLine(

      game_area_x_offset-1,
      game_area_y_offset-1,

      game_area_x_offset-1,
      game_area_y_offset + game_area_height,

      {'|', 0x10}
  );

  //horizontal down
  renderer->drawLine(

      game_area_x_offset-1,
      game_area_y_offset + game_area_height,

      game_area_x_offset + game_area_width,
      game_area_y_offset + game_area_height,

      {'-', 0x10}
  );

  //horizontal up
  renderer->drawLine(

      game_area_x_offset - 1,
      game_area_y_offset - 1,

      game_area_x_offset + game_area_width,
      game_area_y_offset - 1,

      {'-', 0x10}
  );

  //vertical right
  renderer->drawLine(
      game_area_x_offset + game_area_width,
      game_area_y_offset - 1,
      game_area_x_offset + game_area_width,
      game_area_y_offset + game_area_height,
      {'|', 0x10}
  );

  renderer->drawTile(
      {192, 0x10}, 
      game_area_x_offset-1,
      game_area_y_offset + game_area_height
  );

  renderer->drawTile(
      {217, 0x10},
       
      game_area_x_offset + game_area_width,
      game_area_y_offset + game_area_height
  );


  renderer->drawTile(
      {218, 0x10},
      game_area_x_offset-1,
      game_area_y_offset-1
  );
  renderer->drawTile(
      {191, 0x10},
      game_area_x_offset + game_area_width,
      game_area_y_offset -1
  );



  renderer->draw(*player);
    renderer->writeStringToConsole({0,0}, std::to_string(current_fps));
  //renderer->drawCircle(10, 10, 8, {'#', 0x42});
  //renderer->drawLine(5, 5, 100, 20, {'#', 0x42}); 
  //renderer->drawTriangle(50, 2, 100, 2, 50, 18, {'#', 0x42});
  renderer->updateDisplay();
}

void Game::set_quit(){
  quitSignal = true;
}

void Game::start(){
  constexpr float GAME_TICK = 16666.0f; // 60 updates per second game tick

  using namespace std::chrono_literals;
  auto t1 = std::chrono::high_resolution_clock::now();
  auto t0 = std::chrono::high_resolution_clock::now();
  float delta =
      std::chrono::duration_cast<std::chrono::microseconds>(16us).count();
  float timeDiff = 0.0f;

  int fps = 0;
  current_fps = 0;
  while (!has_quit()) {

    t0 = std::chrono::high_resolution_clock::now();
    timeDiff += delta;

    update();
    redraw();


    if (timeDiff >= GAME_TICK) { // update physics/logic once per game tick

            timeDiff = 0;
    }
    t1 = std::chrono::high_resolution_clock::now();

    delta =
        (float)std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0)
            .count();
    if (delta != 0) {
      fps = (int64_t)1000000 /
                 std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0)
                     .count();
      current_fps = fps;
      //cl.setTitle(std::to_string(fps));
    }
  }

}

bool Game::has_quit(){
  return quitSignal;
}
