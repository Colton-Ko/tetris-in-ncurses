/*
        HKU ENGG 1340 Programming and technologies
        Group 140 (2019-2020 2nd Semester)

        AUTHOR
        TAM CHUN KIT            (3035686554)
        CHOW NAM FUNG           (3035712767)

        DATE
        2020-05-08

        FILENAME
        game.h

        VIEW
                Tabsize:        8
                Indentation:    Space
                
*/
#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include "tetris/constants.h"
#include "tetris/types.h"

using namespace std;

// Returns a random bonded by 0 and max.
int randomInteger(int max);

// Return a string which contained a block description
string spawnNewBlockString();

// Sets the cell content of a 2D integer array of size BOARD_HEIGHT x BOARD_WIDTH to zero.
void generateGameBoard
(
        int board[BOARD_HEIGHT][BOARD_WIDTH]
);

// Returns a string converted from a 2D integer array of size BOARD_HEIGHT x BOARD_WIDTH
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

// Set every cell with content of blockNum to zero in a 2D integer array (typically board)
void clearShapeOnBoard(
        int blockNum, 
        int board[BOARD_HEIGHT][BOARD_WIDTH]
);

// Returns true if the rotation causes block placement conflicts. 
bool isCollidedWithAnotherBlock
(
        blockMatrix currentBlockMatrix, 
        int posy, 
        int posx,  
        int blockNum, 
        int board[BOARD_HEIGHT][BOARD_WIDTH]
);

// Used by requestRotate()
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