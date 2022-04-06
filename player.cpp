#include "player.h"

Player::Player() : x(10), y(10){
    chInfo.Char.AsciiChar = 0x0001;
    chInfo.Attributes = FOREGROUND_GREEN | BACKGROUND_BLUE;
}

Player::Player(int cx, int cy) : x(cx), y(cy) {
    chInfo.Char.AsciiChar = 0x0001;
    chInfo.Attributes = FOREGROUND_GREEN | BACKGROUND_BLUE;
}

Player::~Player(){}

const int& Player::getX() const {return x;}
const int& Player::getY() const {return y;}

void Player::setX(int& newx) {x = newx;}
void Player::setY(int& newy) {y = newy;}

void Player::setPos(COORD newpos) {x = newpos.X; y = newpos.Y;}

COORD Player::getPlayerCoords() const{
            COORD temp {(SHORT)x, (SHORT)y};
            return temp;
}

const CHAR_INFO& Player::getPlayerCharInfo() const {return chInfo;}

void Player::move(Moves m){
    
    switch (m){
        case Moves::MOVE_UP:
            --y;
            break;
        case Moves::MOVE_DOWN:
            ++y;
            break;
        case Moves::MOVE_LEFT:
            --x;
            break;
        case Moves::MOVE_RIGHT:
            ++x;
            break;
        default:
            break;
    }
}

void Player::draw(Console& cl){
    cl.updateBuffer(
        getPlayerCoords(),
        chInfo.Char.AsciiChar,
        chInfo.Attributes
    );
}