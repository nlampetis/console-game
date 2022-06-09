#include "player.h"
#include "console.h"
#include "bouncingball.h"
#include "renderer.h"
#include "game.h"

#include <chrono>
#include <random>
#include <wincon.h>

void simpleAsciiScroll(Console &cl) {
  for (int i = 0; i < cl.getBufferSize(); ++i) {
    cl.updateBuffer(i, (WCHAR)i, FOREGROUND_GREEN);
    cl.dumpBufferToConsole();
    if (!cl.handleUserInput()) {
      cl.updateBuffer(i, (WCHAR)i, FOREGROUND_GREEN | BACKGROUND_RED);
      cl.dumpBufferToConsole();
      Sleep(3000);
      break;
    }

    Sleep(10);
  }
}
void playerMovement(Console &cl, Player &plr) {

  PCHAR_INFO myMap = new CHAR_INFO[cl.getBufferSize()];
  /*
  //--------------------------------------------------------------------------//
  //                               raw fill                                   //
  //--------------------------------------------------------------------------//
      CHAR_INFO toAdd;

      toAdd.Attributes = FOREGROUND_GREEN;
      toAdd.Char.AsciiChar = ',';

      std::fill_n(myMap, cl.getBufferSize() , toAdd);
  */
  // memcpy(myMap, ',', cl.getBufferSize() * sizeof(CHAR_INFO));

  //--------------------------------------------------------------------------//
  //                            procedual fill                                //
  //--------------------------------------------------------------------------//
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(1, 5);

  for (int i = 0; i < cl.getBufferSize(); ++i) {
    switch (distribution(generator)) {
    case 1:
      myMap[i].Char.AsciiChar = ',';
      break;

    default:
      myMap[i].Char.AsciiChar = '.';
      break;
    }

    myMap[i].Attributes = FOREGROUND_GREEN;
  }

  //--------------------------------------------------------------------------//
  //                             "game" loop                                  //
  //--------------------------------------------------------------------------//

  using namespace std::chrono_literals;

  Renderer renderer(&cl);
  auto deltaTime = std::chrono::high_resolution_clock::now();
  while (true) {

    auto t0 = std::chrono::high_resolution_clock::now();

    // time diff to resolve the game tick
    auto gameTickDiff =
        std::chrono::duration_cast<std::chrono::milliseconds>(t0 - deltaTime)
            .count();
    // things inside this if statement happen every one second
    // TODO: what if something needs to happen slowed i.e. period > 1s?

    if (gameTickDiff >= 16) { // roughly one second has passed
      plr.move('d');          // for now just move the player to the right
      deltaTime =
          std::chrono::high_resolution_clock::now(); // reset the game tick
                                                     // clock for the next cycle
    }

    cl.fillBufferWithMap(myMap, cl.getBufferSize()); // inefficient
    plr.move(cl.handleKeyInput());
    renderer.draw(plr);
    cl.dumpBufferToConsole();

    auto t1 = std::chrono::high_resolution_clock::now();

    auto dif =
        std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
    if (dif != 0) {

      auto fps = (int64_t)1000000 /
                 std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0)
                     .count();
      cl.setTitle(std::to_string(fps));
    }

    // Sleep(10);
  }
}

