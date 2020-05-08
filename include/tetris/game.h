#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include "tetris/constants.h"
#include "tetris/types.h"

using namespace std;

string spawnNewBlockString();
int randomInteger(int max);

void generateGameBoard
(
        int board[BOARD_HEIGHT][BOARD_WIDTH]
);

string gameboardToString
(
        int board[BOARD_HEIGHT][BOARD_WIDTH], 
        int xmax
);

// Add a given blockMatrix object. Add that blockMatrix to gameboard. Returns whether if a merge is occurred.
bool addShapeToGameBoard
(
        blockMatrix bMatrix, 
        int posy, 
        int posx, 
        int board[BOARD_HEIGHT][BOARD_WIDTH],
        int blockNum
);

blockMatrix shapeStringToArray
(
        string shapeString, 
        int blockNum, 
        int ysize, 
        int xsize
);

void clearShapeOnBoard(
        int blockNum, 
        int board[BOARD_HEIGHT][BOARD_WIDTH]
);

bool isTheMoveValid
(
        int dy, 
        int dx, 
        int posy, 
        int posx, 
        block blockShape
);

bool isCollidedWithAnotherBlock
(
        blockMatrix currentBlockMatrix, 
        int posy, 
        int posx,  
        int blockNum, 
        int board[BOARD_HEIGHT][BOARD_WIDTH]
);

// Validates whether a rotation is valid. Replace &rotation with next rotation if valid.
void requestRotate
(
        block tempBlockObj, 
        int &rotation, 
        int posy, 
        int posx, 
        int blockNum, 
        int board[BOARD_HEIGHT][BOARD_WIDTH]
);

// Validates whether a left move is valid. Decrement &posx if valid.
void requestMoveLeft
(
        block currentBlockObj,
        int posy, 
        int &posx, 
        vector <vector <int> > currentBlockMatrix, 
        int blockNum, 
        int board[BOARD_HEIGHT][BOARD_WIDTH]
);

// Validates whether a right move is valid. Increment &posx if valid.
void requestMoveRight
(
        block currentBlockObj,
        int posy, 
        int &posx, 
        vector <vector <int> > currentBlockMatrix, 
        int blockNum, 
        int board[BOARD_HEIGHT][BOARD_WIDTH]
);

// Validates whether a down move is valid. Increment &posy if valid.
bool requestMoveDown        
(        
        block currentBlockObj,
        int &posy, 
        int posx, 
        vector <vector <int> > currentBlockMatrix, 
        int blockNum, 
        int board[BOARD_HEIGHT][BOARD_WIDTH]
);

// Look for a consecutive empty line to clear. Returns posy of that line if found. Return -1 if not found.
int lookForFilledLine
(
        int board[BOARD_HEIGHT][BOARD_WIDTH]
);

// Clears a line in a gameboard. Accpepts a lineNumber for the line to clear. int * board will be cleared board.
void clearLine
(
        int lineNumber, 
        int board[BOARD_HEIGHT][BOARD_WIDTH]
);

#endif