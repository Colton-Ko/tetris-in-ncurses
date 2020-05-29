/*

        DATE
        2020-05-07

        FILENAME
        controller.h

        VIEW
                Tabsize:        8
                Indentation:    Space
                
*/

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

// Show message if the terminal size is too small
void exitOnSmallTerminal(int ymax, int xmax);

// Prints the score to IWIN
void printScore(int xmax, int score);

// Prints instructions for controlling the game
void printInstructionWindow(int xmax);

// Initialize the game
void gameInit();

#endif
