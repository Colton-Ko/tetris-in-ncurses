/*
	HKU ENGG 1340 Programming and technologies
	Group 140 (2019-2020 2nd Semester)

	AUTHOR
	TAM CHUN KIT	(3035686554)
	CHOW NAM FUNG	(3035712767)

	DATE
	2020-05-07

	FILENAME
	blocks.cpp

	REFERENCES
	This piece has direct references to these sources
		- https://youtu.be/8OK8_tHeCIA
		- https://stackoverflow.com/questions/20326356/how-to-remove-all-the-occurrences-of-a-char-in-c-string

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
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


void getBlocksReady(string blocks[BLOCK_SHAPE_COUNT])
{
	// Clear all exisitng garabge in blocks[7]
	for (int i = 0; i < BLOCK_SHAPE_COUNT; ++i)
	{
		blocks[i] = "";
	}
	// Create blocks
	// The blocks arrangements and implementation ideas 
	// are from https://youtu.be/8OK8_tHeCIA

	blocks[0] = SHAPE_0;
	blocks[1] = SHAPE_1;
	blocks[2] = SHAPE_2;
	blocks[3] = SHAPE_3;
	blocks[4] = SHAPE_4;
	blocks[5] = SHAPE_5;
	blocks[6] = SHAPE_6;

}

// The idea of implementing rotation through index functions
// are from https://youtu.be/8OK8_tHeCIA

int rotateBlock(int i, int mode)
{
	switch (mode)
	{
		case 0:
			i = i;
			break;
		case 1:  // 90 degrees
			i = 12 - 4*(i % 4) + (i / 4);
			break;
		case 2: // 180 degrees
			i = 15 - 4*(i / 4) - (i % 4);
			break;
		case 3: // 270 degrees
			i = 3 + 4*( i % 4) - (i / 4);
		default:
			break;
	}
	return i;
}

string rotateBlock(int i, string blockContent)
{
	string output = "";
	for ( int j = 0; j < BLOCK_WIDTH_SQR; ++j)
	{
		if (isprint(blockContent[j]))
			output += blockContent[rotateBlock(j,i)];
	}
	return output;
}

// Converts the shapeString to integer array to be embedded
vector< vector<int> > blockObjContToMatrix(block blockShape, int blockNum)
{
        vector< vector<int> > blockMatrix;
        vector<int> row;

        for (int i = 0; i  < blockShape.ysize; ++i)
        {
                for (int j = 0; j < blockShape.xsize; ++j)
                {
                        if (blockShape.content.substr(i*blockShape.xsize,blockShape.xsize)[j] == OCCUPIED_CHAR)
                                row.push_back(blockNum);
                        else
                                row.push_back(0);
                }
                blockMatrix.push_back(row);
                row.clear();
        }
        return blockMatrix;
}

block blockStringToBlockObj(string shapeString)
{
	int xsize = BLOCK_WIDTH, ysize = BLOCK_WIDTH;
	string temp = "";               //  Temporary string for checking
	string output = shapeString;    //  Scratch pad string

	for (int j = 0; j < BLOCK_WIDTH; ++j)
	{
		for (int i = 0; i < BLOCK_WIDTH; ++i)
		{
			temp.push_back(shapeString[4*i+j]);
		}
		if (temp == BLOCK_EMPTY_LINE)
			{
			xsize -= 1;
			// Mark chars to be deleted with DELETE CHAR
			for (size_t i = 0; i < BLOCK_WIDTH; ++i)
			{
				output[4*i+j] = DELETE_CHAR;
			}
		}
		temp = "";
	}

	output.erase(remove(output.begin(), output.end(), DELETE_CHAR), output.end());
	// Lines 
	// This code is from https://stackoverflow.com/questions/20326356/how-to-remove-all-the-occurrences-of-a-char-in-c-string

	temp = "";
	string emptyXsizeLine(xsize, '.');

	for (int i = 0; i < BLOCK_WIDTH; ++i)
	{
		for (int j = 0; j < xsize; ++j)
		{
			temp.push_back(output[xsize*i+j]);
		}
		if (temp == emptyXsizeLine)
		{
			ysize -= 1;
			// Mark these chars to be deleted with DELETE CHAR
			for (int j = 0; j < xsize; ++j)
			{
				output[xsize*i+j] = DELETE_CHAR;
			}
		}
		temp = "";
	}

	// This code is from https://stackoverflow.com/questions/20326356/how-to-remove-all-the-occurrences-of-a-char-in-c-string
	output.erase(remove(output.begin(), output.end(), DELETE_CHAR), output.end());

	block result;
	result.ysize = ysize;
	result.xsize = xsize;
	result.content = output;

	return result;
}