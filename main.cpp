#include "player.h"
#include "console.h"

int main() {
	
	//createAndFillConsole();

	Console cl{};
	cl.init();

    Player player{10, 10};

    PCHAR_INFO myMap = new CHAR_INFO[cl.getBufferSize()];
    memset(myMap, ',', cl.getBufferSize() * sizeof(CHAR_INFO));

    cl.fillBufferWithMap(myMap, cl.getBufferSize());

    for (int i = 0; i < cl.getBufferSize(); ++i) {
        player.draw(cl);
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

    while(1){

        cl.handleKeyInput();
        Sleep(10);
    }

	//char c;
	//std::cin >> c;
}