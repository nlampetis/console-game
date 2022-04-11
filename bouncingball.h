#pragma once

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <math.h>

#include "console.h"

struct coord {
        float x;
        float y;
};


class Bouncy{
private:
    coord pos;
    coord vel;
    CHAR_INFO chInfo;
public:
    
    Bouncy();
    virtual ~Bouncy();


    const coord& getVel() const;
    const coord& getPos() const;
    void setVel(int& newx, int& newy);
    void setPos(int& newx, int& newy);
    const CHAR_INFO& getBouncyCharInfo() const;
    
    void move(const char&);
    void draw(Console&);

};