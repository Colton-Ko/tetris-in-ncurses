/*
        HKU ENGG 1340 Programming and technologies
        Group 140 (2019-2020 2nd Semester)

        AUTHOR
        TAM CHUN KIT            (3035686554)
        CHOW NAM FUNG           (3035712767)

        DATE
        2020-05-09

        FILENAME
        window.h

        VIEW
                Tabsize:        8
                Indentation:    Space
*/

#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <ncurses.h>
#include "tetris/constants.h"

using namespace std;

// Returns the length of the longest string in a string array
// string strarr[]:     The string array to be inspected
// int size:            The number of strings in strarr
int getMaxLength(string strarr[], int size);

// Display a menu of choices, and returns an integer representing the user's choice
// string choices[]:    The menu to be printed
// int choicesCount:    The number of choices available
// int ymax:            Height of stdscr
// int xmax:            Width of stdscr
// -> _int:             The choice that user made
int showMenu(string choices[], int choicesCount, int ymax, int xmax);

// Notify user that the game over. Display a menu for handling consequence user's actions.
// int score:   The score in previous math
// -> void
void gameOverMenu(int score);


#endif