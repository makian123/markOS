#include "lib/snake.h"
#include "lib/terminal.h"
#include "lib/keyboard.h"
#include "lib/system.h"

uint16_t x, y;
uint8_t rotation;
bool running;

void DrawSnake(){
    uint16_t tempx = x, tempy = y;
    termSetAt(tempx, tempy, ' ' , termColor(COLOR_GREEN, COLOR_GREEN));
}

void MoveLeft(){ rotation = 'L'; }

void MoveRight(){ rotation = 'R'; }

void MoveDown(){ rotation = 'D'; }

void MoveUp(){ rotation = 'U'; }

void SnakeStart(){
    x = 1;
    y = 1;
    uint8_t ch = 0;
    uint8_t keycode = 0;
    running = true;
    termClear();
    /*Draw boundaries*/
    for(uint16_t xP = 0; xP < TERM_WIDTH; ++xP){
        termSetAt(xP, 0, '#', termColor(COLOR_LWHITE, COLOR_LWHITE));
        termSetAt(xP, TERM_HEIGHT - 1, '#', termColor(COLOR_LWHITE, COLOR_LWHITE));
        
    }
    for(uint16_t yP = 0; yP < TERM_HEIGHT; ++yP){
        termSetAt(0, yP, '#', termColor(COLOR_LWHITE, COLOR_LWHITE));
        termSetAt(TERM_WIDTH - 1, yP, '#', termColor(COLOR_LWHITE, COLOR_LWHITE));
        
    }
    while(running){
        MoveSnake();
        
        keycode = getInput();
        switch (keycode)
        {
            case KEY_RIGHT:
                MoveRight();
                break;
            case KEY_LEFT:
                MoveLeft();
                break;
            case KEY_UP:
                MoveUp();
                break;
            case KEY_DOWN:
                MoveDown();
                break;                
            }
    }
    rotation = 'R';
}

void MoveSnake(){
    switch (rotation)
    {
    case 'L':
        if(termGetAt(x - 1, y) != '#'){
            x -= 1;
        }
        else{
            SnakeQuit();
        }
        break;

    case 'R':
        if(termGetAt(x + 1, y) != '#'){
            x += 1;
        }
        else{
            SnakeQuit();
        }
        break;

    case 'U':
        if(termGetAt(x, y - 1) != '#'){
            y -= 1;
        }
        else{
            SnakeQuit();
        }
        break;
    
    default:
        if(termGetAt(x, y + 1) != '#'){
            y += 1;
        }
        else{
            SnakeQuit();
        }
        break;
    }
    DrawSnake();
}

void SnakeQuit(){
    running = false;
}