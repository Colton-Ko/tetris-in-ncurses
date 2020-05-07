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
        This piece of code has direct reference from 
                -       https://stackoverflow.com/questions/16909164/function-pointer-multiple-definition

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
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

WINDOW * twin;
WINDOW * iwin;
WINDOW * dwin;

void drawBox(WINDOW * win)
{
        box(win, 0, 0);                                                // Draw outline box
        wrefresh(win);
}

void clearScreen(WINDOW * window, int posx, int posy)
{
        wclear(window);
        drawBox(window);
}


void exitOnSmallTerminal(int ymax, int xmax)
{
        // If you have a VT-80 terminal you can run this game
        mvprintw(ymax/2-1, abs(xmax-LEN_RUN_LARGE_TERM)/2, RUN_LARGE_TERM);
        mvprintw(ymax/2, abs(xmax-LEN_MIN_SIZE_PRMPT)/2, MIN_SIZE_PRMPT);
        mvprintw(ymax/2+1, abs(xmax-LEN_EXIT_ANY_KEY)/2, EXIT_ANY_KEY);

        getch();                                                        // Press any key to exit
        endwin();                                                       // Clear ncurses screen window
        exit(1);                                                        // Leave game due to insufficent space to draw game window
}

void printScore(WINDOW * iwin, int xmax, int score)
{
        string strscore = to_string(score);                             // Convert integer score to std::string
        mvwprintw(iwin,11, (xmax-strscore.size())/2,
                strscore.c_str());                                      // Place in middle
}

void printInstructionWindow(WINDOW * iwin, int xmax)
{
        // Prints the instruction window 
        mvwprintw(iwin, 1, (xmax-12)/2 ,"TETRIS GAME");                 // 1st line: Game Title
        mvwprintw(iwin, 3, 2,"KEYS");                                   // 2nd: Walkthrough on keys
                                                                        // Keys are same as in Minecraft my fav. game
        mvwprintw(iwin, 5, (xmax-10)/2, "W : Rotate");                  // W -> Rotate
        mvwprintw(iwin, 6, (xmax-10)/2, "A : Left");                    // A -> Left
        mvwprintw(iwin, 7, (xmax-10)/2, "S : Down");                    // S -> Down
        mvwprintw(iwin, 8, (xmax-10)/2, "D : Right");                   // D -> Right 

        mvwprintw(iwin, 10, 2, "SCORE");                                // Show score
        mvwprintw(iwin, 11, (xmax-1)/2, "0");                           // Initialization score is 0

        drawBox(iwin);
        wrefresh(iwin);
}


void drawFromGameBoard(WINDOW * twin, int board[BOARD_HEIGHT][BOARD_WIDTH])
{
        for (int i = 0; i < BOARD_HEIGHT; ++i)
        {
                for (int j = 0; j < BOARD_WIDTH; ++j)
                {
                        if (board[i][j])
                        {
                                wattron(twin, COLOR_PAIR(WHITE_BACKGROUND));                    // Enable printing a whole block "beautifully :)"
                                mvwprintw(twin, i + X_PADDING, j + Y_PADDING, "#");             // Full block just for this line
                                wattroff(twin, COLOR_PAIR(WHITE_BACKGROUND));                   // Turn off that beautify thing
                        }
                }
        }
        wrefresh(twin);
}

