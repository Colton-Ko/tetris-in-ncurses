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
			board[posy+i][posx+j] = blockMatrix[i][j];
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
			if (board[i][j] == blockNum)
				board[i][j] = 0;
		}
        }
}

bool isCollidedWithAnotherBlock(int board[BOARD_HEIGHT][BOARD_WIDTH])
{
        
}