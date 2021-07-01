#ifndef SNAKE_H
#define SNAKE_H

#include "types.h"

void SnakeStart();
void MoveLeft();
void MoveRight();
void MoveDown();
void MoveUp();

void DrawSnake();
void MoveSnake();
void SnakeQuit();
void WaitIO(uint32_t timerCount);

#endif