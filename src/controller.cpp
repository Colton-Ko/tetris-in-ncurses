/*
	HKU ENGG 1340 Programming and technologies
	Group 140 (2019-2020 2nd Semester)

	AUTHOR
	TAM CHUN KIT	(3035686554)
	CHOW NAM FUNG	(35<x+>)

	DATE
	2020-05-07

	FILENAME
	controller.cpp

	REFERENCES
	This piece has direct references to these sources
		-	

	VIEW
		Tabsize	:	8
		Indentation:	TAB
*/

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
#include <algorithm>

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

// Dont worry this will be removed in later versions
void drawGameBoard(WINDOW * twin, int posy, int posx, vector < vector<int> > blockMatrix)
{
	for (int i = 0; i < blockMatrix.size(); i++)					// For each ysize
	{
		for (int j = 0; j < blockMatrix[i].size(); j++)
		{
			if (blockMatrix[i][j])
			{
				wattron(twin, COLOR_PAIR(WHITE_BACKGROUND));		// Enable printing a whole block "beautifully :)"
				mvwprintw(twin, i+posy, posx+j, "#");			// Full block just for this line
				wattroff(twin, COLOR_PAIR(WHITE_BACKGROUND));		// Turn off that beautify thing
			}
				
		}
	}

	wrefresh(twin);
}

void startGame(WINDOW * twin, WINDOW * iwin, WINDOW * dwin, int ymax, int xmax)
{	
	// Spawn block
	string blocks[BLOCK_SHAPE_COUNT];
	string currentBlock = spawnNewBlock();

	// Declare variables
	int rotation = 0;			// Stores Rotation angle, where angle = 90*rotation
	int blocksCount = 1;			// Acts as the block Number
	block currentBlockObj;			// Current blockObject/ blockShape
	block tempBlockObj;			// Temporary block object
	vector< vector<int> > blockMatrix;	// 2D vector for storing the matrix verison of blockShape

        // Generate gameboard
        string boardStr = "";
        int board[BOARD_HEIGHT][BOARD_WIDTH];
        generateGameBoard(board);

	int posx = (xmax-currentBlockObj.xsize)/2, posy = 0;

	while (true)
	{
		char x = getch();
		clearOldBlocks(twin, posx, posy);
		clearShapeOnBoard(blocksCount, board);

		wmove(iwin, 19, 0);
		wclrtoeol(iwin);
		wmove(iwin, 20, 0);
		wclrtoeol(iwin);

		blockStringToBlockObj(rotateBlock(rotation, currentBlock));
		
		switch (x)
		{
			case W_KEY:
				tempBlockObj = blockStringToBlockObj(rotateBlock((rotation + 1) % 4, currentBlock));
				if (posx + tempBlockObj.xsize < BOARD_WIDTH && posy + tempBlockObj.ysize < BOARD_HEIGHT && posx > tempBlockObj.xsize)
				{
					rotation = (rotation + 1) % 4;
				}
				mvwprintw(iwin, 19, (xmax-5)/2, (to_string(posx) + " " + to_string(posy)).c_str());
				mvwprintw(iwin, 20, (xmax-5)/2, (to_string(posx + tempBlockObj.xsize) + " " + to_string(posy + tempBlockObj.ysize)).c_str());
				mvwprintw(iwin, 15, (xmax-1)/2, to_string(rotation).c_str());
				mvwprintw(iwin, 14, (xmax-1)/2, "W");
				break;

			case A_KEY:
				if (posx - 1 > 0)					// Prevent going out of bonds
					posx -= 1;
				mvwprintw(iwin, 14, (xmax-1)/2, "A");
				mvwprintw(iwin, 19, (xmax-5)/2, (to_string(posx) + " " + to_string(posy)).c_str());
				mvwprintw(iwin, 20, (xmax-5)/2, (to_string(posx + currentBlockObj.xsize) + " " + to_string(posy + currentBlockObj.ysize)).c_str());
				break;

			case S_KEY:
				if (posy + currentBlockObj.ysize < BOARD_HEIGHT)	// Prevent going out of bonds
					posy += 1;
				mvwprintw(iwin, 14, (xmax-1)/2, "S");
				mvwprintw(iwin, 19, (xmax-5)/2, (to_string(posx) + " " + to_string(posy)).c_str());
				mvwprintw(iwin, 20, (xmax-5)/2, (to_string(posx + currentBlockObj.xsize) + " " + to_string(posy + currentBlockObj.ysize)).c_str());
				break;
			case D_KEY:
				if (posx + 1 + currentBlockObj.xsize < BOARD_WIDTH+1)
					posx += 1;
				mvwprintw(iwin, 14, (xmax-1)/2, "D");
				mvwprintw(iwin, 19, (xmax-5)/2, (to_string(posx) + " " + to_string(posy)).c_str());
				mvwprintw(iwin, 20, (xmax-5)/2, (to_string(posx + currentBlockObj.xsize) + " " + to_string(posy + currentBlockObj.ysize)).c_str());
				break;								
		}

		currentBlockObj = blockStringToBlockObj(rotateBlock(rotation % 4, currentBlock));
		blockMatrix = blockObjContToMatrix(currentBlockObj, blocksCount);
		addShapeToGameBoard(blockMatrix, posy, posx, board);
		drawGameBoard(twin, posy + Y_PADDING, posx + X_PADDING, blockMatrix);
		updateDebug(board, dwin);
		box(iwin, 0, 0);
		wrefresh(iwin);
	}
}

void game_init()
{
	initscr();      					// Initialize screen
	srand(time(NULL));					// Initialize random seed
	start_color();						// Full black background
	init_pair(WHITE_BACKGROUND, COLOR_WHITE, COLOR_WHITE);
	curs_set(0);    					// Hide cursor
	noecho();							// No echo (@echo off)
	
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
		newwin(DWIN_HEIGHT, DWIN_WIDTH, 0,
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