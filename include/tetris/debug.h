/*
        HKU ENGG 1340 Programming and technologies
        Group 140 (2019-2020 2nd Semester)

        AUTHOR
        TAM CHUN KIT            (3035686554)
        CHOW NAM FUNG           (3035712767)

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