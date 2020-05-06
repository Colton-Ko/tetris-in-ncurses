#ifndef BLOCKS_H
#define BLOCKS_H

#include <string>
#include "tetris/controller.h"

using namespace std;

#define BLOCK_SHAPE_COUNT 7
#define BLOCK_WIDTH 4
#define BLOCK_WIDTH_SQR 16

#define SHAPE_0 "..#...#...#...#."
#define SHAPE_1 ".##..#...#......"
#define SHAPE_2 ".#...#...##....."
#define SHAPE_3 "..#..##..#......"
#define SHAPE_4 ".#...##...#....."
#define SHAPE_5 ".....##..##....."
#define SHAPE_6 ".#...##..#......"

#define BLOCK_EMPTY_LINE "...."


void getBlocksReady(string blocks[BLOCK_SHAPE_COUNT]);
int rotateBlock(int i, int mode);
string rotateBlock(int i, string blockContent);
void addShapeToGameBoard(int block[BLOCK_WIDTH][BLOCK_WIDTH], int ysize, int xsize, int posx, int posy, int board[BOARD_HEIGHT][BOARD_WIDTH]);

#endif