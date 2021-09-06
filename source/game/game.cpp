#include <game/game.hpp>

Framebuffer gameBuffer;

void DrawTriangle(Triangle triangle){
}

void gameInit(Framebuffer fb){
    gameBuffer = fb;
    Triangle starting;
    starting = {{100, 100, 0}, {300, 100, 0}, {200, 150, 0}};
    DrawTriangle(starting);
}