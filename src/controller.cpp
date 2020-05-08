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

void printScore(int xmax, int score)
{
        string strscore = to_string(score);                             // Convert integer score to std::string
        mvwprintw(iwin,11, (xmax-strscore.size())/2,
                strscore.c_str());                                      // Place in middle
}

void printInstructionWindow(int xmax)
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

        box(iwin, 0, 0);
        wrefresh(iwin);
}

void drawFromGameBoard(int board[BOARD_HEIGHT][BOARD_WIDTH])
{
        #if USE_1x2_BLOCK == 1
        if (USE_1x2_BLOCK)                              // If instructed to use 1x2 drawing mode, y=1, x=2
        {
                int visual[BOARD_HEIGHT][DRAW_WIDTH];   // Copy array first

                for (int i = 0; i < BOARD_HEIGHT; ++i)
                {
                        for (int j = 0; j < BOARD_WIDTH; ++j)
                        {
                                visual[i][2*j] = board[i][j];
                                visual[i][2*j+1] = board[i][j];
                        }
                        for (int j = 0; j < DRAW_WIDTH; ++j)
                        {
                                if (visual[i][j])
                                {
                                wattron(twin, COLOR_PAIR(visual[i][j] % BLOCK_SHAPE_COUNT + 1));        // Enable printing a whole block "beautifully :) with colors"
                                mvwprintw(twin, i + X_PADDING, j + Y_PADDING, "#");                     // Full block just for this line
                                wattroff(twin, COLOR_PAIR(visual[i][j] % BLOCK_SHAPE_COUNT + 1));       // Turn off that beautify thing
                                }
                        }
                }
                wrefresh(twin);
                return;
        }
        #endif

        for (int i = 0; i < BOARD_HEIGHT; ++i)
        {
                for (int j = 0; j < BOARD_WIDTH; ++j)
                {
                        if (board[i][j])
                        {
                                wattron(twin, COLOR_PAIR(board[i][j] % BLOCK_SHAPE_COUNT + 1));         // Enable printing a whole block "beautifully :) with colors"
                                mvwprintw(twin, i + X_PADDING, j + Y_PADDING, "#");                     // Full block just for this line
                                wattroff(twin, COLOR_PAIR(board[i][j] % BLOCK_SHAPE_COUNT + 1));        // Turn off that beautify thing
                        }
                }
        }
        wrefresh(twin);
}

void resetPosition(int &posy, int &posx, block cb, int xmax)
{
        posy = 0;
        if (USE_1x2_BLOCK)
                posx = (xmax/2-cb.xsize/2)/2;
        else
                posx = (xmax-cb.xsize/2)/2;
}

void updateScore(int linesCleared, int xmax)
{
        mvwprintw(
                                iwin, 
                                11, 
                                (xmax - to_string(linesCleared).length())/2, 
                                to_string(linesCleared).c_str());
}

void spawnNewBlock(string &currentBlock, block &currentBlockObj, blockMatrix &bMatrix, int rotation, int blocksCount)
{
        currentBlock = spawnNewBlockString();                                   // Spawn new block and put in currentBlock (blockString)
        currentBlockObj = 
                 convertBlockStringToBlockObj(rotateBlock(rotation % 4, currentBlock)); // Update currentBlockObj
        bMatrix = convertToBlockMatrix(currentBlockObj, blocksCount);           // Convert to blockMatrix
}

void handleGameOver(int linesCleared, int ymax, int xmax)
{
        // Show game over
        werase(twin);
        werase(iwin);
        werase(dwin);

        wrefresh(twin);
        wrefresh(iwin);
        wrefresh(dwin);

        getmaxyx(stdscr, ymax, xmax);

        string score (YOUR_SCORE);
        score += to_string(linesCleared);

        string exitEnter (EXIT_ENTER_KEY);

        clear();

        mvprintw(ymax/2-1, (xmax-LEN_END_GAME)/2 , END_GAME);
        mvprintw(ymax/2, (xmax-score.length())/2 , score.c_str());
        mvprintw(ymax/2+1, (xmax-exitEnter.length())/2 , exitEnter.c_str());

        nodelay(stdscr, false);
        refresh();

        while(getch() != ' ')
        {
                clear();

                mvprintw(ymax/2-1, (xmax-LEN_END_GAME)/2 , END_GAME);
                mvprintw(ymax/2, (xmax-score.length())/2 , score.c_str());
                mvprintw(ymax/2+1, (xmax-exitEnter.length())/2 , exitEnter.c_str());

                nodelay(stdscr, false);
                refresh();
        }

        endwin();
        exit(0);
}

void doTick()
{
        this_thread::sleep_for(chrono::milliseconds(TIME_PER_TICK));
}

void cleanTetrisScreenBuffer()
{
        werase(twin);                                                           // Clear TWIN window
        box(twin, 0, 0);                                                        // Redraw the boarder
}

