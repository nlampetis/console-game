#include "console.h"

#include <minwindef.h>
#include <string>
#include <wincon.h>
#include <winnt.h>

short Console::WINDOW_HEIGHT = 0;
short Console::WINDOW_WIDTH = 0;

Console::Console() : mWindowSizeRect{0, 0, 120, 30} {

  mConsoleFont.cbSize = sizeof(CONSOLE_FONT_INFOEX);

  mOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  mInHandle = GetStdHandle(STD_INPUT_HANDLE);

  mFinalBufferCoords = {mWindowSizeRect.Right, mWindowSizeRect.Bottom};
  mFinalBufferSize =
      (size_t)mWindowSizeRect.Right * (size_t)mWindowSizeRect.Bottom;
  mPcharInfoBuffer = new CHAR_INFO[mFinalBufferSize];
  memset(mPcharInfoBuffer, 0, sizeof(CHAR_INFO) * mFinalBufferSize);
  mPinBuffer = new INPUT_RECORD[256];

  WINDOW_HEIGHT = mFinalBufferCoords.Y;
  WINDOW_WIDTH = mFinalBufferCoords.X;
}

Console::Console(short x, short y) : mWindowSizeRect{0, 0, x, y} {

  mConsoleFont.cbSize = sizeof(CONSOLE_FONT_INFOEX);

  mOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  mInHandle = GetStdHandle(STD_INPUT_HANDLE);

  mFinalBufferCoords = {mWindowSizeRect.Right, mWindowSizeRect.Bottom};
  mFinalBufferSize =
      (size_t)mWindowSizeRect.Right * (size_t)mWindowSizeRect.Bottom;
  mPcharInfoBuffer = new CHAR_INFO[mFinalBufferSize];
  memset(mPcharInfoBuffer, 0, sizeof(CHAR_INFO) * mFinalBufferSize);
  mPinBuffer = new INPUT_RECORD[256];

  WINDOW_HEIGHT = mFinalBufferCoords.Y;
  WINDOW_WIDTH = mFinalBufferCoords.X;
}

Console::~Console() {
  delete[] mPcharInfoBuffer;
  delete[] mPinBuffer;
}

