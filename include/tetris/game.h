#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include "tetris/constants.h"
#include "tetris/types.h"

using namespace std;

string spawnNewBlock();
int randomInteger(int max);
void generateGameBoard(int board[BOARD_HEIGHT][BOARD_WIDTH]);
string gameboardToString(int board[BOARD_HEIGHT][BOARD_WIDTH], int xmax);
void addShapeToGameBoard(vector<vector<int>> blockMatrix, int posy, int posx, int board[BOARD_HEIGHT][BOARD_WIDTH]);
vector<vector<int>> shapeStringToArray(string shapeString, int blockNum, int ysize, int xsize);
void clearShapeOnBoard(int blockNum, int board[BOARD_HEIGHT][BOARD_WIDTH]);
#endif