#include "player.h"
#include "sprite.h"

Player::Player() : Player(10, 10) {}

Player::Player(int cx, int cy) : x(cx), y(cy) {
    
    CHAR_INFO chInfo;
    chInfo.Char.AsciiChar = 0x0001;
    chInfo.Attributes = FOREGROUND_GREEN | BACKGROUND_BLUE;
    sprite = new Sprite(5, 5, chInfo);
    CHAR_INFO replacement {0x0000};

    for (int xx = 1; xx < 4; ++xx){
      for (int yy = 1; yy < 4; ++yy){
        sprite->changePixel(xx, yy, replacement);
      }
    }
    /*sprite->changePixel(1, 2, replacement);
    sprite->changePixel(1, 3, replacement);
    sprite->changePixel(1, 4, replacement);
    sprite->changePixel(2, 1, replacement);
    sprite->changePixel(2, 2, replacement);
    sprite->changePixel(2, 3, replacement);
    sprite->changePixel(2, 4, replacement);*/
}


Player::Player(int cx, int cy, Sprite * asprite) : x(cx), y(cy) {
    sprite = asprite;
}

Player::~Player(){

    delete sprite;
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
    return sprite->width();
}

short Player::getHeight() const {
    return sprite->height();
}

Sprite * Player::getSprite() const {
    return sprite;
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
            if(y - 1 >= 0) 
                --y;
            break;
        case 's':
            if(y + getHeight() < Console::WINDOW_HEIGHT)
                ++y;
            break;
        case 'a':
            if(x - 1 >= 0)
            --x;
            break;
        case 'd':
            if(x + getWidth() < Console::WINDOW_WIDTH)
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

void Player::automove1s(float delta){
    incrementDelta(delta);
    if (internalTimeDelta >= 250000){
        move('s');
        internalTimeDelta = 0;
    }
}
