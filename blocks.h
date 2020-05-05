#ifndef BLOCKS_H
#define BLOCKS_H

#include <string>
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

void getBlocksReady(string blocks[BLOCK_SHAPE_COUNT]);
int rotateBlock(int i, int mode);

#endif