#ifndef GAME_H
#define GAME_H

#include <string>
#include "controller.h"
using namespace std;

#define ROTATIONS 4
#define BOARD_HEIGHT TWIN_HEIGHT-2
#define BOARD_WIDTH TWIN_WIDTH/2

string spawnNewBlock();
int randomInteger(int max);
void generateGameBoard(int board[BOARD_HEIGHT][BOARD_WIDTH]);
string gameboardToString(int board[BOARD_HEIGHT][BOARD_WIDTH], int xmax);

#endif