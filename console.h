#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>

class Console {
private:
	
	HANDLE mOutHandle;
	HANDLE mInHandle;
	SMALL_RECT mWindowSizeRect;
	CONSOLE_FONT_INFOEX mConsoleFont;
	CHAR_INFO* mPcharInfoBuffer;
	PINPUT_RECORD mPinBuffer;
	COORD mFinalBufferCoords;
	CONSOLE_CURSOR_INFO mConsoleVursorInfo;
	CONSOLE_SCREEN_BUFFER_INFO m_csbi;
	size_t mFinalBufferSize;
	

public:
	Console();
	Console(short, short);
	virtual ~Console();

	void init();
	void updateBuffer(const int& i, const WCHAR&, const WORD&);
	void dumpBufferToConsole();
	void hideCursor();
	bool handleUserInput();
};

