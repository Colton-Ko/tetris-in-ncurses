/*
        HKU ENGG 1340 Programming and technologies
        Group 140 (2019-2020 2nd Semester)

        AUTHOR
        TAM CHUN KIT            (3035686554)
        CHOW NAM FUNG           (3035712767)

        DATE
        2020-05-09

        FILENAME
        window.cpp          [MAIN FUNCTION IS DEFINED IN THIS FILE]

        VIEW
                Tabsize:        8
                Indentation:    Space

        PURPOSES

                This file handles specialized window operations involving ncurses throughout the game
                1.      Displaying menus
                2.      Handling menu operations for different game events
                        2.1     End Game
                        2.2     Show score history

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
#include "tetris/gamerecord.h"

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
        
        string gameOverLabel =              
                " Game over ";

        string yourScoreLabel =
                "Your score is " + 
                to_string(score);

        string submitHint = 
                "Press Spacebar to select your choice";
        
        string controlKeysHint =
                "Press W to move up, Press S to move down";


        const int MENU_YSIZE = END_GAME_CHOICES;// Specify the height of endGameMenu to be number of choices

        attron(A_REVERSE);
        mvwprintw                               // Prints a Title (Game over)
        (
                stdscr, 
                (ymax-MENU_YSIZE)/2-5 , 
                (xmax-gameOverLabel.length())/2, 
                gameOverLabel.c_str()
        );
        attroff(A_REVERSE);

        mvwprintw                               // Prints a hint for play scores
        (
                stdscr, 
                (ymax-MENU_YSIZE)/2-2, 
                (xmax-yourScoreLabel.length())/2, 
                yourScoreLabel.c_str()
        );

        // Check high score
        int highScore = readHistoryFile();
        if (highScore < score)
        {
                string newHighScore = "0w0 NEW HIGH SCORE!";
                attron(A_BLINK);
                mvwprintw                               // Print new high score hint
                (
                        stdscr, 
                        (ymax-MENU_YSIZE)/2-3 , 
                        (xmax-newHighScore.length())/2, 
                        newHighScore.c_str()
                );
                attroff(A_BLINK);
        }

        mvwprintw                               // Prints a hint for keys to submit choices
        (
                stdscr, 
                (ymax-MENU_YSIZE)/2+MENU_YSIZE+4, 
                (xmax-submitHint.length())/2, 
                submitHint.c_str()
        );

        mvwprintw                               // Prints a hint for keys to select choices
        (
                stdscr, 
                (ymax-MENU_YSIZE)/2+MENU_YSIZE+5 , 
                (xmax-controlKeysHint.length())/2, 
                controlKeysHint.c_str()
        );

        refresh();                              // Refresh the standard screen such that the changes are applied

        string  endGameMenu[] =                 // Create end game menu
                {
                        PLAY_AGAIN, 
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

        bool notifyWriteFailed = !writeToHistoryTextFile(getCurrentDateTime(), score);

        switch(choice)
        {
                case (PLAY_AGAIN_CHOICE):
                        clear();
                        gameInit();
                        break;

                case (END_GAME_CHOICE):
                        endwin();
                        cout << "Had a great time with you. See you ! =w=" << endl;
                        if (notifyWriteFailed)
                        {
                                cout << "We could not write the score history file." << endl;
                        }
                        exit(0);
                        break;
        }
}