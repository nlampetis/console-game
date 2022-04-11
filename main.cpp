#include "player.h"
#include "console.h"
#include "bouncingball.h"

#include <chrono>
#include <random>

void simpleAsciiScroll(Console& cl){
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

void playerMovement(Console& cl, Player& plr){
    
    PCHAR_INFO myMap = new CHAR_INFO[cl.getBufferSize()];
    
    /* 
    //-----------------------------------------------------------------------------------------//
    //                                      raw fill                                           //
    //-----------------------------------------------------------------------------------------//
        CHAR_INFO toAdd;
        
        toAdd.Attributes = FOREGROUND_GREEN;
        toAdd.Char.AsciiChar = ',';

        std::fill_n(myMap, cl.getBufferSize() , toAdd); 
    */
    //memcpy(myMap, ',', cl.getBufferSize() * sizeof(CHAR_INFO));

    //-----------------------------------------------------------------------------------------//
    //                                      procedual fill                                     //
    //-----------------------------------------------------------------------------------------//
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1,5);


    for(int i = 0; i < cl.getBufferSize(); ++i){
        switch ( distribution(generator) )
        {
        case 1:
            myMap[i].Char.AsciiChar = ',';
            break;
        
        default:
            myMap[i].Char.AsciiChar = '.';
            break;
        }

        myMap[i].Attributes = FOREGROUND_GREEN;
    }


    //-----------------------------------------------------------------------------------------//
    //                                      "game" loop                                        //
    //-----------------------------------------------------------------------------------------//

    using namespace std::chrono_literals;


    auto deltaTime = std::chrono::high_resolution_clock::now();
    while(true){
        

        auto t0 = std::chrono::high_resolution_clock::now();

        //time diff to resolve the game tick
        auto gameTickDiff = std::chrono::duration_cast<std::chrono::milliseconds>(t0 - deltaTime).count();
        //things inside this if statement happen every one second
        //TODO: what if something needs to happen slowed i.e. period > 1s?
        
        if(gameTickDiff >= 16){ //roughly one second has passed
            plr.move('d'); //for now just move the player to the right
            deltaTime = std::chrono::high_resolution_clock::now(); // reset the game tick clock for the next cycle
        }

        cl.fillBufferWithMap(myMap, cl.getBufferSize()); //inefficient
        plr.move(cl.handleKeyInput());
        plr.draw(cl);
        cl.dumpBufferToConsole();

        auto t1 = std::chrono::high_resolution_clock::now();
        
        auto dif = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
        if(dif != 0){

            auto fps =  (int64_t) 1000000 / std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
            cl.setTitle(std::to_string(fps));
        }
        
    
        //Sleep(10);
        
    }
}


void bouncingBallLoop(Console& cl){
    
    Bouncy ball {};

    PCHAR_INFO myMap = new CHAR_INFO[cl.getBufferSize()];
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1,5);
    for(int i = 0; i < cl.getBufferSize(); ++i){
        switch ( distribution(generator) )
        {
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
    auto deltaTime = std::chrono::high_resolution_clock::now();
    while(true){
        auto t0 = std::chrono::high_resolution_clock::now();


        float gameTickDiff = std::chrono::duration_cast<std::chrono::milliseconds>(t0 - deltaTime).count();

        
        if(gameTickDiff >= 5){ 

            ball.move(cl, gameTickDiff/1000);
            deltaTime = std::chrono::high_resolution_clock::now(); // reset the game tick clock for the next cycle
        }

        
        cl.fillBufferWithMap(myMap, cl.getBufferSize()); //inefficient
        ball.draw(cl);

        cl.dumpBufferToConsole();

        auto t1 = std::chrono::high_resolution_clock::now();
        
        auto dif = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
        if(dif != 0){
            auto fps =  (int64_t) 1000000 / std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
            cl.setTitle(std::to_string(fps));
        }
    }
}

int main() {
	
    //dummy comment 3
	Console cl{};
	cl.init();
    Player player{10, 10};

    //simpleAsciiScroll(cl);
    //playerMovement(cl, player);
    bouncingBallLoop(cl);

    //this is a second dummy comment

}