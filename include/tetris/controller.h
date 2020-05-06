#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>
#include <ncurses.h>
using namespace std;

#define GAMEINFO "GAME RULES"
#define TWIN_HEIGHT 22
#define TWIN_WIDTH 22

#define IWIN_HEIGHT 22
#define IWIN_WIDTH 22

#define BOARD_HEIGHT TWIN_HEIGHT-2
#define BOARD_WIDTH TWIN_WIDTH/2

#define W_KEY 119
#define A_KEY 97
#define S_KEY 115
#define D_KEY 100

void clearOldBlocks(WINDOW * window, int posx, int posy);
void drawBox(WINDOW * win);
void exitOnSmallTerminal(int ymax, int xmax);
void printScore(WINDOW * iwin, int xmax, int score);
void printInstructionWindow(WINDOW * iwin, int xmax);
void drawBlock(WINDOW * twin, int posy, int posx, string currentBlock, int rotation);
void startGame(WINDOW * twin, WINDOW * iwin, int ymax, int xmax);

#endif