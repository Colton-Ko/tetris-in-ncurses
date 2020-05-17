/*
        HKU ENGG 1340 Programming and technologies
        Group 140 (2019-2020 2nd Semester)

        AUTHOR
        TAM CHUN KIT            (3035686554)
        CHOW NAM FUNG           (3035712767)

        DATE
        2020-05-07

        FILENAME
        controller.cpp          [MAIN FUNCTION IS DEFINED IN THIS FILE]

        REFERENCES

                This piece of code has direct reference from 
                -       https://stackoverflow.com/questions/16909164/function-pointer-multiple-definition

        VIEW
                Tabsize:        8
                Indentation:    Space

        PURPOSES

                This is the ViewController for the game. It is responsible for the following.
                1.      Passing game events to game logic part (game.cpp)
                2.      Initializing ncurses Window
                3.      Provision of User interface
                4.      Hosting the main gameloop
                5.      Provision of Entry point

        CHECKLIST

                - [X]   Fix File header
                - [X]   Add comment on every function in header
                - [X]   Add comment on every function in implementation file
                - [X]   Check header file name (Is it for another file?)
                - [X]   Check indentation consistency
*/

#include "tetris/blocks.h"
#include "tetris/controller.h"
#include "tetris/game.h"
#include "tetris/debug.h"
#include "tetris/options.h"
#include "tetris/constants.h"
#include "tetris/types.h"
#include "tetris/bookkeeper.h"
#include "tetris/window.h"

#include <ncurses.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

WINDOW * twin;  // Tetris Window
WINDOW * iwin;  // Information Window
WINDOW * dwin;  // Debug Window

// Show message if the terminal size is too small
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

// Returns the score based on number of lines cleared
int calculateScore(int score)
{
        return score*BOARD_WIDTH;
}


void printScore(int xmax, int score)
{
        string strscore = to_string(calculateScore(score));             // Convert integer score to std::string
        mvwprintw(iwin,11, (xmax-strscore.size())/2,
                strscore.c_str());                                      // Place in middle
}

// Prints instructions for controlling the game
void printInstructionWindow(int xmax)
{
        // Prints the instruction window 
        mvwprintw(iwin, 1, (xmax-LEN_GAME_TITLE)/2 ,GAME_TITLE);        // 1st line: Game Title
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

// Draw the tetris window from the board
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
                                        // Painting
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
                                // Painting
                                wattron(twin, COLOR_PAIR(board[i][j] % BLOCK_SHAPE_COUNT + 1));         // Enable printing a whole block "beautifully :) with colors"
                                mvwprintw(twin, i + X_PADDING, j + Y_PADDING, "#");                     // Full block just for this line
                                wattroff(twin, COLOR_PAIR(board[i][j] % BLOCK_SHAPE_COUNT + 1));        // Turn off that beautify thing
                        }
                }
        }
        wrefresh(twin);
}

// Set block position back to top and at middle
void resetPosition(int &posy, int &posx, block cb, int xmax)
{
        posy = 0;
        if (USE_1x2_BLOCK)              
                posx = (xmax/2-cb.xsize/2)/2;           // Bring posx back to middle
        else
                posx = (xmax-cb.xsize/2)/2;             // Bring posx back to middle
}

// Update the score and print it to IWIN
void updateScore(int linesCleared, int xmax)
{
        linesCleared = calculateScore(linesCleared);
        mvwprintw(
                                iwin, 
                                11, 
                                (xmax - to_string(linesCleared).length())/2, 
                                to_string(linesCleared).c_str());
}

// Spawn a new block
void spawnNewBlock(blockString &currentBlock, block &currentBlockObj, blockMatrix &bMatrix, int rotation, int blocksCount)
{
        currentBlock = spawnNewBlockString();                                           // Spawn new block and put in currentBlock (blockString)
        currentBlockObj = 
                 convertBlockStringToBlockObj(rotateBlock(rotation % 4, currentBlock)); // Update currentBlockObj
        bMatrix = convertToBlockMatrix(currentBlockObj, blocksCount);                   // Convert to blockMatrix
}

// Print game over prompt
void handleGameOver(int linesCleared, int ymax, int xmax)
{
        int score = calculateScore(linesCleared);
        gameOverMenu(score);
}

// Provides game tick for syncing game events and "gravity"
void doTick()
{
        this_thread::sleep_for(chrono::milliseconds(TIME_PER_TICK));
}

// Clear the screen buffer in tetris screen and draw boarders in it
void cleanTetrisScreenBuffer()
{
        werase(twin);           // Clear TWIN window
        box(twin, 0, 0);        // Redraw the boarder
}

// Print the current level of the game to IWIN
void printLevel(int linesCleared, int xmax)
{
        string levelPrompt = "LEVEL";
        string level = to_string(linesCleared/LEVELUP_PER_CLEARLINES + 1);
        mvwprintw(iwin, 12, 2, levelPrompt.c_str());
        mvwprintw(iwin, 13, (xmax- level.length())/2, level.c_str());
}

