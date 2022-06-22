#pragma once

#include "console.h"

struct KeyStates{
  bool pressed;
  bool released;
  bool held;
};

class MyInputHandler {
private:
  Console *console;
  HANDLE inHandle;

  short keyOldState[256];
  short keyNewState[256];
  bool mouseOldState[5];
  bool mouseNewState[5];

  KeyStates keys[256], mouse[5];

  short mouseX;
  short mouseY;

public:
  MyInputHandler(Console *console);
  void getInput();

  KeyStates * getKeys();
  KeyStates * getMouseKeys();
  
  short getMouseX();
  short getMouseY();

};
