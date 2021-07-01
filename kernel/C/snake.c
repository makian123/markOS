#include "lib/snake.h"
#include "lib/terminal.h"
#include "lib/keyboard.h"
#include "lib/system.h"
#include "lib/osMath.h"

uint16_t x, y;
uint16_t prevX, prevY;
uint8_t rotation;
bool running;

void DrawSnake(){
    uint16_t tempx = x, tempy = y;
    termSetAt(tempx, tempy, '#' , termColor(COLOR_GREEN, COLOR_GREEN));
    termSetAt(prevX, prevY, ' ' , termColor(COLOR_BLACK, COLOR_BLACK));
}

void spawnFruit(){
    uint32_t nextX = 0;
    uint32_t nextY = 0;
    while(termGetAt(nextX, nextY) != ' '){
        while(nextX >= TERM_WIDTH || nextX <= 0) nextX = rand();
        while(nextY >= TERM_HEIGHT || nextY <= 0) nextY = rand();
    }
    termSetAt(nextX, nextY, 'A', termColor(COLOR_RED, COLOR_RED));
    nextX = 0;
    nextY = 0;
}

void MoveLeft(){ rotation = 'L'; }

void MoveRight(){ rotation = 'R'; }

void MoveDown(){ rotation = 'D'; }

void MoveUp(){ rotation = 'U'; }

void SnakeStart(){
    x = 1;
    y = 1;
    prevX = 1;
    prevY = 1;
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
    rotation = 'R';
    spawnFruit();
    while(running){
        MoveSnake();
        
        keycode = getInputNoLoop();
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
        sleep(1);
    }
}

void MoveSnake(){
    prevY = y;
    prevX = x;
    switch (rotation)
    {
    case 'L':
        if(termGetAt(x - 1, y) != '#'){
            x -= 1;
            if(termGetAt(x - 1, y) == 'A'){

            }
        }
        else{
            SnakeQuit();
        }
        break;

    case 'R':
        if(termGetAt(x + 1, y) != '#'){
            x += 1;
            if(termGetAt(x - 1, y) == 'A'){
                
            }
        }
        else{
            SnakeQuit();
        }
        break;

    case 'U':
        if(termGetAt(x, y - 1) != '#'){
            y -= 1;
            if(termGetAt(x - 1, y) == 'A'){
                
            }
        }
        else{
            SnakeQuit();
        }
        break;
    
    default:
        if(termGetAt(x, y + 1) != '#'){
            y += 1;
            if(termGetAt(x - 1, y) == 'A'){
                
            }
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