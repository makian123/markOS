#ifndef GAMERPG_H
#define GAMERPG_H

#include "types.h"

struct item{
    uint16_t dmgBuff;
    uint8_t *name;
    uint16_t defenseBuff;
    bool usable;
}item;

struct player{
    uint8_t *name;
    uint16_t HP;
    uint16_t damage;
    uint16_t defense;
    struct item *inventory;
}player;

struct enemy{
    uint16_t HP;
    uint16_t DMG;
    uint8_t *name;
}enemy;

void gameQuit();
uint8_t * getCommand();
void startIn();
void GamePopBack();
void AddToCommand(uint8_t ch);
void ExecuteCommand();
void GameInit();
void Move();

#endif