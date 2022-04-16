#include "player.h"

Player::Player() : Player(10, 10) {}

Player::Player(int cx, int cy) : x(cx), y(cy) {
    
    chInfo = new CHAR_INFO[1];
    chInfo[0].Char.AsciiChar = 0x0001;
    chInfo[0].Attributes = FOREGROUND_GREEN | BACKGROUND_BLUE;
}

Player::~Player(){

    delete[] chInfo;
}

const int& Player::getX() const {return x;}
const int& Player::getY() const {return y;}

void Player::setX(int& newx) {x = newx;}
void Player::setY(int& newy) {y = newy;}

void Player::setPos(COORD newpos) {x = newpos.X; y = newpos.Y;}

COORD Player::getPos() const{
    COORD temp {(SHORT)x, (SHORT)y};
    return temp;
}

short Player::getWidth() const {
    return 1;
}

short Player::getHeight() const {
    return 1;
}

CHAR_INFO * Player::getSprite() const {
    return chInfo;
}


/* WCHAR Player::getChar() const {
    return chInfo.Char.AsciiChar;
}

WORD Player::GetAttr() const {
    return chInfo.Attributes;
} */



//const CHAR_INFO& Player::getPlayerCharInfo() const {return chInfo;}

void Player::move(const char& m){
    
    switch (m){
        case 'w':
            if(y-1 >= 0) 
                --y;
            break;
        case 's':
            if(y+1 < Console::WINDOW_HEIGHT)
                ++y;
            break;
        case 'a':
            --x;
            break;
        case 'd':
            ++x;
            break;
        default:
            break;
    }
}

/* void Player::draw(Console& cl){
    cl.updateBuffer(
        getPos(),
        chInfo.Char.AsciiChar,
        chInfo.Attributes
    );
} */

void Player::incrementDelta(float toAddelta){
    internalTimeDelta += toAddelta;
}

void Player::automove1s(){
    if (internalTimeDelta >= 250000){
        move('s');
        internalTimeDelta = 0;
    }
}