void startGame(WINDOW * twin, WINDOW * iwin, WINDOW * dwin, int ymax, int xmax)
{        
        // Spawn block
        string blocks[BLOCK_SHAPE_COUNT];
        string currentBlock = spawnNewBlock();

        // Declare variables
        int rotation = 0;                               // Stores Rotation angle, where angle = 90*rotation
        int blocksCount = 1;                            // Acts as the block Number
        block currentBlockObj;                          // Current blockObject/ blockShape
        block tempBlockObj;                             // Temporary block object
        vector< vector<int> > blockMatrix;              // 2D vector for storing the matrix verison of blockShape

        int gameTicksOver = 0;
        int level = INITIAL_LEVEL;
        bool forceDown = false;
        bool spawnNew = false;
        int lineToClear = -1;

        // Generate gameboard                        
        string boardStr = "";                        
        int board[BOARD_HEIGHT][BOARD_WIDTH];        
        generateGameBoard(board);                

        int posx = (xmax-currentBlockObj.xsize)/2, posy = 0;

        // Another thread is for moving block down every interval
        while (true)
        {
                mvwprintw(dwin, 5, 13, to_string(blocksCount).c_str());
                if (spawnNew)
                {
                        spawnNew = false;                                                       // Turn that flag off incase I forgot

                        // Check for filled line to clear
                        lineToClear = lookForFilledLine(board);
                        if (lineToClear != NO_LINE_TO_CLEAR)                                    // Need to clear that line
                        {

                        }


                        // Spawn new block
                        blocksCount += 1;                                                       // Increment blockNum to lock existing blocks
                        currentBlock = spawnNewBlock();                                         // Spawn new block and put in currentBlock (blockString)
                        blockMatrix = blockObjContToMatrix(currentBlockObj, blocksCount);       // Convert to blockMatrix

                        posy = 0;
                        posx = (xmax-currentBlockObj.xsize)/2;
                }

                this_thread::sleep_for(chrono::milliseconds(TIME_PER_TICK));            // Game tick is for 50ms by default
                gameTicksOver += 1;                                                     // Add 1 tick
                
                forceDown = (gameTicksOver == level);                                   // Level stores number of game tick for every force move down

                char x = getch();                                                       // Get user's control from keyboard
                clearScreen(twin, posx, posy);                                          // Clear TWIN window
                clearShapeOnBoard(blocksCount, board);                                  // Clear existing blockNum pieces in gameboard array

                blockStringToBlockObj(rotateBlock(rotation, currentBlock));              // Convert spawnedNewBlock to block object

                switch (x)
                {
                        case W_KEY:
                                tempBlockObj = blockStringToBlockObj(rotateBlock((rotation + 1) % 4, currentBlock));
                                requestRotate(tempBlockObj, rotation, posy, posx, blocksCount, board);
                                // mvwprintw(iwin, 19, (xmax-5)/2, (to_string(posx) + " " + to_string(posy)).c_str());
                                // mvwprintw(iwin, 20, (xmax-5)/2, (to_string(posx + tempBlockObj.xsize) + " " + to_string(posy + tempBlockObj.ysize)).c_str());
                                // mvwprintw(iwin, 15, (xmax-1)/2, to_string(rotation).c_str());
                                // mvwprintw(iwin, 14, (xmax-1)/2, "W");
                                break;

                        case A_KEY:
                                requestMoveLeft(currentBlockObj, posy, posx, blockMatrix, blocksCount, board);
                                // mvwprintw(iwin, 14, (xmax-1)/2, "A");
                                // mvwprintw(iwin, 19, (xmax-5)/2, (to_string(posx) + " " + to_string(posy)).c_str());
                                // mvwprintw(iwin, 20, (xmax-5)/2, (to_string(posx + currentBlockObj.xsize) + " " + to_string(posy + currentBlockObj.ysize)).c_str());
                                break;

                        case S_KEY:
                                spawnNew = !requestMoveDown(currentBlockObj, posy, posx, blockMatrix, blocksCount, board);
                                // mvwprintw(iwin, 14, (xmax-1)/2, "S");
                                // mvwprintw(iwin, 19, (xmax-5)/2, (to_string(posx) + " " + to_string(posy)).c_str());
                                // mvwprintw(iwin, 20, (xmax-5)/2, (to_string(posx + currentBlockObj.xsize) + " " + to_string(posy + currentBlockObj.ysize)).c_str());
                                break;
                        case D_KEY:
                                requestMoveRight(currentBlockObj, posy, posx, blockMatrix, blocksCount, board);
                                // mvwprintw(iwin, 14, (xmax-1)/2, "D");
                                // mvwprintw(iwin, 19, (xmax-5)/2, (to_string(posx) + " " + to_string(posy)).c_str());
                                // mvwprintw(iwin, 20, (xmax-5)/2, (to_string(posx + currentBlockObj.xsize) + " " + to_string(posy + currentBlockObj.ysize)).c_str());
                                break;                                                                
                }

                if (forceDown)        // Reached number of ticks
                {
                        gameTicksOver = 0;
                        if (!requestMoveDown(currentBlockObj, posy, posx, blockMatrix, blocksCount, board))
                        {
                                spawnNew = true;                                        // Lock in place by turning spawnNew flag
                        }
                        else                                                            // Can be moved down one block
                        {                                                               // Nothing happened, everything just like normal
                                continue;
                        }
                        
                        // mvwprintw(iwin, 14, (xmax-1)/2, "S");
                        // mvwprintw(iwin, 19, (xmax-5)/2, (to_string(posx) + " " + to_string(posy)).c_str());
                        // mvwprintw(iwin, 20, (xmax-5)/2, (to_string(posx + currentBlockObj.xsize) + " " + to_string(posy + currentBlockObj.ysize)).c_str());
                }

                currentBlockObj = blockStringToBlockObj(rotateBlock(rotation % 4, currentBlock));
                blockMatrix = blockObjContToMatrix(currentBlockObj, blocksCount);
                addShapeToGameBoard(blockMatrix, posy, posx, board, blocksCount);
                drawFromGameBoard(twin, board);
                updateDebug(board, dwin);
                box(iwin, 0, 0);
                wrefresh(iwin);
        }
}

void game_init()
{
        initscr();                                                      // Initialize screen
        srand(time(NULL));                                              // Initialize random seed
        start_color();                                                  // Full black background
        init_pair(WHITE_BACKGROUND, COLOR_WHITE, COLOR_WHITE);
        curs_set(0);                                                    // Hide cursor
        noecho();                                                       // No echo (@echo off)

        int ymax, xmax;                                                 // Variables for screen dimension
        getmaxyx(stdscr, ymax, xmax);                                   // Get screen dimension

        // Terminal is too small
        if (ymax < TWIN_WIDTH || xmax < TWIN_WIDTH+IWIN_WIDTH)
                exitOnSmallTerminal(ymax, xmax);
        
        twin = 
                newwin(TWIN_HEIGHT, TWIN_WIDTH, 0, 0);                  // twin stands for "Tetris Window"
        iwin = 
                newwin(IWIN_HEIGHT, IWIN_WIDTH, 0, TWIN_WIDTH);         // iwin stands for "Info. window"

        nodelay(stdscr, true);
        
        refresh();

        drawBox(twin);
        drawBox(iwin);
        

        dwin = newwin(DWIN_HEIGHT, DWIN_WIDTH, 0,
                        TWIN_WIDTH + IWIN_WIDTH);

        refresh();
        debugWindow(dwin);
        
        getmaxyx(iwin, ymax, xmax);
        printInstructionWindow(iwin, xmax);
        startGame(twin, iwin, dwin, ymax, xmax);

}

int main()
{
        game_init();
}