#include "console.h"

int main() {
	
	//createAndFillConsole();

	Console cl{};
	cl.init();

    for (int i = 0; i < 3600; ++i) {
        cl.updateBuffer(i, (WCHAR)i, FOREGROUND_GREEN);
        cl.dumpBufferToConsole();
        if (!cl.handleUserInput()) {
            cl.updateBuffer(i, (WCHAR)i, FOREGROUND_GREEN | BACKGROUND_RED);
            cl.dumpBufferToConsole();
            Sleep(3000);
            break;
        }

        Sleep(16);
    }

	//char c;
	//std::cin >> c;
}