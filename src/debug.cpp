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

const string DEBUGWINDOW = "DEBUG WINDOW";

void debugWindow(WINDOW * dwin)
{
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

void updateDebug(int board[BOARD_HEIGHT][BOARD_WIDTH], WINDOW * dwin)
{
        int xmax;
        xmax = getmaxx(dwin);
        string boardStr = gameboardToString(board, xmax);

        mvwprintw(dwin, 1, 0, boardStr.c_str());

        box(dwin, 0, 0);
        mvwprintw(dwin, 0, (xmax - DEBUGWINDOW.size())/2, "DEBUG WINDOW");
        
        wrefresh(dwin);
}