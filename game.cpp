#include "blocks.h"
#include "controller.h"
#include "game.h"
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

void generateGameBoard(int board[TWIN_HEIGHT][TWIN_WIDTH])
{
        for (int i = 0; i < TWIN_HEIGHT; ++i)
        {
                for (int j = 0; j < TWIN_WIDTH; ++j)
                {
                        board[i][j] = 0;
                }
        }
}