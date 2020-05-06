#include "blocks.h"
#include "controller.h"
#include "game.h"
#include "debug.h"
#include "options.h"
#include <ncurses.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

void clearOldBlocks(WINDOW * window, int posx, int posy)
{
	string emptyline = "                ";
	for (int i = 0; i < 4; ++i)
		mvwprintw(window, i+posy, posx, emptyline
			.substr(4*i,4).c_str());
}

void drawBox(WINDOW * win)
{
	box(win, 0, 0);						// Draw outline box
	wrefresh(win);
}

void exitOnSmallTerminal(int ymax, int xmax)
{
	// If you have a VT-80 terminal you can run this game
	mvprintw(ymax/2, abs(xmax-41)/2, "Please this program in a larger terminal!");
	mvprintw(ymax/2+1, abs(xmax-21)/2, "Minimum size is 44x22");

	getch();						// Press any key to exit
	endwin();						// Clear ncurses screen window
	exit(1);						// Leave game due to insufficent space to draw game window
}

void printScore(WINDOW * iwin, int xmax, int score)
{
	string strscore = to_string(score);			// Convert integer score to std::string
	mvwprintw(iwin,11, (xmax-strscore.size())/2,
		strscore.c_str());				// Place in middle
}

void printInstructionWindow(WINDOW * iwin, int xmax)
{
	// Prints the instruction window 
	mvwprintw(iwin, 1, (xmax-12)/2 ,"TETRIS GAME");		// 1st line: Game Title
	mvwprintw(iwin, 3, 2,"KEYS");				// 2nd: Walkthrough on keys
								// Keys are same as in Minecraft my fav. game
	mvwprintw(iwin, 5, (xmax-10)/2, "W : Rotate");		// W -> Rotate
	mvwprintw(iwin, 6, (xmax-10)/2, "A : Left");		// A -> Left
	mvwprintw(iwin, 7, (xmax-10)/2, "S : Down");		// S -> Down
	mvwprintw(iwin, 8, (xmax-10)/2, "D : Right");		// D -> Right 

	mvwprintw(iwin, 10, 2, "SCORE");			// Show score
	mvwprintw(iwin, 11, (xmax-1)/2, "0");			// Initialization score is 0

	drawBox(iwin);
	wrefresh(iwin);
}

void drawBlock(WINDOW * twin, int posy, int posx, string currentBlock, int rotation)
{
	for (int i = 0; i < 4; ++i)
			mvwprintw(twin, i+posy, posx, 		
			rotateBlock(rotation, currentBlock)
			.substr(4*i,4).c_str());

	wrefresh(twin);
}

void startGame(WINDOW * twin, WINDOW * iwin, int ymax, int xmax)
{	
	// Spawn block
	string blocks[BLOCK_SHAPE_COUNT];
	string currentBlock = spawnNewBlock();
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
				mvwprintw(iwin, 15, (xmax-1)/2, 
					to_string(rotation).c_str());
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

		drawBlock(twin, posy, posx, currentBlock, rotation);
		wrefresh(iwin);
	}
}

int main()
{
	initscr();      					// Initialize screen
	srand(time(NULL));					// Initialize random seed

	use_default_colors();
	start_color();						// Full black background
	curs_set(0);    					// Hide cursor
	noecho();						// No echo (@echo off)
	
	int ymax, xmax;						// Variables for screen dimension
	getmaxyx(stdscr, ymax, xmax);				// Get screen dimension

	// Terminal is too small
	if (ymax < TWIN_WIDTH || xmax < TWIN_WIDTH+IWIN_WIDTH)
		exitOnSmallTerminal(ymax, xmax);

	
	WINDOW * twin = 
		newwin(TWIN_HEIGHT, TWIN_WIDTH, 0, 0);		// twin stands for "Tetris Window"
	WINDOW * iwin = 
		newwin(IWIN_HEIGHT, IWIN_WIDTH, 0, TWIN_WIDTH);	// iwin stands for "Info. window"

	
	refresh();

	drawBox(twin);
	drawBox(iwin);
	
	if (DEBUG) 
	{
		WINDOW * dwin = 
			newwin(IWIN_HEIGHT, IWIN_WIDTH, 0,
				TWIN_WIDTH + IWIN_WIDTH);
		refresh();
		debugWindow(dwin);
	}
	
	getmaxyx(iwin, ymax, xmax);

	printInstructionWindow(iwin, xmax);

	startGame(twin, iwin, ymax, xmax);

}