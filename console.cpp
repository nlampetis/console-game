#include "console.h"
//#include "myincludes.h"
//#include <string.h>     // for strcpy_s, strcat_s
//#include <stdlib.h>     // for _countof
//#include <stdio.h>      // for printf
//#include <errno.h>      // for return values

Console::Console() : mWindowSizeRect{ 0, 0, 120, 30} {
	
	mConsoleFont.cbSize = sizeof(CONSOLE_FONT_INFOEX);

	mOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	mInHandle = GetStdHandle(STD_INPUT_HANDLE);

	mFinalBufferCoords = { mWindowSizeRect.Right , mWindowSizeRect.Bottom };
	mFinalBufferSize = (size_t)mWindowSizeRect.Right * (size_t)mWindowSizeRect.Bottom;
	mPcharInfoBuffer = new CHAR_INFO[mFinalBufferSize];
	memset(mPcharInfoBuffer, 0, sizeof(CHAR_INFO) * mFinalBufferSize);
	mPinBuffer = new INPUT_RECORD[256];
}

Console::Console(short x, short y) : mWindowSizeRect{ 0, 0, x, y } {

	mConsoleFont.cbSize = sizeof(CONSOLE_FONT_INFOEX);

	mOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	mInHandle = GetStdHandle(STD_INPUT_HANDLE);

	mFinalBufferCoords = { mWindowSizeRect.Right , mWindowSizeRect.Bottom };
	mFinalBufferSize = (size_t)mWindowSizeRect.Right * (size_t)mWindowSizeRect.Bottom;
	mPcharInfoBuffer = new CHAR_INFO[mFinalBufferSize];
	memset(mPcharInfoBuffer, 0, sizeof(CHAR_INFO) * mFinalBufferSize);
	mPinBuffer = new INPUT_RECORD[256];
}

Console::~Console() {
	delete[] mPcharInfoBuffer;
	delete[] mPinBuffer;
}

void Console::init() {
	


	mOutHandle = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,	// read/write access
		FILE_SHARE_READ | FILE_SHARE_WRITE,	// shared
		NULL,                    // default security attributes
		CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE
		NULL);                   // reserved; must be NULL
	if (mOutHandle == INVALID_HANDLE_VALUE)
		MessageBox(NULL, TEXT("CreateConsoleScreenBuffer"), TEXT("Console Error"), MB_OK);


	if (!SetConsoleScreenBufferSize(mOutHandle, mFinalBufferCoords))
		MessageBox(NULL, TEXT("SetConsoleScreenBufferSize"), TEXT("Console Error"), MB_OK);

	//set buffer as active screen buffer
	if (!SetConsoleActiveScreenBuffer(mOutHandle))
		MessageBox(NULL, TEXT("SetConsoleActiveScreenBuffer"), TEXT("Console Error"), MB_OK);
	
	SMALL_RECT initialRect = { 0, 0, 0, 0 };
	SetConsoleWindowInfo(mOutHandle, TRUE, &initialRect);

	
	//if above doesn't work
	//wcscpy_s(cFont.FaceName , L"Consolas");

	//SetCurrentConsoleFontEx(mOutHandle, TRUE, &mConsoleFont);

	GetCurrentConsoleFontEx(mOutHandle, FALSE, &mConsoleFont);
	SetCurrentConsoleFontEx(mOutHandle, FALSE, &mConsoleFont);


	COORD maxTemp = GetLargestConsoleWindowSize(mOutHandle);

	if (!GetConsoleScreenBufferInfo(mOutHandle, &m_csbi))
		MessageBox(NULL, TEXT("GetConsoleScreenBufferInfo"), TEXT("Console Error"), MB_OK);


	if (maxTemp.X >= m_csbi.dwMaximumWindowSize.X) {
		maxTemp.X = m_csbi.dwMaximumWindowSize.X - 1;
	}
	else {
		maxTemp.X -= 1;
	}
	if (maxTemp.Y >= m_csbi.dwMaximumWindowSize.Y) {
		maxTemp.Y = m_csbi.dwMaximumWindowSize.Y - 1;
	}
	else {
		maxTemp.Y -= 1;
	}

	mWindowSizeRect = { 0 , 0 , (SHORT)maxTemp.X,  (SHORT)maxTemp.Y };
	if (!SetConsoleWindowInfo(mOutHandle, TRUE, &mWindowSizeRect)) {
		printf("SetConsoleWindowInfo failed - (%d)\n", GetLastError());
	}

	mFinalBufferCoords = { ++maxTemp.X, ++maxTemp.Y };
	SetConsoleScreenBufferSize(mOutHandle, mFinalBufferCoords);


	hideCursor();

	//SetConsoleMode(mOutHandle, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	GetConsoleScreenBufferInfo(mOutHandle, &m_csbi);
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

}

void Console::updateBuffer(const int& i, const WCHAR& c, const WORD& a) {

	mPcharInfoBuffer[i].Char.AsciiChar = c;
	mPcharInfoBuffer[i].Attributes = a;
}


void Console::dumpBufferToConsole() {

	COORD startCoord = { 0,0 };
	WriteConsoleOutput(
		mOutHandle,
		mPcharInfoBuffer,
		mFinalBufferCoords,
		startCoord,
		&mWindowSizeRect
	);

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

		if (!ReadConsoleInput(
			mInHandle,    // input handle
			mPinBuffer,  // buffer to read into
			255,       // size of buffer
			&numberRead))    // not overlapped
			return  true;
		if (mPinBuffer[0].Event.KeyEvent.uChar.AsciiChar == (CHAR)97) return false;

		updateBuffer(0, (WCHAR)mPinBuffer[0].Event.KeyEvent.uChar.AsciiChar, FOREGROUND_GREEN | BACKGROUND_RED);

	}

	return true;
}