void Console::init() {

  mOutHandle = CreateConsoleScreenBuffer(
      GENERIC_READ | GENERIC_WRITE,       // read/write access
      FILE_SHARE_READ | FILE_SHARE_WRITE, // shared
      NULL,                               // default security attributes
      CONSOLE_TEXTMODE_BUFFER,            // must be TEXTMODE
      NULL);                              // reserved; must be NULL
  if (mOutHandle == INVALID_HANDLE_VALUE)
    MessageBox(NULL, TEXT("CreateConsoleScreenBuffer"), TEXT("Console Error"),
               MB_OK);

  // this must be here because at the time of setting the console screen buffer
  // size the buffer CANNOT BE SMALLER than the window containing it
  SMALL_RECT initialRect = {0, 0, 0, 0};
  SetConsoleWindowInfo(mOutHandle, TRUE, &initialRect);

  if (!SetConsoleScreenBufferSize(mOutHandle, mFinalBufferCoords))
    MessageBox(NULL, TEXT("SetConsoleScreenBufferSize"), TEXT("Console Error"),
               MB_OK);

  // set buffer as active screen buffer
  if (!SetConsoleActiveScreenBuffer(mOutHandle))
    MessageBox(NULL, TEXT("SetConsoleActiveScreenBuffer"),
               TEXT("Console Error"), MB_OK);

  // if above doesn't work
  // wcscpy_s(cFont.FaceName , L"Consolas");

  // SetCurrentConsoleFontEx(mOutHandle, TRUE, &mConsoleFont);

  GetCurrentConsoleFontEx(mOutHandle, FALSE, &mConsoleFont);

  // TODO: implement this in a function to change the font
  wcscpy_s(mConsoleFont.FaceName, L"Consolas");
  mConsoleFont.dwFontSize = {60, 16};

  SetCurrentConsoleFontEx(mOutHandle, FALSE, &mConsoleFont);

  COORD maxTemp = GetLargestConsoleWindowSize(mOutHandle);

  if (!GetConsoleScreenBufferInfo(mOutHandle, &m_csbi))
    MessageBox(NULL, TEXT("GetConsoleScreenBufferInfo"), TEXT("Console Error"),
               MB_OK);

  // log the window info
  std::string to_log_max = "max X is: " + std::to_string(maxTemp.X);
  to_log_max.append("\nmax Y is: " + std::to_string(maxTemp.Y));

  to_log_max.append("\n\ndwMax X is: " +
                    std::to_string(m_csbi.dwMaximumWindowSize.X));
  to_log_max.append("\ndwMax Y is: " +
                    std::to_string(m_csbi.dwMaximumWindowSize.Y));

  to_log_max.append("\n\ncurr X is: " + std::to_string(m_csbi.dwSize.X));
  to_log_max.append("\ncurr Y is: " + std::to_string(m_csbi.dwSize.Y));

  // logMessageBox(to_log_max);

  if (maxTemp.X >= m_csbi.dwMaximumWindowSize.X) {
    maxTemp.X = m_csbi.dwMaximumWindowSize.X - 1;
  } else {
    maxTemp.X -= 1;
  }
  if (maxTemp.Y >= m_csbi.dwMaximumWindowSize.Y) {
    maxTemp.Y = m_csbi.dwMaximumWindowSize.Y - 1;
  } else {
    maxTemp.Y -= 1;
  }

  mWindowSizeRect = {0, 0, (SHORT)maxTemp.X, (SHORT)maxTemp.Y};
  if (!SetConsoleWindowInfo(mOutHandle, TRUE, &mWindowSizeRect)) {
    printf("SetConsoleWindowInfo failed - (%d)\n", GetLastError());
  }

  mFinalBufferCoords = {++maxTemp.X, ++maxTemp.Y};

  // TODO: fix
  // the error here is the fact that having a very big font
  // of a very low res display will cause the buffer to shrink
  // as a result the output buffer size will not be fixed
  //
  // this may sometimes be desired though but not at this point
  SetConsoleScreenBufferSize(mOutHandle, mFinalBufferCoords);

  hideCursor();

  if (!SetConsoleMode(mInHandle, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT |
                                     ENABLE_MOUSE_INPUT)) {
    logMessageBox("Could not set console mode");
  }

  GetConsoleScreenBufferInfo(mOutHandle, &m_csbi);
  HWND consoleWindow = GetConsoleWindow();
  SetWindowLong(consoleWindow, GWL_STYLE,
                GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX &
                    ~WS_SIZEBOX);
}

void Console::updateBuffer(const int &i, const WCHAR &c, const WORD &a) {

  mPcharInfoBuffer[i].Char.AsciiChar = c;
  mPcharInfoBuffer[i].Attributes = a;
}

void Console::updateBuffer(const COORD &pos, const WCHAR &c, const WORD &a) {

  if ((pos.X < Console::WINDOW_WIDTH && pos.X >= 0) &&
      (pos.Y < Console::WINDOW_HEIGHT && pos.Y >= 0)) {
    mPcharInfoBuffer[(pos.Y) * mFinalBufferCoords.X + pos.X].Char.AsciiChar = c;
    mPcharInfoBuffer[(pos.Y) * mFinalBufferCoords.X + pos.X].Attributes = a;
  }
}

void Console::fillBufferWithMap(const CHAR_INFO *map, const int &size) {
  if (size != mFinalBufferSize)
    return;
  for (int i = 0; i < size; ++i) {
    updateBuffer(i, map[i].Char.AsciiChar, map[i].Attributes);
  }
}

