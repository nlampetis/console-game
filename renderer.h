#pragma once

#include <windows.h>

#include "console.h"
#include "drawable.h"

class Renderer {
    private:
        Console * console;
    public:
        Renderer(Console*);
        virtual ~Renderer();
        void draw(const Drawable&);
        void updateDisplay();
        void writeStringToConsole(const COORD&,  const std::string&);
        void writeStringCentered(const short& ypos, const std::string& str);
};
