/*
        HKU ENGG 1340 Programming and technologies
        Group 140 (2019-2020 2nd Semester)

        AUTHOR
        TAM CHUN KIT            (3035686554)
        CHOW NAM FUNG           (3035712767)

        DATE
        2020-05-07

        FILENAME
        controller.cpp

        REFERENCES
	This piece of code has direct references to these sources
                - https://stackoverflow.com/questions/23970721/how-to-shift-2d-array-elements-down-in-c

        VIEW
                Tabsize:        8
                Indentation:    Space
*/

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
#include <thread>
#include <algorithm>
#include <iterator>

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
        for (int i = 0; i < Y_UPPER_BOUNDARY; ++i)
        {
                for (int j = 0; j < X_UPPER_BOUNDARY; ++j)
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

// Add a given blockMatrix object. Add that blockMatrix to gameboard. Returns whether if a merge is occurred.
bool addShapeToGameBoard(blockMatrix bMatrix, int posy, int posx, int board[BOARD_HEIGHT][BOARD_WIDTH], int blockNum)
{
        int isMerged = false;
        for (int i = 0; i < bMatrix.size(); i++)
        {
                for (int j = 0; j < bMatrix[i].size(); j++)
                {
                        if 
                        (
                                board[posy+i][posx+j] != blockNum &&            // If board data has cellValue different from blockMatrix
                                board[posy+i][posx+j] != EMPTY_BLOCK_NUM         // It is merged not copy
                        )
                        {
                                isMerged = true;
                                continue;
                        }           
                        if (board[posy+i][posx+j] == EMPTY_BLOCK_NUM)           // Only repalce if zero
                        {
                                board[posy+i][posx+j] = bMatrix[i][j];
                        }

                }
        }
        return isMerged;
}

void clearShapeOnBoard(int blockNum, int board[BOARD_HEIGHT][BOARD_WIDTH])
{
        // STEP 1: Run through the whole block and find all cells with same blockNumber
        for (int i = 0; i < BOARD_HEIGHT; ++i)
        {
                for (int j = 0; j < BOARD_WIDTH; ++j)
                {
                        if (board[i][j] != blockNum)        // Ignore if it is not our target
                        {
                                continue;
                        }
                        if (board[i][j] == blockNum)        // Set as zero if it is same as our blockNum
                        {
                                board[i][j] -= blockNum;
                        }
                }
        }
}

// Make a copy of a game board
void copyGameBoard(int src[BOARD_HEIGHT][BOARD_WIDTH], int dest[BOARD_HEIGHT][BOARD_WIDTH])
{
        for ( int i = 0; i < Y_UPPER_BOUNDARY; ++i )
        {
                for ( int j = 0; j < X_UPPER_BOUNDARY; ++j )
                {
                        dest[i][j] = src[i][j];
                }
        }
}

bool findUnequalAdjecentBlockNumber(int blockNum, int dy, int dx, int augmentedBoard[BOARD_HEIGHT][BOARD_WIDTH])
{
        int adjecentCellData = 0;
        int currentCellData = 0;

        for ( int i = 0; i < Y_UPPER_BOUNDARY; ++i )
        {
                for ( int j = 0; j < X_UPPER_BOUNDARY; ++j )
                {
                        if
                        (       i + dy > BOARD_HEIGHT &&                // Condition 2A : Must not exceed y bondary
                                j + dx > BOARD_WIDTH  &&                // Condition 2B : Must not exceed x bondary upper
                                j + dx < X_LOWER_BOUNDARY               // Condition 2C : Must not exceed x boundary lower
                        )
                        {
                                return true;                            // Collided with boundary
                        }

                        currentCellData  = augmentedBoard[i][j];        // Extract currentCell from augmented
                        adjecentCellData = augmentedBoard[i+dy][j+dx];  // Extract adjecentCellData from augmented

                        if      
                                (    
                                currentCellData == blockNum &&          // Condition 1  : Current block must be scan target
                                adjecentCellData != blockNum &&         // Condition 2  : Adjecent block is not same as current
                                adjecentCellData != EMPTY_BLOCK_NUM     // Condition 3  : Adjecent is nonzero
                                )
                        {
                                return true;                            // All 5 condition satisify -> Collided with adjecent
                        }
                }
        }
        return false;                                                   // None satisified -> Not collided
}

// Check whether a collision occurs with another block
bool isCollidedWithAnotherBlock(vector< vector<int> > currentBlockMatrix, int posy, int posx, int blockNum, int board[BOARD_HEIGHT][BOARD_WIDTH], int direction)
{
        // Duplicate the board, we use that for "simulation"
        int augmentedBoard[BOARD_HEIGHT][BOARD_WIDTH];
        copyGameBoard(board, augmentedBoard);

        addShapeToGameBoard(currentBlockMatrix, posy, posx, augmentedBoard, blockNum);

        switch(direction)
        {
                case DIRECTION_DOWN:
                        return findUnequalAdjecentBlockNumber(blockNum, 1, 0, augmentedBoard);
                        break;
                
                case DIRECTION_LEFT:
                        return findUnequalAdjecentBlockNumber(blockNum, 0, -1, augmentedBoard);
                        break;

                case DIRECTION_RIGHT:
                        return findUnequalAdjecentBlockNumber(blockNum, 0, 1, augmentedBoard);
                        break;
        }

        // Not collided
        return false;
}

// Checks whether a rotation is legit in a gameboard. Returns true if rotation is valid.
bool isRotationValid(block tempBlockObj, int posy, int posx, int blockNum, int board[BOARD_HEIGHT][BOARD_WIDTH])
{
        // Typical steps for converting to blockMatrix
        // 1. Convert to block Object   (Block object always have trimmed dimensions)
        // 2. Convert to blockMatrix
        // 3. Add the blockMatrix at the desired location
        // 
        // After the above 3 steps, we can then conclude whether it is valid

        blockMatrix matrix = blockObjContToMatrix(tempBlockObj, blockNum);      // Stpe 1

        int augmentedBoard[BOARD_HEIGHT][BOARD_WIDTH];                          // Step 2A
        copyGameBoard(board, augmentedBoard);                                   // Step 2B

        if (addShapeToGameBoard(matrix, posy, posx, augmentedBoard, blockNum))            // Step 3
        {
                return false;                                                   // Since addShapeToGameBoard returns true when it is merged
        }
                                                                 
        return true;                                                            // Did not merge

}

// Validates whether a right move is valid. Increment &posx if valid.
void requestMoveRight
(
        block currentBlockObj,
        int posy, 
        int &posx, 
        vector <vector <int> > currentBlockMatrix, 
        int blockNum, 
        int board[BOARD_HEIGHT][BOARD_WIDTH]
)
{
        if (posx + currentBlockObj.xsize < BOARD_WIDTH)
        {
                if (isCollidedWithAnotherBlock(currentBlockMatrix, posy, posx, blockNum, board, DIRECTION_RIGHT))
                {
                        return;
                }
                posx += 1;
        }
                
}

// Validates whether a left move is valid. Decrement &posx if valid.
void requestMoveLeft
(
        block currentBlockObj,
        int posy, 
        int &posx, 
        vector <vector <int> > currentBlockMatrix, 
        int blockNum, 
        int board[BOARD_HEIGHT][BOARD_WIDTH]
)
{
        if (posx - 1 > 0){
                if (isCollidedWithAnotherBlock(currentBlockMatrix, posy, posx, blockNum, board, DIRECTION_LEFT))
                {
                        return;
                }
                posx -= 1;
        }
}

// Validates whether a down move is valid. Increment &posy if valid.
bool requestMoveDown        
(        
        block currentBlockObj,
        int &posy, 
        int posx, 
        vector <vector <int> > currentBlockMatrix, 
        int blockNum, 
        int board[BOARD_HEIGHT][BOARD_WIDTH]
)
{
        if (posy + 1 + currentBlockObj.ysize < BOARD_HEIGHT + Y_PADDING)
        {
                if (isCollidedWithAnotherBlock(currentBlockMatrix, posy, posx, blockNum, board, DIRECTION_DOWN))
                {
                        return false;
                }
                posy += 1;
                return true;
        }
        // Reached the buttom
        return false;
}

// Validates whether a rotation is valid. Replace &rotation with next rotation if valid.
void requestRotate(block tempBlockObj, int &rotation, int posy, int posx, int blockNum, int board[BOARD_HEIGHT][BOARD_WIDTH])
{
        if (posx + tempBlockObj.xsize < X_UPPER_BOUNDARY && posy + tempBlockObj.ysize < Y_UPPER_BOUNDARY && posx > tempBlockObj.xsize)
        {
                if (isRotationValid(tempBlockObj, posy, posx, blockNum, board))
                        rotation = (rotation + 1) % ROTATIONS;
        }
}

// Look for a consecutive empty line to clear. Returns posy of that line if found. Return -1 if not found.
int lookForFilledLine(int board[BOARD_HEIGHT][BOARD_WIDTH])
{	
        // To look for empty lines
        // Traverse the whole line and count for non-zero blockNumber
        // To speed things up this for loop is run reversed.
        
        for (int i = 0; i < Y_UPPER_BOUNDARY; ++i)
        {
                for (int j = 0; j < X_UPPER_BOUNDARY; ++j)
                {
                        if (board[Y_UPPER_BOUNDARY-i-1][j] == 0)        // An empty block is found
                        {                                               // Impossible to be empty
                                break;                                  // Break the whole loop
                        }
                        if (j == X_UPPER_BOUNDARY - 1)
                        {
                                return i;
                        }
                }
        }
        return NO_LINE_TO_CLEAR;
}

// Clear a specified line
void clearLine(int lineNumber, int board[BOARD_HEIGHT][BOARD_WIDTH])
{
        // The idea of inverting came from https://stackoverflow.com/questions/23970721/how-to-shift-2d-array-elements-down-in-c
        
        // STEPS
        // 1. Set zero on that line
        // 2. Shift that line back to top (Y_UPPER_BOUNDARY = 0)

        if 
        (
                lineNumber == NO_LINE_TO_CLEAR ||       // Safety checking for invalid lines
                lineNumber < Y_LOWER_BOUNDARY  ||       // Line number does not even exist (Too low)
                lineNumber > Y_UPPER_BOUNDARY           // Line number does not even exist (Too high)
        )
        {
                return;                         // Leave function
        }

        // Set zero on that line
        for (int j = 0; j < X_UPPER_BOUNDARY; ++j)
        {
                board[lineNumber][j] = 0;
        }


}