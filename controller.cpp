#include "blocks.h"
#include "controller.h"
#include <ncurses.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

void clearOldBlocks(WINDOW * window, int posx, int posy)
{
	string emptyline = "                ";
	for (int i = 0; i < 4; ++i)
		mvwprintw(window, i+posy, posx, emptyline.substr(4*i,4).c_str());
}

int main()
{
	srand(time(0)); // Initialize random seed
	initscr();      // Initialize screen

	use_default_colors();
	start_color();	// Full black background
	curs_set(0);    // Hide cursor
	noecho();	
	
	// Get screen dimension
	int ymax, xmax;
	getmaxyx(stdscr, ymax, xmax);

	// Terminal is too small
	if (ymax < TWIN_WIDTH || xmax < TWIN_WIDTH+IWIN_WIDTH)
	{
		mvprintw(ymax/2, abs(xmax-41)/2, "Please this program in a larger terminal!");
		mvprintw(ymax/2+1, abs(xmax-21)/2, "Minimum size is 44x22");
		getch();
		endwin();
		exit(1);
	}

	// twin stands for "Tetris Window"
	// iwin stands for "Info. window"
	WINDOW * twin = newwin(TWIN_HEIGHT, TWIN_WIDTH, 0, 0);
	WINDOW * iwin = newwin(IWIN_HEIGHT, IWIN_WIDTH, 0, TWIN_WIDTH);
	refresh();
	box(twin, 0, 0);
	box(iwin, 0, 0);
	wrefresh(twin);

	getmaxyx(iwin, ymax, xmax);

	mvwprintw(iwin,1, (xmax-12)/2 ,"TETRIS GAME");
	mvwprintw(iwin,3, 2,"KEYS");

	mvwprintw(iwin,5, (xmax-10)/2, "W : Rotate");
	mvwprintw(iwin,6, (xmax-10)/2, "A : Left");
	mvwprintw(iwin,7, (xmax-10)/2, "S : Down");
	mvwprintw(iwin,8, (xmax-10)/2, "D : Right");

	mvwprintw(iwin,10, 2, "SCORE");
	mvwprintw(iwin,11, (xmax-1)/2, "0");


	wrefresh(iwin);

	// Spawn block
	string blocks[BLOCK_SHAPE_COUNT];
	getBlocksReady(blocks);
	string currentBlock = blocks[0];
	int rotation = 0;
	int posx = (xmax-4)/2, posy = 1;

	while (true)
	{
		char x = getch();
		clearOldBlocks(twin, posx, posy);
		switch (x)
		{
			case W_KEY:
				rotation = (rotation + 1) % 4;
				mvwprintw(iwin, 15, (xmax-1)/2, to_string(rotation).c_str());
				mvwprintw(iwin, 14, (xmax-1)/2, "W");
				break;
			case A_KEY:
				if (posx > 1)
					posx -= 1;
				mvwprintw(iwin, 14, (xmax-1)/2, "A");
				break;
			case S_KEY:
				if (posy < TWIN_HEIGHT-5)
					posy += 1;
				mvwprintw(iwin, 14, (xmax-1)/2, "S");
				break;
			case D_KEY:
				if (posx < TWIN_WIDTH-5)
					posx += 1;
				mvwprintw(iwin, 14, (xmax-1)/2, "D");
				break;								
		}
		for (int i = 0; i < 4; ++i)
			mvwprintw(twin, i+posy, posx, rotateBlock(rotation, currentBlock).substr(4*i,4).c_str());
		wrefresh(twin);
		wrefresh(iwin);
	}


}