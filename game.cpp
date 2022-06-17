#include "game.h"
#include "console.h"

short Game::game_area_height = 0;
short Game::game_area_width = 0;
short Game::game_area_x_offset = 0;
short Game::game_area_y_offset = 0;

Game::Game() {

  quitSignal = false;
  console = new Console{};
  console->init();
  uirenderer = new Renderer{console};
  // TODO implement custom offset
  game_area_x_offset = 0;
  game_area_y_offset = 10;
  game_area_width = Console::WINDOW_WIDTH - game_area_x_offset;
  game_area_height = Console::WINDOW_HEIGHT - game_area_y_offset;

  gamerenderer = new Renderer{
    console,
    game_area_x_offset,
    game_area_y_offset,
    game_area_width,
    game_area_height
  };
}

Game::Game(short x, short y) {

  quitSignal = false;
  console = new Console{};
  console->init();
  uirenderer = new Renderer{console};

  game_area_x_offset = x;
  game_area_y_offset = y;
  game_area_width = Console::WINDOW_WIDTH - 2 * game_area_x_offset;
  game_area_height = Console::WINDOW_HEIGHT - 2 * game_area_y_offset;

  gamerenderer = new Renderer{
    console,
    game_area_x_offset,
    game_area_y_offset,
    game_area_width,
    game_area_height
  };
}

Game::~Game() {

  for (auto &g : game_objects) {
    delete g;
  }

  delete player;
  delete uirenderer;
  delete gamerenderer;
  delete console;
}

void Game::init() {}

void Game::addGameObject(GameObject *g) { game_objects.push_back(g); }

void Game::addUIObject(std::string s) { ui_objects.push_back(s); }

void Game::addPlayer(Player *p) { player = p; }

void Game::update() {

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

void Game::redraw() {

  // whole screen
  uirenderer->fillBackground({'.', 0x00}, 0, 0, Console::WINDOW_WIDTH,
                           Console::WINDOW_HEIGHT);

  // game area
  uirenderer->fillBackground({'.', 0x88}, Game::game_area_x_offset,
                           Game::game_area_y_offset, Game::game_area_width,
                           Game::game_area_height);

  if (!game_objects.empty()) {
    for (auto &g : game_objects) {
      uirenderer->draw(*g);
    }
  }

  uirenderer->drawRectBorder(game_area_x_offset, game_area_y_offset,
                           game_area_width, game_area_height,
                           FOREGROUND_BLUE | FOREGROUND_RED
                           // 0x21
  );

  uirenderer->draw(*player);
  uirenderer->writeStringToConsole({0, 0}, std::to_string(current_fps));
  uirenderer->writeStringCentered(2, "This is nice");
  uirenderer->drawCircle(10, 10, 20, {'#', 0x42});
  gamerenderer->drawCircle(120, 10, 20, {'#', 0x42}); //works!
  gamerenderer->writeStringCentered(5, "hello there"); //works!
  gamerenderer->drawLine(10,15, 400, 400, {'#', 0x42}); //works!
  //renderer->drawLine(-10, 0, 1000, 1000, {'#', 0x42});
  //renderer->drawTriangle(50, 2, 100, 2, 50, 18, {'#', 0x42});
  gamerenderer->drawLine(0, 0, 1000, 1000, {'#', 0x42});
  gamerenderer->drawTriangle(50, 2, 100, 2, 50, 18, {'#', 0x42});
  
  
  /*
    this is a bit funny but kind of? good because of the
    fact that the two renderers share the same console and
    as a result the same output buffer. This means that jjuif one
    renderer calls its dump function everything will be drawn
  */
  uirenderer->updateDisplay();
}

void Game::set_quit() { quitSignal = true; }

void Game::start() {
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
      // cl.setTitle(std::to_string(fps));
    }
  }
}

bool Game::has_quit() { return quitSignal; }
