/*
	HKU ENGG 1340 Programming and technologies
	Group 140 (2019-2020 2nd Semester)

	AUTHOR
	TAM CHUN KIT	(3035686554)
	CHOW NAM FUNG	(35<x+>)

	DATE
	2020-05-07

	FILENAME
	blocks.cpp

	REFERENCES
	This piece has direct references to these sources
		- https://youtu.be/8OK8_tHeCIA
		- https://stackoverflow.com/questions/20326356/how-to-remove-all-the-occurrences-of-a-char-in-c-string

	VIEW
		Tabsize = 8
		Indentation:	TAB
*/

#ifndef BLOCKS_H
#define BLOCKS_H

#include <string>
#include <vector>
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