#include "ncurses.h"
#include "game.h"
#include "controller.h"
#include <string>
using namespace std;

const string DEBUGWINDOW = "DEBUG WINDOW";

void debugWindow(WINDOW * dwin)
{
        int ymax, xmax;
        getmaxyx(dwin, ymax, xmax);
        box(dwin, 0, 0);
        mvwprintw(dwin, 0, (xmax - DEBUGWINDOW.size())/2, "DEBUG WINDOW");
        wrefresh(dwin);
}