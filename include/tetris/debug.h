/*

        DATE
        2020-05-07

        FILENAME
        debug.h

        VIEW
                Tabsize:        8
                Indentation:    Space
                
*/

#ifndef DEBUG_H
#define DEBUG_H
#include "ncurses.h"

// Draws required content in Debug Window if called. Does nothing if DEBUG is set as 0 in options.h
void debugWindow();

// Updates the text componenets in Debug Window
void updateDebug(int board[BOARD_HEIGHT][BOARD_WIDTH]);

#endif
