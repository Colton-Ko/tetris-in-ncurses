/*

        DATE
        2020-05-08

        FILENAME
        debug.cpp

        VIEW
                Tabsize:        8
                Indentation:    Space

        PURPOSE

                This file is responsible for handling the presentation of debugging-
                information.

*/

#include "tetris/blocks.h"
#include "tetris/controller.h"
#include "tetris/game.h"
#include "tetris/debug.h"
#include "tetris/options.h"
#include "tetris/constants.h"
#include "tetris/types.h"

#include <ncurses.h>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;


// Draws required content in Debug Window if called. Does nothing if DEBUG is set as 0 in options.h
void debugWindow()
{
        if (!DEBUG)
        {
                return;
        }

        string DEBUGWINDOW = "DEBUG WINDOW";
        int ymax, xmax;
        getmaxyx(dwin, ymax, xmax);

        // Generate gameboard
        string boardStr = "";
        int board[BOARD_HEIGHT][BOARD_WIDTH];
        generateGameBoard(board);
        boardStr = gameboardToString(board, xmax);

        mvwprintw(dwin, 1, 0, boardStr.c_str());

        box(dwin, 0, 0);
        mvwprintw(dwin, 0, (xmax - DEBUGWINDOW.size())/2, "DEBUG WINDOW");
        
        wrefresh(dwin);

}

// Updates the text componenets in Debug Window
void updateDebug(int board[BOARD_HEIGHT][BOARD_WIDTH])
{
        if (!DEBUG)
        {
                return;
        }
        int xmax;
        xmax = getmaxx(dwin);
        
        string boardStr = gameboardToString(board, xmax);
        string DEBUGWINDOW = "DEBUG WINDOW";

        mvwprintw(dwin, 1, 0, boardStr.c_str());

        box(dwin, 0, 0);
        mvwprintw(dwin, 0, (xmax - DEBUGWINDOW.size())/2, "DEBUG WINDOW");
        
        wrefresh(dwin);
}
