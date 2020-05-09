#include "tetris/blocks.h"
#include "tetris/controller.h"
#include "tetris/game.h"
#include "tetris/debug.h"
#include "tetris/options.h"
#include "tetris/constants.h"
#include "tetris/types.h"

#include <ncurses.h>
#include <string>
#include <iostream>

// Returns the length of the longest string in a string array
// string strarr[]:     The string array to be inspected
// int size:            The number of strings in strarr
int getMaxLength(string strarr[], int size)
{
        int currentLength = strarr[0].length();
        for (int i = 0; i < size; ++i)
        {
                if (strarr[i].length() > currentLength)
                {
                        currentLength = strarr[i].length();
                }
        }
        return currentLength + X_MENUSIZE_PADDING;
}

// Display a menu of choices, and returns an integer representing the user's choice
// string choices[]:    The menu to be printed
// int choicesCount:    The number of choices available
// int ymax:            Height of stdscr
// int xmax:            Width of stdscr
// -> _int:             The choice that user made
int showMenu(string choices[], int choicesCount, int ymax, int xmax)
{
        const int MENU_YSIZE = choicesCount;
        const int MENU_XSIZE = getMaxLength(choices, choicesCount);

        WINDOW * menuWin =                              // Draws a new Window
                newwin                                  // Called menuWin
                (                                       // Where the dimensions is
                        MENU_YSIZE+Y_MENUSIZE_PADDING,  // Menu height and y-padding
                        MENU_XSIZE+X_MENUSIZE_PADDING,  // Menu width and x-padding
                        (ymax-MENU_YSIZE)/2,            // Place in middle of screen (Center Y)
                        (xmax-MENU_XSIZE)/2             // Place in middle of screen (Center X)
                );

        box(menuWin, 0, 0);                             // Draw a boarder box around the menuWindow

        getmaxyx(menuWin, ymax, xmax);                  // Get dimensions of menuWin

        for (int i = 0; i < choicesCount; ++i)          // Print all choices 
                mvwprintw
                (
                        menuWin,                        // MenuWindow
                        i+1,                            // Offset of each line is by 1
                        (xmax-choices[i].length())/2,   // Place in middle (CenterX)
                        choices[i].c_str()              // Choice content
                );
        
        keypad(stdscr, true);

        wrefresh(menuWin);                              // Refresh the menuWin

        int choice = 0;                                 // Set initial choice as 0 (Tempt user to play once more :) )


        int response = getch();                                // Get the character that user fed
        while (response != KEY_SPACE)                           // If space is not pressed
        {
                switch (response)                               // Handle the response
                {
                        case W_KEY:
                                if (choice > 0)
                                        choice-=1;
                                break;

                        case S_KEY:
                                if (choice < choicesCount - 1)
                                        choice+=1;
                                break;

                        case KEY_UP:
                                if (choice > 0)
                                        choice-=1;
                                break;

                        case KEY_DOWN:
                                if (choice < choicesCount - 1)
                                        choice+=1;
                                break;
                }

                for (int i = 0; i < 3; ++i)
                {
                        mvwprintw(menuWin, i+1, (xmax-choices[i].length())/2, choices[i].c_str());
                        if (i == choice)
                        {
                                wattron(menuWin, A_REVERSE);
                                mvwprintw(menuWin, i+1, (xmax-choices[i].length())/2, choices[i].c_str());
                                wattroff(menuWin, A_REVERSE);
                        }
                }
                
                wrefresh(menuWin);
                response = getch();
        }
        keypad(stdscr, false);
        wclear(menuWin);
        delwin(menuWin);
        return choice;
}

// Notify user that the game over. Display a menu for handling consequence user's actions.
// int score:   The score in previous math
// -> void
void gameOverMenu(int score)
{
        delwin(twin);                           // Release memory from TWIN
        delwin(iwin);                           // Release memory from IWIN
        clear();                                // Clears the standard screen Window

        int ymax, xmax;                         // Stores the screen dimension info
        getmaxyx(stdscr, ymax, xmax);           // ymax = height, xmax = width
        
        // Check high score

        string gameOverLabel = 
                " Game over ";

        string yourScoreLabel =
                "Your score is " + 
                to_string(score) + 
                ".";

        string submitHint =                     // Prints a hint for user
                "Press Spacebar to select your choice.";
        
        string controlKeysHint =
                "Press W to move up. S to move down.";


        const int MENU_YSIZE = END_GAME_CHOICES;// Specify the height of endGameMenu to be number of choices

        attron(A_REVERSE);
        mvwprintw                               // Prints a hint for keys to select choices
        (
                stdscr, 
                (ymax-MENU_YSIZE)/2-7 , 
                (xmax-gameOverLabel.length())/2, 
                gameOverLabel.c_str()
        );
        attroff(A_REVERSE);

        mvwprintw                               // Prints a hint for keys to select choices
        (
                stdscr, 
                (ymax-MENU_YSIZE)/2-5 , 
                (xmax-yourScoreLabel.length())/2, 
                yourScoreLabel.c_str()
        );


        mvwprintw                               // Prints a hint for keys to select choices
        (
                stdscr, 
                (ymax-MENU_YSIZE)/2-3 , 
                (xmax-submitHint.length())/2, 
                submitHint.c_str()
        );

        mvwprintw                               // Prints a hint for keys to select choices
        (
                stdscr, 
                (ymax-MENU_YSIZE)/2-2 , 
                (xmax-controlKeysHint.length())/2, 
                controlKeysHint.c_str()
        );

        refresh();                              // Refresh the standard screen such that the changes are applied

        string  endGameMenu[] =                 // Create end game menu
                {
                        PLAY_AGAIN, 
                        SEE_SCORES,
                        LEAVE_GAME
                };

        int choice = 
                showMenu                        // Ask for user's choice
                (
                        endGameMenu,            // The menu to be printed
                        END_GAME_MENU_CHOICES,  // The number of choices available
                        ymax,                   // Height of stdscr
                        xmax                    // Width of stdscr
                );     

        switch(choice)
        {
                case (PLAY_AGAIN_CHOICE):
                        clear();
                        gameInit();
                        break;

                case (SEE_SCORES_CHOICE):
                        endwin();
                        cout << "NOT DONE!" << endl;
                        exit(0);
                        break;

                case (END_GAME_CHOICE):
                        endwin();
                        cout << "Had a great time with you. See you ! :)" << endl;
                        exit(0);
                        break;
        }
}