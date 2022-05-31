#pragma once

#include "console.h"
#include "player.h"

class InputHandler{
    private:
      Console * console;
      Player * player;
    public:
      InputHandler(Console * console, Player * player);
      void handleKeyboardInput(char * oldstate, char * newstate, int bfsize);

};
