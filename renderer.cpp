#include "renderer.h"
#include "sprite.h"

Renderer::Renderer(Console* cl){
    console = cl;
}

Renderer::~Renderer(){

}

void Renderer::draw(const Drawable& drawable){

    Sprite * sprite =  drawable.getSprite();
    
    short startX = drawable.getPos().X;
    short startY = drawable.getPos().Y;
    short width = drawable.getWidth();
    short height = drawable.getHeight();
    short endX = width + startX;
    short endY = height + startY;

    for(short i = startX ; i < endX ; ++i){
        for(short j = startY ; j < endY ; ++j ){
            COORD currentXY = {i,j};
            if(sprite->geSpriteTable()[(j - startY)*width + i - startX].Char.AsciiChar == 0x0000)
              continue;
            console->updateBuffer(
                currentXY, 
                sprite->geSpriteTable()[(j - startY)*width + i - startX].Char.AsciiChar, 
                sprite->geSpriteTable()[(j - startY)*width + i - startX].Attributes
            );
        }
    }
    
}


void Renderer::writeStringToConsole(const COORD& pos, const std::string& str){
  SHORT currx = pos.X;
  for(char c : str){
    console->updateBuffer({currx++, pos.Y}, c, 0x12); 
  }
}

void Renderer::writeStringCentered(const short& ypos, const std::string& str){
  short center = Console::WINDOW_WIDTH/2 - str.size() /2;  
  COORD pos = {center, ypos};
  writeStringToConsole(pos,str);
}


void Renderer::updateDisplay(){
    console->dumpBufferToConsole();
}