void Console::dumpBufferToConsole() {

  COORD startCoord = {0, 0};
  WriteConsoleOutput(mOutHandle, mPcharInfoBuffer, mFinalBufferCoords,
                     startCoord, &mWindowSizeRect);
}
void Console::hideCursor() {

  GetConsoleCursorInfo(mOutHandle, &mConsoleVursorInfo);
  mConsoleVursorInfo.bVisible = FALSE;
  SetConsoleCursorInfo(mOutHandle, &mConsoleVursorInfo);
}

bool Console::handleUserInput() {

  DWORD numberRead;

  GetNumberOfConsoleInputEvents(mInHandle, &numberRead);
  if (numberRead > 0) {

    if (!ReadConsoleInput(mInHandle,    // input handle
                          mPinBuffer,   // buffer to read into
                          255,          // size of buffer
                          &numberRead)) // not overlapped
      return true;
    if (mPinBuffer[0].Event.KeyEvent.uChar.AsciiChar == (CHAR)97)
      return false;

    updateBuffer(0, (WCHAR)mPinBuffer[0].Event.KeyEvent.uChar.AsciiChar,
                 FOREGROUND_GREEN | BACKGROUND_RED);
  }

  return true;
}

// TODO this better get a buffer pointer as a parameter
// and save the input there
char Console::handleKeyInput() {

  // Have we read more than 0 events?
  //
  // If this check did not occur then the
  // ReadConsoleInput fucntion would be called
  // each frame update and WAIT until an input
  // has been detected
  //
  // In this case we call the function only when
  // an event is present and it executes without
  // any waiting time
  DWORD cNumRead;
  GetNumberOfConsoleInputEvents(mInHandle, &cNumRead);
  if (cNumRead > 0) {

    // if yes then get the console input
    if (!ReadConsoleInput(mInHandle, mInBuf, 128, &cNumRead)) {
      // assuming there is no need for " " input
      return ' ';
    }
    switch (mInBuf[0].EventType) {
    case KEY_EVENT: // keyboard input
      return KeyEventProc(mInBuf[0].Event.KeyEvent);
      break;
    default:
      break;
    }
  }
  // assuming there is no need for " " input
  return ' ';
}

char KeyEventProc(KEY_EVENT_RECORD rec) {
  if (rec.bKeyDown == TRUE && rec.wRepeatCount <= 1) {
    switch (rec.wVirtualKeyCode) {
    case VK_UP:
      return 'w';
      break;
    case VK_DOWN:
      return 's';
      break;
    case VK_LEFT:
      return 'a';
      break;
    case VK_RIGHT:
      return 'd';
      break;
    case 0x51:
      return 'q';
      break;
    default:
      return rec.uChar.AsciiChar;
      break;
    }
  }
  return ' ';
};

const size_t &Console::getBufferSize() const { return mFinalBufferSize; }

void Console::setTitle(std::string title) {

  title = "fps: " + title;
  const char *title2 = title.c_str();
  SetConsoleTitleA(title2);
}

const COORD &Console::getFinalBufferCoords() const {
  return mFinalBufferCoords;
}

void Console::fontResize(const bool &bigger) {

  // SMALL_RECT initialRect = { 0, 0, 0, 0 };
  // SetConsoleWindowInfo(mOutHandle, TRUE, &initialRect);

  if (bigger) {
    mConsoleFont.dwFontSize.X += 4;
    mConsoleFont.dwFontSize.Y += 1;
    SetCurrentConsoleFontEx(mOutHandle, FALSE, &mConsoleFont);

  } else {
    mConsoleFont.dwFontSize.X -= 4;
    mConsoleFont.dwFontSize.Y -= 1;
    SetCurrentConsoleFontEx(mOutHandle, FALSE, &mConsoleFont);
  }
}

void logMessageBox(const std::string &msg) {
  MessageBox(NULL, TEXT(msg.c_str()), TEXT("Console Logger"), MB_OK);
}

HANDLE Console::getInHandle() { return mInHandle; }
