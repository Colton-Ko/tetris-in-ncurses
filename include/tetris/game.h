#ifndef GAME_H
#define GAME_H

#include <string>
#include "tetris/controller.h"
#include "tetris/blocks.h"

using namespace std;

#define ROTATIONS 4

string spawnNewBlock();
int randomInteger(int max);
void generateGameBoard(int board[BOARD_HEIGHT][BOARD_WIDTH]);
string gameboardToString(int board[BOARD_HEIGHT][BOARD_WIDTH], int xmax);
void addShapeToGameBoard(int block[BLOCK_WIDTH][BLOCK_WIDTH], int ysize, int xsize, int posy, int posx, int board[BOARD_HEIGHT][BOARD_WIDTH]);
void shapeStringToArray(string shapeString, int iteration, int block[BLOCK_WIDTH][BLOCK_WIDTH]);
void clearShapeOnBoard(int blockNum, int board[BOARD_HEIGHT][BOARD_WIDTH]);
#endif