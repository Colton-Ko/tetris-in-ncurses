/*

        DATE
        2020-05-07

        FILENAME
        blocks.cpp

        REFERENCES

                This piece of code has direct references to these sources
                -       https://youtu.be/8OK8_tHeCIA
                -       https://stackoverflow.com/questions/20326356/how-to-remove-all-the-occurrences-of-a-char-in-c-string

        VIEW
        
                Tabsize	:	8
                Indentation:	Space

        PURPOSES

                This file is reposible for block manipulations.
                1.      Block rotation
                2.      Block objects conversions

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

// Receives a string array of BLOCK_SHAPE_COUNT length 
// Fills all available variations of blocks into blocks[]
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
// Returns the remapping index of rotating a block String
// from original index (integer i) and angle of rotation (integer mode)
// Angle of rotation = 90*mode
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
        }
        return i;
}

// Returns a rotated copy of 16-character string blockContent
// int rotation: mode of rotation, angle to rotate = 90*mode
// blockString bString: 16-character string to be rotated
blockString rotateBlock(int rotation, blockString bString)
{
        string output = "";
        for ( int j = 0; j < BLOCK_WIDTH_SQR; ++j)
        {
                if (isprint(bString[j]))
                output += bString[rotateBlock(j,rotation)];
        }
        return output;
}

// Converts the blockString to integer array to be embedded
// into gameboard
blockMatrix convertToBlockMatrix(block blockShape, int blockNum)
{
        blockMatrix bMatrix;
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
                bMatrix.push_back(row);
                row.clear();
        }
        return bMatrix;
}

// Take in a 16-character blockString and then convert to an block object
// blockString bString: The blockString to be converted
block convertBlockStringToBlockObj(blockString bString)
{
        int xsize = BLOCK_WIDTH, ysize = BLOCK_WIDTH;
        string temp = "";               //  Temporary string for checking
        string output = bString;    //  Scratch pad string

        for (int j = 0; j < BLOCK_WIDTH; ++j)
        {
                for (int i = 0; i < BLOCK_WIDTH; ++i)
                {
                temp.push_back(bString[4*i+j]);
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