// ViewController of the game
void startGame(int ymax, int xmax)
{        
        // Spawn blocks
        blockString blocks[BLOCK_SHAPE_COUNT];
        blockString currentBlock = spawnNewBlockString();

        // Declare variables
        int rotation    = 0;                            // Stores Rotation angle, where angle = 90*rotation
        int blocksCount = 1;                            // Acts as the block Number
        block currentBlockObj;                          // Current blockObject/ blockShape
        block tempBlockObj;                             // Temporary block object
        blockMatrix bMatrix;                            // 2D vector for storing the matrix verison of blockShape

        // VARIABLES
        int gameTicksOver       = 0             ;       // Related to gamespeed
        int lineToClear         = -1            ;       // The line number in board to clear
        int linesCleared        = 0             ;       // Number of lines cleared (~= Score)
        int posx                = 0             ;       // X coordinate of block
        int posy                = 0             ;       // Y coordinate of block

        // FLAGS
        int level               = INITIAL_LEVEL ;       // The higher the level, the faster the game
        bool forceDown          = false         ;       // Whether a "gravity" pulls a block downward
        bool spawnNew           = true          ;       // Whether a new block needs to be spawned

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
                        spawnNew = false;                                       // Turn that flag off in case I forgot

                        // Check for filled line to clear
                        lineToClear = lookForFilledLine(board);
                        while (lineToClear != NO_LINE_TO_CLEAR)                 // Detect all lines to clear with While Loop
                        {
                                clearLine(lineToClear, board);                  // Clear that line
                                lineToClear = lookForFilledLine(board);         // Update new lineToClear
                                linesCleared += 1;
                                level =                                         // Update Level
                                        INITIAL_LEVEL - 
                                        (linesCleared/LEVELUP_PER_CLEARLINES);
                        }

                        printLevel(linesCleared, xmax);                         // Print current Level

                        // Update score
                        updateScore(linesCleared, xmax);

                        // Spawn new block
                        blocksCount += 1;                                       // Increment blockNum to lock existing blocks
                        spawnNewBlock                                           // Spawn a new block
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
                        if (!requestMoveDown(                                           // Validates the new moveDown request
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

                convertBlockStringToBlockObj(rotateBlock(rotation, currentBlock));      // Convert spawnedNewBlock to block object

                // Controller
                switch (x)
                {
                        case W_KEY:     // W key is rotate
                                tempBlockObj = convertBlockStringToBlockObj(rotateBlock((rotation + 1) % 4, currentBlock));
                                requestRotate(tempBlockObj, rotation, posy, posx, blocksCount, board);
                                break;

                        case A_KEY:     // A key is left
                                requestMoveLeft(currentBlockObj, posy, posx, bMatrix, blocksCount, board);
                                break;

                        case S_KEY:     // S key is down
                                spawnNew = !requestMoveDown(currentBlockObj, posy, posx, bMatrix, blocksCount, board);
                                break;
                        case D_KEY:     // D key is right
                                requestMoveRight(currentBlockObj, posy, posx, bMatrix, blocksCount, board);
                                break;                                                                
                }

                if (forceDown)                           // Reached number of ticks
                {
                        gameTicksOver = 0;
                        if (!requestMoveDown(currentBlockObj, posy, posx, bMatrix, blocksCount, board))
                                spawnNew = true;        // Lock in place by turning spawnNew flag
                        else
                                continue;
                }

                // Add the current block to TWIN window (view)
                currentBlockObj = convertBlockStringToBlockObj(rotateBlock(rotation % 4, currentBlock));
                bMatrix = convertToBlockMatrix(currentBlockObj, blocksCount);
                addShapeToGameBoard(bMatrix, posy, posx, board, blocksCount);
                drawFromGameBoard(board);
                updateDebug(board);
                box(iwin, 0, 0);
                wrefresh(iwin);
        }
}

// Initialize colors for painting
void initializeColors()
{
        // Color pairs for blocks
        init_pair(RED_BACKGROUND, COLOR_RED, COLOR_RED);
        init_pair(YELLOW_BACKGROUND, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(GREEN_BACKGROUND, COLOR_GREEN, COLOR_GREEN);
        init_pair(CYAN_BACKGROUND, COLOR_CYAN, COLOR_CYAN);
        init_pair(BLUE_BACKGROUND, COLOR_BLUE, COLOR_BLUE);
        init_pair(MAGENTA_BACKGROUND, COLOR_MAGENTA, COLOR_MAGENTA);
        init_pair(WHITE_BACKGROUND, COLOR_WHITE, COLOR_WHITE);
}

// Set up Window parameters
void setupWindow()
{
        start_color();                                                  // Full black background
        initializeColors();                                             // Initialize Colors
        curs_set(0);                                                    // Hide cursor
        noecho();                                                       // No echo (@echo off)
}

// Initialize the game
void gameInit()
{
        initscr();                                                      // Initialize screen
        srand(time(0));                                                 // Initialize random seed

        setupWindow();
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

        box(twin, 0, 0);                                                // Draw boarders for Tetris Window
        box(iwin, 0, 0);                                                // Draw boarders for Information Window

        wrefresh(twin);                                                 // Refresh
        wrefresh(iwin);                                                 // Refresh
        
        if (DEBUG)
        {
                dwin = newwin(DWIN_HEIGHT, DWIN_WIDTH, 0,               // Create new debug window if DEBUG is set
                                TWIN_WIDTH + IWIN_WIDTH);
        }

        refresh();
        debugWindow();
        
        getmaxyx(iwin, ymax, xmax);
        printInstructionWindow(xmax);                                   // Print instructions
        startGame(ymax, xmax);                                          // Start game logic part

}

// Entry Point
int main()
{
        gameInit();                                                    // Initialize game
}