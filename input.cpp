#include "input.h"
#include "console.h"

#include <cstring>
#include <string>

MyInputHandler::MyInputHandler(Console *cn) {

  console = cn;

  std::memset(keyNewState, 0, 256 * sizeof(short));
  std::memset(keyOldState, 0, 256 * sizeof(short));
  std::memset(keys, 0, 256 * sizeof(KeyStates));

  mouseX = 0;
  mouseY = 0;

  inHandle = cn->getInHandle();
}

void MyInputHandler::getInput() {

  for (int i = 0; i < 256; i++) {
    keyNewState[i] = GetAsyncKeyState(i);

    keys[i].pressed = false;
    keys[i].released = false;

    if (keyNewState[i] != keyOldState[i]) {
      if (keyNewState[i] & 0x8000) {
        keys[i].pressed = !keys[i].held;
        keys[i].held = true;
      } else {
        keys[i].released = true;
        keys[i].held = false;
      }
    }

    keyOldState[i] = keyNewState[i];
  }

  /*mouse handlers*/

  // Handle Mouse Input - Check for window events
  INPUT_RECORD inBuf[32];
  DWORD events = 0;
  GetNumberOfConsoleInputEvents(inHandle, &events);

  std::string msg = "Number of events" + std::to_string(events);
  // logMessageBox(msg);

  if (events > 0)
    ReadConsoleInput(inHandle, inBuf, events, &events);

  for (DWORD i = 0; i < events; i++) {
    msg.append(std::to_string(inBuf[i].EventType));
    switch (inBuf[i].EventType) {
    case MOUSE_EVENT: {
      switch (inBuf[i].Event.MouseEvent.dwEventFlags) {
      case MOUSE_MOVED: {
        mouseX = inBuf[i].Event.MouseEvent.dwMousePosition.X;
        mouseY = inBuf[i].Event.MouseEvent.dwMousePosition.Y;
      } break;
      default:
        break;
      }
    } break;
    case KEY_EVENT: // keyboard input
                    // logMessageBox("key event");
                    // break;
    case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing
                                   // logMessageBox("window size event");
                                   // break;
    case FOCUS_EVENT: // disregard focus events
    case MENU_EVENT:  // disregard menu events
    default:
      break;
    }
  }

  for (int m = 0; m < 5; m++) {
    mouse[m].pressed = false;
    mouse[m].released = false;

    if (mouseNewState[m] != mouseOldState[m]) {
      if (mouseNewState[m]) {
        mouse[m].pressed = true;
        mouse[m].held = true;
      } else {
        mouse[m].released = true;
        mouse[m].held = false;
      }
    }

    mouseOldState[m] = mouseNewState[m];
  }
}

KeyStates *MyInputHandler::getKeys() { return keys; }

KeyStates *MyInputHandler::getMouseKeys() { return mouse; }

short MyInputHandler::getMouseX() { return mouseX; }

short MyInputHandler::getMouseY() { return mouseY; }
