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
#include <vector>
#include <list>
#include <string>
#include <thread>
#include <algorithm>
#include <iterator>

using namespace std;

int randomInteger(int max)
{
	int x = rand() % max;
	return x;
}

string spawnNewBlock()
{
	string blocks[BLOCK_SHAPE_COUNT];
	getBlocksReady(blocks);
	
	return rotateBlock(randomInteger(ROTATIONS), blocks[randomInteger(BLOCK_SHAPE_COUNT)]);
}

void generateGameBoard(int board[BOARD_HEIGHT][BOARD_WIDTH])
{
	for (int i = 0; i < BOARD_HEIGHT; ++i)
	{
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			board[i][j] = 0;
		}
	}
}

string gameboardToString(int board[BOARD_HEIGHT][BOARD_WIDTH], int xmax)
{
	string output = "";
	// string spaces((xmax-BOARD_WIDTH)/2, ' ');
	for (int i = 0; i < BOARD_HEIGHT; ++i)
	{
		output;
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			output += to_string(board[i][j]);
		}
		output += " \n";
	}
	return output;
}

void addShapeToGameBoard(vector< vector<int> > blockMatrix, int posy, int posx, int board[BOARD_HEIGHT][BOARD_WIDTH])
{
	for (int i = 0; i < blockMatrix.size(); i++)
	{
		for (int j = 0; j < blockMatrix[i].size(); j++)
		{
			if (!board[posy+i][posx+j])	// Only repalce if zero
			{
				board[posy+i][posx+j] = blockMatrix[i][j];
			}

		}
	}
}

void clearShapeOnBoard(int blockNum, int board[BOARD_HEIGHT][BOARD_WIDTH])
{
	// STEP 1: Run through the whole block and find all cells with same blockNumber
	for (int i = 0; i < BOARD_HEIGHT; ++i)
	{
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			if (board[i][j] != blockNum)	// Ignore if it is not our target
			{
				continue;
			}
			if (board[i][j] == blockNum)	// Set as zero if it is same as our blockNum
			{
				board[i][j] -= blockNum;
			}
		}
	}
}

void copyGameBoard(int src[BOARD_HEIGHT][BOARD_WIDTH], int dest[BOARD_HEIGHT][BOARD_WIDTH])
{
	for ( int i = 0; i < BOARD_HEIGHT; ++i )
	{
		for ( int j = 0; j < BOARD_WIDTH; ++j )
		{
			dest[i][j] = src[i][j];
		}
	}
}

