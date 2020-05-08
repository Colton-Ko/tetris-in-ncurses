#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>
#include <ncurses.h>
#include "tetris/constants.h"

using namespace std;

// Extern from https://stackoverflow.com/questions/16909164/function-pointer-multiple-definition
extern WINDOW * twin;
extern WINDOW * iwin;
extern WINDOW * dwin;

void clearOldBlocks(int posx, int posy);
void exitOnSmallTerminal(int ymax, int xmax);
void printScore(int xmax, int score);
void printInstructionWindow(int xmax);
void drawBlockMatrix(int posy, int posx, block currentBlock);
void startGame(int ymax, int xmax);

#endif