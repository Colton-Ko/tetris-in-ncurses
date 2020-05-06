#include "tetris/blocks.h"
#include "tetris/controller.h"
#include "tetris/game.h"
#include "tetris/debug.h"
#include "tetris/options.h"
#include "tetris/constants.h"
#include "tetris/types.h"

#include <ncurses.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>

void drawBox(WINDOW * win)
{
	box(win, 0, 0);						// Draw outline box
	wrefresh(win);
}

void clearOldBlocks(WINDOW * window, int posx, int posy)
{
	wclear(window);
	drawBox(window);
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


void drawBlockMatrix(WINDOW * twin, int posy, int posx, vector<vector<int>> blockMatrix)
{
	for (int i = 0; i < blockMatrix.size(); i++)			// For each ysize
	{
		for (int j = 0; j < blockMatrix[i].size(); j++)
		{
			if (blockMatrix[i][j])
			{
				wattron(twin, 0);
				mvwprintw(twin, i+posy, posx+j, "##");
				wattroff(twin, 0);
			}
				
		}
	}

	wrefresh(twin);
}

void startGame(WINDOW * twin, WINDOW * iwin, WINDOW * dwin, int ymax, int xmax)
{	
	init_pair(0, COLOR_WHITE, COLOR_WHITE);
	// Spawn block
	string blocks[BLOCK_SHAPE_COUNT];
	string currentBlock = spawnNewBlock();
	int rotation = 0;
	int blocksCount = 1;
	block currentBlockObj;
	vector<vector<int>> blockMatrix;

        // Generate gameboard
        string boardStr = "";
        int board[BOARD_HEIGHT][BOARD_WIDTH];
        generateGameBoard(board);

	int posx = (xmax-4)/2, posy = 1;

	while (true)
	{
		char x = getch();
		clearOldBlocks(twin, posx, posy);
		clearShapeOnBoard(blocksCount, board);

		currentBlockObj = blockStringToBlockObj(rotateBlock(rotation, currentBlock));
		blockMatrix = shapeStringToArray(currentBlockObj.content, blocksCount, currentBlockObj.ysize, currentBlockObj.xsize);

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

		drawBlockMatrix(twin, posy, posx, blockMatrix);
		// posy is same as position y on twin
		// posx is 0.5x as in twin, because scale x : scale y = 1:2
		addShapeToGameBoard(blockMatrix, posy, posx/2, board);
		updateDebug(board, dwin);

		wrefresh(iwin);
	}
}

void game_init()
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
	

	WINDOW * dwin = 
		newwin(IWIN_HEIGHT, IWIN_WIDTH, 0,
			TWIN_WIDTH + IWIN_WIDTH);
	refresh();
	debugWindow(dwin);
	
	getmaxyx(iwin, ymax, xmax);
	printInstructionWindow(iwin, xmax);
	startGame(twin, iwin, dwin, ymax, xmax);

}

int main()
{
	game_init();
}