#include "inputhandler.h"

InputHandler::InputHandler(Console *cn, Player *pl) {
  console = cn;
  player = pl;
}

void InputHandler::handleKeyboardInput(char *oldstate, char *newstate,
                                       int bfsize) {

  /*
   *  for (int i = 0; i < bfsize; i++) {
   *    newstate[i] = GetAsyncKeyState(i);
   *
   *    m_keys[i].bPressed = false;
   *    m_keys[i].bReleased = false;
   *
   *    if (newstate[i] != oldstate[i]) {
   *      if (newstate[i] & 0x8000) {
   *        m_keys[i].bPressed = !m_keys[i].bHeld;
   *        m_keys[i].bHeld = true;
   *      } else {
   *        m_keys[i].bReleased = true;
   *        m_keys[i].bHeld = false;
   *      }
   *    }
   *
   *    oldstate[i] = newstate[i];
   *  }
   *
   */
  // mouse handlers
  /*
   *
   *  // Handle Mouse Input - Check for window events
   *  INPUT_RECORD inBuf[32];
   *  DWORD events = 0;
   *  GetNumberOfConsoleInputEvents(m_hConsoleIn, &events);
   *  if (events > 0)
   *    ReadConsoleInput(m_hConsoleIn, inBuf, events, &events);
   *
   *
   *
   *  for (int m = 0; m < 5; m++) {
   *    m_mouse[m].bPressed = false;
   *    m_mouse[m].bReleased = false;
   *
   *    if (m_mouseNewState[m] != m_mouseOldState[m]) {
   *      if (m_mouseNewState[m]) {
   *        m_mouse[m].bPressed = true;
   *        m_mouse[m].bHeld = true;
   *      } else {
   *        m_mouse[m].bReleased = true;
   *        m_mouse[m].bHeld = false;
   *      }
   *    }
   *
   *    m_mouseOldState[m] = m_mouseNewState[m];
   *  }
   */
}
