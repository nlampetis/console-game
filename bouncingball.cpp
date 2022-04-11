#include "bouncingball.h"

Bouncy::Bouncy(){
    
    pos.x = 10.0f;
    pos.y = 10.0f;

    vel.x = 30.0f;
    vel.y = 25.0f;

    chInfo.Char.AsciiChar = 0x0001;
    chInfo.Attributes = FOREGROUND_GREEN | BACKGROUND_BLUE;

}

Bouncy::~Bouncy(){}


const myCoord& Bouncy::getVel() const {return vel;}
const myCoord& Bouncy::getPos() const {return pos;}

void Bouncy::setVel(float& newx, float& newy) {
    vel.x = newx;
    vel.y = newy;
}
void Bouncy::setPos(float& newx, float& newy) {
    pos.x = newx;
    pos.y = newy;
}

const CHAR_INFO& Bouncy::getBouncyCharInfo() const {
    return chInfo;
}

void Bouncy::draw(Console& cl){
    COORD temp = {static_cast<SHORT>(floor(pos.x)), static_cast<SHORT>(floor(pos.y))};
    cl.updateBuffer(
        temp,
        chInfo.Char.AsciiChar,
        chInfo.Attributes
    );
}

void Bouncy::move(Console& cl, float delta){
    

    short limitX = cl.getFinalBufferCoords().X;
    short limitY = cl.getFinalBufferCoords().Y;

    pos.x = (float)pos.x + delta * (float)vel.x;
    pos.y = (float)pos.y + delta * (float)vel.y;

    //boundry checks y
    if(pos.y < 0){
        pos.y = -pos.y;
        vel.y = -vel.y;
    }
    if(pos.y > limitY){
        pos.y = 2 * limitY - pos.y;
        vel.y = -vel.y;
    }

    //boundry checks x
    if(pos.x < 0){
        pos.x = -pos.x;
        vel.x = -vel.x;
    }
    if(pos.x > limitX){
        pos.x = 2 * limitX - pos.x;
        vel.x = -vel.x;
    }
}