void bouncingBallLoop(Console &cl) {

  Bouncy ball{};
  Player player{5, 5};
  Renderer renderer(&cl);
  PCHAR_INFO myMap = new CHAR_INFO[cl.getBufferSize()];
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(1, 5);
  for (int i = 0; i < cl.getBufferSize(); ++i) {
    switch (distribution(generator)) {
    case 1:
      myMap[i].Char.AsciiChar = ',';
      break;

    default:
      myMap[i].Char.AsciiChar = '.';
      break;
    }

    myMap[i].Attributes = FOREGROUND_GREEN;
  }

  using namespace std::chrono_literals;
  auto t1 = std::chrono::high_resolution_clock::now();
  auto t0 = std::chrono::high_resolution_clock::now();
  float delta =
      std::chrono::duration_cast<std::chrono::microseconds>(16us).count();
  float totalDelta = 0.0f;

  while (true) {

    t0 = std::chrono::high_resolution_clock::now();

    const char inpt = cl.handleKeyInput();

    totalDelta += delta;
    if (totalDelta > 64000) {
      // player.incrementDelta(totalDelta);
      // player.automove1s(totalDelta);
      // player.move(inpt);
      ball.move(cl, totalDelta / 1000000);
      totalDelta = 0.0f;
      // deltaTime = std::chrono::high_resolution_clock::now(); // reset the
      // game tick clock for the next cycle
    }

    player.automove1s(delta);
    player.move(inpt);

    if (inpt == '+') {
      // logMessageBox("i am here");
      cl.fontResize(true);
    } else if (inpt == '-') {
      cl.fontResize(false);
    } else if (inpt == 'q') {
      break;
    } else if (inpt == 'p') {
      system("pause");
    }

    cl.fillBufferWithMap(myMap, cl.getBufferSize()); // inefficient

     //player.draw(cl); //without the renderer
    //renderer.draw(player);


    CHAR_INFO temp;
    temp.Attributes = FOREGROUND_RED;
    temp.Char.AsciiChar = 'x';
    //renderer.drawLine(5, 5, 5, 10, temp); //vertical
    //renderer.drawLine(7, 7, 15, 7, temp); //horizontal

    //renderer.drawLine(1, 1, 20 , 3, temp);
    //renderer.drawLine(1, 1, 3, 20, temp);
/*
 *
 *    renderer.drawLine(0, 15, 0, 0, temp);
 *    renderer.drawLine(0, 15, 20, 0, temp);
 *    renderer.drawLine(0, 15, 50, 0, temp);
 *    renderer.drawLine(0, 15, 80, 0, temp);
 *    renderer.drawLine(0, 15, 110, 0, temp);
 *    renderer.drawLine(0, 15, 200, 15, temp); 
 *    renderer.drawLine(0, 15, 110, Console::WINDOW_HEIGHT - 1, temp);
 *    renderer.drawLine(0, 15, 80, Console::WINDOW_HEIGHT - 1, temp);
 *    renderer.drawLine(0, 15, 20, Console::WINDOW_HEIGHT - 1, temp);
 *    renderer.drawLine(0, 15, 50, Console::WINDOW_HEIGHT - 1, temp);
 *    renderer.drawLine(0, 15, 0, Console::WINDOW_HEIGHT - 1, temp);
 *
 */

    renderer.drawLine(Console::WINDOW_WIDTH - 1, 15, Console::WINDOW_WIDTH -1 , Console::WINDOW_HEIGHT -1, temp);
    renderer.drawLine(Console::WINDOW_WIDTH - 1, 15, 110, Console::WINDOW_HEIGHT-1, temp);
    renderer.drawLine(Console::WINDOW_WIDTH - 1, 15, 80, Console::WINDOW_HEIGHT-1, temp);
    renderer.drawLine(Console::WINDOW_WIDTH - 1, 15, 50, Console::WINDOW_HEIGHT-1, temp);
    renderer.drawLine(Console::WINDOW_WIDTH - 1, 15, 0, Console::WINDOW_HEIGHT-1, temp);
    renderer.drawLine(Console::WINDOW_WIDTH - 1, 15, 0, 0, temp);
    renderer.drawLine(Console::WINDOW_WIDTH - 1, 15, 50, 0, temp);
    renderer.drawLine(Console::WINDOW_WIDTH - 1, 15, 80, 0, temp);
    renderer.drawLine(Console::WINDOW_WIDTH - 1, 15, 110, 0, temp);
    renderer.drawLine(Console::WINDOW_WIDTH - 1, 15, Console::WINDOW_WIDTH - 1, 0, temp);

    renderer.drawTriangle(10, 10, 5, 5, 20, 5, {'x', FOREGROUND_BLUE});
    renderer.drawTriangle(4, 20, 40, 10, 74, 20, {'x', FOREGROUND_BLUE});


    renderer.drawCircle(60, 15, 8, {'X', 0x25});
    renderer.drawCircle(60, 15, 5, {'X', 0x25});
    renderer.drawCircle(60, 15, 12, {'X', 0x25});

    //renderer.writeStringToConsole({0, 20}, "hello"); 
    renderer.writeStringCentered(5, "This is really cool!");

    //ball.draw(cl);
    renderer.updateDisplay();
    // cl.dumpBufferToConsole(); //without the renderer

    t1 = std::chrono::high_resolution_clock::now();

    delta =
        (float)std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0)
            .count();
    if (delta > 1000000) {
      delta = 16; // for debugging
    }
    if (delta != 0) {
      auto fps = (int64_t)1000000 /
                 std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0)
                     .count();
      cl.setTitle(std::to_string(fps));
    }
  }
}

void gameLoopTemplate(Console &cl) {

  Renderer renderer(&cl);
  constexpr float GAME_TICK = 16666.0f; // 60 updates per second game tick

  using namespace std::chrono_literals;
  auto t1 = std::chrono::high_resolution_clock::now();
  auto t0 = std::chrono::high_resolution_clock::now();
  float delta =
      std::chrono::duration_cast<std::chrono::microseconds>(16us).count();
  float timeDiff = 0.0f;

  while (true) {

    t0 = std::chrono::high_resolution_clock::now();

    if (timeDiff >= GAME_TICK) { // update physics/logic once per game tick
      timeDiff = 0;
    }

    // renderer.draw(Drawable); //example draw every cycle
    renderer.updateDisplay(); // refresh every cycle

    t1 = std::chrono::high_resolution_clock::now();

    delta =
        (float)std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0)
            .count();
    if (delta != 0) {
      auto fps = (int64_t)1000000 /
                 std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0)
                     .count();
      cl.setTitle(std::to_string(fps));
    }
  }
}

void gameLoopTemplateNew(Game * game) {

  constexpr float GAME_TICK = 16666.0f; // 60 updates per second game tick

  using namespace std::chrono_literals;
  auto t1 = std::chrono::high_resolution_clock::now();
  auto t0 = std::chrono::high_resolution_clock::now();
  float delta =
      std::chrono::duration_cast<std::chrono::microseconds>(16us).count();
  float timeDiff = 0.0f;

  int fps = 0;
  while (!game->has_quit()) {

    t0 = std::chrono::high_resolution_clock::now();

    if (timeDiff >= GAME_TICK) { // update physics/logic once per game tick
      timeDiff = 0;
    }

    game->update();
    game->redraw();

    t1 = std::chrono::high_resolution_clock::now();

    delta =
        (float)std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0)
            .count();
    if (delta != 0) {
      fps = (int64_t)1000000 /
                 std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0)
                     .count();
      //cl.setTitle(std::to_string(fps));
    }
  }
}

int main() {
  Game game {5, 5};
  game.init();
  game.addPlayer(new Player{10, 10});
  game.start();
}
