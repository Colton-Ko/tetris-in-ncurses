#ifndef BLOCKS_H
#define BLOCKS_H

#include <string>
#include "tetris/constants.h"
#include "tetris/types.h"

using namespace std;

void getBlocksReady(string blocks[BLOCK_SHAPE_COUNT]);
int rotateBlock(int i, int mode);
string rotateBlock(int i, string blockContent);
void addShapeToGameBoard(int block[BLOCK_WIDTH][BLOCK_WIDTH], int ysize, int xsize, int posx, int posy, int board[BOARD_HEIGHT][BOARD_WIDTH]);
block blockStringToBlockObj(string shapeString);
vector< vector<int> > blockObjContToMatrix(block blockShape, int n);
#endif