bool isCollidedWithAnotherBlock(vector< vector<int> > currentBlockMatrix, int posy, int posx, int blockNum, int board[BOARD_HEIGHT][BOARD_WIDTH], int direction)
{
	// Duplicate the board, we use that for "simulation"
	int augmentedBoard[BOARD_HEIGHT][BOARD_WIDTH];
	copyGameBoard(board, augmentedBoard);
	
	// // Add blockMatrix (shape) into the augmented board
	// switch (direction)
	// {
	// 	// case DIRECTION_DOWN:
	// 	// 	posy += 1;
	// 	// 	break;
		
	// 	case DIRECTION_LEFT:
	// 		posx -= 1;
	// 		break;
		
	// 	case DIRECTION_RIGHT:
	// 		posx += 1;
	// 		break;
	// }

	addShapeToGameBoard(currentBlockMatrix, posy, posx, augmentedBoard);

	// Now the shape is added to augmented board
	// Check for its collision with others :)

	// Look for every blockNum in the augmentedBoard

	// endwin();
	// system("stty sane");
	// for ( int i = 0; i < BOARD_HEIGHT; ++i )
	// {
	// 	for ( int j = 0; j < BOARD_WIDTH; ++j )
	// 	{
	// 		cout << augmentedBoard[i][j];
	// 	}
	// 	cout << endl;
	// }
	// char x;
	// cout << "Press any key to continue..." << endl;
	// cin >> x;
	// exit(0);

	for ( int i = 0; i < BOARD_HEIGHT; ++i )
	{
		for ( int j = 0; j < BOARD_WIDTH; ++j )
		{
			switch(direction)
			{
				case DIRECTION_DOWN:
					// Now we are at the required block
					// Collision are only happened when the buttom is touched
					// with another block or ground

					// Checking buttom side

					// We walk look at blockNum at index of i+1

					// Cases
					// If buttom adj. is other blockNum		{1}
					// 	-> Collided
					// If buttom adj. block is 0			{2}
					//	-> Floating
					// If buttom adj. block is same as blockNum	{3}
					//	-> Within same block

					if (augmentedBoard[i][j] == blockNum && i + 1 < BOARD_HEIGHT)
					{
						// If the cell below is same as current block {Case 3}
						if (augmentedBoard[i+1][j] == blockNum)
						{
							continue;		// CASE 3
						}
						else if (augmentedBoard[i+1][j] == EMPTY_BLOCK_NUM)
						{
							continue;		// CASE 2
						}
						if (augmentedBoard[i+1][j] != blockNum)
						{
							// Collided		// CASE 1
							return true;
						}
					}
					break;
				
				case DIRECTION_LEFT:
					// Checking Left side

					// We walk look at blockNum at index of j-1

					// Cases
					// If left adj. is other blockNum		{1}
					// 	-> Collided
					// If left adj. block is 0			{2}
					//	-> Not collide
					// If left adj. block is same as blockNum	{3}
					//	-> Within same block

					if (augmentedBoard[i][j] == blockNum && j - 1 > 0)
					{
						// If the cell below is same as current block {Case 3}
						if (augmentedBoard[i][j-1] == blockNum)
						{
							continue;		// CASE 3
						}
						else if (augmentedBoard[i][j-1] == EMPTY_BLOCK_NUM)
						{
							continue;		// CASE 2
						}
						if (augmentedBoard[i][j-1] != blockNum)
						{
							// Collided		// CASE 1
							return true;
						}
					}
					break;

				case DIRECTION_RIGHT:
					// Checking Right side

					// We walk look at blockNum at index of j+1

					// Cases
					// If right adj. is other blockNum		{1}
					// 	-> Collided
					// If right adj. block is 0			{2}
					//	-> Not collide
					// If right adj. block is same as blockNum	{3}
					//	-> Within same block

					if (augmentedBoard[i][j] == blockNum && j + 1 < BOARD_WIDTH)
					{
						// If the cell below is same as current block {Case 3}
						if (augmentedBoard[i][j+1] == blockNum)
						{
							continue;		// CASE 3
						}
						else if (augmentedBoard[i][j+1] == EMPTY_BLOCK_NUM)
						{
							continue;		// CASE 2
						}
						if (augmentedBoard[i][j+1] != blockNum)
						{
							// Collided		// CASE 1
							return true;
						}
					}
					break;
			}
		}
	}

	// Not collided
	return false;
}

void requestMoveRight
(
	block currentBlockObj,
	int posy, 
	int &posx, 
	vector <vector <int> > currentBlockMatrix, 
	int blockNum, 
	int board[BOARD_HEIGHT][BOARD_WIDTH]
)
{
	if (posx + currentBlockObj.xsize < BOARD_WIDTH)
	{
		if (isCollidedWithAnotherBlock(currentBlockMatrix, posy, posx, blockNum, board, DIRECTION_RIGHT))
		{
			return;
		}
		posx += 1;
	}
		
}

void requestMoveLeft
(
	block currentBlockObj,
	int posy, 
	int &posx, 
	vector <vector <int> > currentBlockMatrix, 
	int blockNum, 
	int board[BOARD_HEIGHT][BOARD_WIDTH]
)
{
	if (posx - 1 > 0){
		if (isCollidedWithAnotherBlock(currentBlockMatrix, posy, posx, blockNum, board, DIRECTION_LEFT))
		{
			return;
		}
		posx -= 1;
	}
}

// The arguments are too long I added newlines for them :)
bool requestMoveDown	
(	
	block currentBlockObj,
	int &posy, 
	int posx, 
	vector <vector <int> > currentBlockMatrix, 
	int blockNum, 
	int board[BOARD_HEIGHT][BOARD_WIDTH]
)
{
	if (posy + 1 + currentBlockObj.ysize < BOARD_HEIGHT + Y_PADDING)
	{
		if (isCollidedWithAnotherBlock(currentBlockMatrix, posy, posx, blockNum, board, DIRECTION_DOWN))
		{
			return false;
		}
		posy += 1;
		return true;
	}
	// Reached the buttom
	return false;
}

void requestRotate(block tempBlockObj, int &rotation, int posy, int posx)
{
	// if (posx + tempBlockObj.xsize < BOARD_WIDTH && posy + tempBlockObj.ysize < BOARD_HEIGHT && posx > tempBlockObj.xsize)
	// {
	// 	rotation = (rotation + 1) % ROTATIONS;
	// }
	rotation = (rotation + 1) % ROTATIONS;
}