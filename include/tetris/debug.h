#ifndef DEBUG_H
#define DEBUG_H
#include "ncurses.h"

void debugWindow(WINDOW * dwin);
void updateDebug(int board[BOARD_HEIGHT][BOARD_WIDTH], WINDOW * dwin);

#endif