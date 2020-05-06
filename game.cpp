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
        string spaces((xmax-BOARD_WIDTH)/2, ' ');
        for (int i = 0; i < BOARD_HEIGHT; ++i)
        {
                output += " " + spaces;
                for (int j = 0; j < BOARD_WIDTH; ++j)
                {
                        output += to_string(board[i][j]);
                }
                output += " \n";
        }
        return output;
}