void startGame(int ymax, int xmax)
{        
        // Spawn blocks
        string blocks[BLOCK_SHAPE_COUNT];
        string currentBlock = spawnNewBlockString();

        // Declare variables
        int rotation    = 0;                            // Stores Rotation angle, where angle = 90*rotation
        int blocksCount = 1;                            // Acts as the block Number
        block currentBlockObj;                          // Current blockObject/ blockShape
        block tempBlockObj;                             // Temporary block object
        blockMatrix bMatrix;                            // 2D vector for storing the matrix verison of blockShape

        int gameTicksOver       = 0             ;       // Related to gamespeed
        int level               = INITIAL_LEVEL ;       // The higher the level, the faster the game
        bool forceDown          = false         ;       // Whether a "gravity" pulls a block downward
        bool spawnNew           = false         ;       // Whether a new block needs to be spawned
        int lineToClear         = -1            ;       // The line number in board to clear
        int linesCleared        = 0             ;       // Number of lines cleared (~= Score)
        int posx                = 0             ;       // X coordinate of block
        int posy                = 0             ;       // Y coordinate of block

        string boardStr         = ""            ;       // String representation of block

        // Create board Array (2D)
        int board[BOARD_HEIGHT][BOARD_WIDTH];    

        generateGameBoard(board);                               // Fill with zeros
        resetPosition(posy, posx, currentBlockObj, xmax);       // Reset posy posx

        // Another thread is for moving block down every interval
        while (true)
        {

                if (spawnNew)
                {
                        spawnNew = false;                                                       // Turn that flag off incase I forgot

                        // Check for filled line to clear
                        lineToClear = lookForFilledLine(board);
                        while (lineToClear != NO_LINE_TO_CLEAR)                                 // Detect all lines to clear with While Loop
                        {
                                clearLine(lineToClear, board);                                  // Clear that line
                                lineToClear = lookForFilledLine(board);                         // Update new lineToClear
                                linesCleared += 1;
                        }

                        // Update score
                        updateScore(linesCleared, xmax);

                        // Spawn new block
                        blocksCount += 1;                                                       // Increment blockNum to lock existing blocks
                        spawnNewBlock
                        (
                                currentBlock, 
                                currentBlockObj, 
                                bMatrix, 
                                rotation, 
                                blocksCount
                        );

                        // Reset posx posy
                        resetPosition(posy, posx, currentBlockObj, xmax);

                        // Check Whether new block is spawned valid
                        if (!requestMoveDown(
                                currentBlockObj, 
                                posy, 
                                posx, 
                                bMatrix, 
                                blocksCount, 
                                board))                                                 // If collided with block below
                        {
                                handleGameOver(linesCleared, ymax, xmax);
                        }
                }

                doTick();                                                               // Game tick is for 50ms by default
                gameTicksOver += 1;                                                     // Add 1 tick
                
                forceDown = (gameTicksOver == level);                                   // Level stores number of game tick for every force move down

                char x = getch();                                                       // Get user's control from keyboard
                cleanTetrisScreenBuffer();                                              // Clear Tetris Screen buffer
                clearShapeOnBoard(blocksCount, board);                                  // Clear existing blockNum pieces in gameboard array

                convertBlockStringToBlockObj(rotateBlock(rotation, currentBlock));             // Convert spawnedNewBlock to block object

                switch (x)
                {
                        case W_KEY:
                                tempBlockObj = convertBlockStringToBlockObj(rotateBlock((rotation + 1) % 4, currentBlock));
                                requestRotate(tempBlockObj, rotation, posy, posx, blocksCount, board);
                                break;

                        case A_KEY:
                                requestMoveLeft(currentBlockObj, posy, posx, bMatrix, blocksCount, board);
                                break;

                        case S_KEY:
                                spawnNew = !requestMoveDown(currentBlockObj, posy, posx, bMatrix, blocksCount, board);
                                break;
                        case D_KEY:
                                requestMoveRight(currentBlockObj, posy, posx, bMatrix, blocksCount, board);
                                break;                                                                
                }

                if (forceDown)        // Reached number of ticks
                {
                        gameTicksOver = 0;
                        if (!requestMoveDown(currentBlockObj, posy, posx, bMatrix, blocksCount, board))
                                spawnNew = true;                                        // Lock in place by turning spawnNew flag
                        else
                                continue;
                }

                currentBlockObj = convertBlockStringToBlockObj(rotateBlock(rotation % 4, currentBlock));
                bMatrix = convertToBlockMatrix(currentBlockObj, blocksCount);
                addShapeToGameBoard(bMatrix, posy, posx, board, blocksCount);
                drawFromGameBoard(board);
                updateDebug(board);
                box(iwin, 0, 0);
                wrefresh(iwin);
        }
}

void game_init()
{
        initscr();                                                      // Initialize screen
        srand(time(0));                                                 // Initialize random seed
        start_color();                                                  // Full black background
        
        init_pair(RED_BACKGROUND, COLOR_RED, COLOR_RED);
        init_pair(YELLOW_BACKGROUND, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(GREEN_BACKGROUND, COLOR_GREEN, COLOR_GREEN);
        init_pair(CYAN_BACKGROUND, COLOR_CYAN, COLOR_CYAN);
        init_pair(BLUE_BACKGROUND, COLOR_BLUE, COLOR_BLUE);
        init_pair(MAGENTA_BACKGROUND, COLOR_MAGENTA, COLOR_MAGENTA);
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


        box(twin, 0, 0);
        box(iwin, 0, 0);

        wrefresh(twin);
        wrefresh(iwin);
        
        if (DEBUG)
        {
                dwin = newwin(DWIN_HEIGHT, DWIN_WIDTH, 0,
                                TWIN_WIDTH + IWIN_WIDTH);
        }

        refresh();
        debugWindow();
        
        getmaxyx(iwin, ymax, xmax);
        printInstructionWindow(xmax);
        startGame(ymax, xmax);

}

int main()
{
        game_init();
}