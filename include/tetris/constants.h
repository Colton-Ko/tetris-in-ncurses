/*

        DATE
        2020-05-07
        
        FILENAME
        constants.h
        
        VIEW
                Tabsize:        8
                Indentation:    Space
                
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H 

#include "tetris/options.h"

#define GAMEINFO                "GAME RULES"
#define GAME_TITLE              "TETRIS GAME"
#define LEN_GAME_TITLE          11


#define TWIN_HEIGHT             24
#define TWIN_WIDTH              23

#define IWIN_HEIGHT             24
#define IWIN_WIDTH              23

#define DWIN_HEIGHT             24
#define DWIN_WIDTH              23

#define BOARD_HEIGHT            22
#define BOARD_WIDTH             20

#if USE_1x2_BLOCK == 1
    #undef BOARD_WIDTH
    #define BOARD_WIDTH         10
    #define DRAW_WIDTH          20
#endif

#define W_KEY                   119
#define A_KEY                   97
#define S_KEY                   115
#define D_KEY                   100

#define ROTATIONS               4
#define TIME_PER_TICK           50
#define INITIAL_LEVEL           15

#define BLOCK_SHAPE_COUNT       7
#define BLOCK_WIDTH             4
#define BLOCK_WIDTH_SQR         16

#define LEVELUP_PER_CLEARLINES  3

#define SHAPE_0                 "..#...#...#...#."
#define SHAPE_1                 ".##..#...#......"
#define SHAPE_2                 ".#...#...##....."
#define SHAPE_3                 "..#..##..#......"
#define SHAPE_4                 ".#...##...#....."
#define SHAPE_5                 ".....##..##....."
#define SHAPE_6                 ".#...##..#......"

#define BLOCK_EMPTY_LINE        "...."
#define OCCUPIED_CHAR           '#'

#define DELETE_CHAR             'X'

#define EMPTY_BLOCK_NUM         0
#define X_PADDING               1
#define Y_PADDING               1

#define DIRECTION_DOWN          0
#define DIRECTION_LEFT          1
#define DIRECTION_RIGHT         2

#define LEN_RUN_LARGE_TERM      41
#define LEN_MIN_SIZE_PRMPT      21
#define LEN_EXIT_ANY_KEY        22

#define NO_LINE_TO_CLEAR        -1

#define X_UPPER_BOUNDARY        BOARD_WIDTH
#define X_LOWER_BOUNDARY        0
#define Y_UPPER_BOUNDARY        BOARD_HEIGHT
#define Y_LOWER_BOUNDARY        0

#define RUN_LARGE_TERM          "Please this program in a larger terminal!"
#define MIN_SIZE_PRMPT          "Minimum size is 50x24"
#define EXIT_ANY_KEY            "Press any key to exit"

#define EXIT_ENTER_KEY          "Press Spacebar to exit"

#define END_GAME                "Game Over!"
#define LEN_END_GAME            10
#define YOUR_SCORE              "Your score is "
#define LEN_YOUR_SCORE          14

// Block colors
#define RED_BACKGROUND          2
#define YELLOW_BACKGROUND       3
#define GREEN_BACKGROUND        4
#define CYAN_BACKGROUND         5
#define BLUE_BACKGROUND         6
#define MAGENTA_BACKGROUND      7
#define WHITE_BACKGROUND        1

#define HISTORY_FILENAME        "tetris_high_scores.txt"
#define SEPARATION_CHAR         ','
#define OPERATION_FAILED        -1

#define CHOICE_PLAY_AGAIN       0
#define CHOICE_SEE_HIGH         1
#define CHOICE_LEAVE_GAME       2

#define PLAY_AGAIN              (string) " Play again " 
#define LEAVE_GAME              (string) " Leave game "
#define END_GAME_CHOICES        3

#define Y_MENUSIZE_PADDING      2
#define X_MENUSIZE_PADDING      2

#define KEY_SPACE               ' '

#define END_GAME_MENU_CHOICES   2
#define PLAY_AGAIN_CHOICE       0
#define END_GAME_CHOICE         1

#endif
