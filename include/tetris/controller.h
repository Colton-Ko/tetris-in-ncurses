#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>
#include <ncurses.h>
#include "tetris/constants.h"
using namespace std;

void drawBox(WINDOW * win);
void clearOldBlocks(WINDOW * window, int posx, int posy);
void exitOnSmallTerminal(int ymax, int xmax);
void printScore(WINDOW * iwin, int xmax, int score);
void printInstructionWindow(WINDOW * iwin, int xmax);
void drawBlockMatrix(WINDOW * twin, int posy, int posx, block currentBlock);
void startGame(WINDOW * twin, WINDOW * iwin, int ymax, int xmax);

#endif