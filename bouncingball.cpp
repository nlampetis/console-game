#include "bouncingball.h"

Bouncy::Bouncy(){

}

Bouncy::~Bouncy(){}


const coord& Bouncy::getVel() const {return vel;}
const coord& Bouncy::getPos() const {return pos;}

void Bouncy::setVel(int& newx, int& newy) {
    vel.x = newx;
    vel.y = newy;
}
void Bouncy::setPos(int& newx, int& newy) {
    pos.x = newx;
    pos.y = newy;
}

const CHAR_INFO& Bouncy::getBouncyCharInfo() const {
    return chInfo;
}

void Bouncy::draw(Console& cl){
    COORD temp = {static_cast<SHORT>(floor(pos.x)), static_cast<SHORT>(floor(pos.x))};
    cl.updateBuffer(
        temp,
        chInfo.Char.AsciiChar,
        chInfo.Attributes
    );
}

void Bouncy::move(const char& m){
    
    switch (m){
        case 'w':
            --pos.y;
            break;
        case 's':
            ++pos.y;
            break;
        case 'a':
            --pos.x;
            break;
        case 'd':
            ++pos.x;
            break;
        default:
            break;
    }
}
