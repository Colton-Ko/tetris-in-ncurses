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
vector<vector<int>> shapeStringToArray(string shapeString, int blockNum, int ysize, int xsize)
{
	vector<vector<int>> blockMatrix;
	vector<int> row;
    cout << ysize << " " << xsize << endl;
	for (int i = 0; i < ysize; ++i)
	{
		for (int j = 0; j < xsize; ++j)
		{
				switch(shapeString[4*i+j])
				{
					case '.':							// Empty space
						row.push_back(0);
						break;
					case '#':						
						row.push_back(blockNum);		// Solid block
						break;
				}
		}
		blockMatrix.push_back(row);
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

    // This code is from https://stackoverflow.com/questions/20326356/how-to-remove-all-the-occurrences-of-a-char-in-c-string
    output.erase(remove(output.begin(), output.end(), DELETE_CHAR), output.end());

    temp = "";
    for (int i = 0; i < BLOCK_WIDTH; ++i)
    {
        for (int j = 0; j < xsize; ++j)
        {
            temp.push_back(shapeString[4*i+j]);
        }
        if (temp == BLOCK_EMPTY_LINE)
        {
            ysize -= 1;
            // Mark these chars to be deleted with DELETE CHAR
            for (int j = 0; j < xsize; ++j)
            {
                output[4*i+j] = DELETE_CHAR;
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