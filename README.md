# Tetris Game Project

<img src="README.assets/image-20200509153732356.png" alt="image-20200509153732356" style="zoom:50%;" />

## Table of Contents

- [Tetris Game Project](#tetris-game-project)
   - [Group 140 Members](#group-140-members)
   - [Project Information](#project-information)
       - [Topic](#topic)
       - [Description](#description)
       - [Rules](#rules)
       - [Implementation](#implementation)   
   - [Running the code](#running-the-code)
       - [Requirements](#requirements)
       - [Compiling from source](#compiling-from-source)
       - [Compiling options](#compiling-options)
       - [Folder structure](#folder-structure)
   - [Compliance to Project Requirements](#compliance-to-project-requirements)
       - [Random Events](#random-events)
       - [Use of Arrays](#use-of-arrays)
       - [Dynamic Memory management](#dynamic-memory-management)
            - [struct declaration of Link Lists](#struct-declaration-of-link-lists)
            - [Usage scenario](#usage-scenario)
      - [File Management](#file-management)
      - [Modularity](#modularity)
          - [Ease of reading code](#ease-of-reading-code)
      - [In code comments](#in-code-comments)
   - [References](#references)


### Group 140 Members

1. Chow Nam Fung 
	- User ID: [EnLsGm](https://github.com/EnLsGm)
	- University Number: 3035712767
2. Tam Chun Kit
	- User ID: [Colton-Ko](https://github.com/Colton-Ko)
	- University Number: 3035686554

## Project Information

#### Topic

- Tetris 

#### Description

Tetris is a traditional and common game, where blocks in different shapes drop from the top of the screen, and the players should try to tessellate the dropped blocks. The player gets a credit whenever a row is filled with blocks.

#### Rules

1. The falling blocks are randomly determined.
2. If the stacked blocks exceeds the height limit in the game board, the game is over.

#### Implementation

1. [Random Events](#Random-Events)
	- The falling blocks are randomly determined.
2. [Use of Arrays](#Use-of-Arrays)
	-  The shape of the blocks and game board are stored and manipulated in arrays.
3. [Dynamic Memory management](#Dynamic-Memory-management)
  - The file loading and saving feature is implemented using dynamic memory.
4. [File Management](#File-Management)
  - Loading and saving score history involves file management.
5. [Modularity](#Modularity)
  - The code will be split into multiple files for better modularity and manageability.

## Running the code

### Requirements

1. Active Internet connection (during compilation)

2. Terminal size larger than dimension of 50x24

3. Running on either one of the environment below

   1. Linux with GCC installed (`g++`)
   2. macOS with Apple LLVM C++ compiler installed (`clang++`)
   3. FreeBSD with LLVM C++ compiler installed (`c++`)

   - Provided that you also have standard c library installed on your system.

   > **Note**
   >
   > Theoretically, Linux Subsystem on Windows (WSL) should also works. 
   > But it is not tested on that platform.
   >
   > ***A native executable for Windows is not available.***

### Compiling from source

To compile the code, simply run the following in your shell (Terminal emulator)

```sh
make
```

After compiling the sources, the executable is in `bin` folder. To run the game, type

```sh
./bin/tetris
```

> **Tip**
>
> If the folder path of your current working directory contains spaces, the compilation may fail.
> To solve this problem, try moving the repository to a folder with its path without spaces. For example, `~/`.

### Compiling options

- **Clean** existing binaries (Except `ncurses`)

  ```sh
  make clean
  ```

- **Clean all** existing binaries (Including `ncurses`)

  ```sh
  make clean_all
  ```
  
- Compile only `ncurses`

  ```sh
  make ncurses
  ```

> **Tip**
>
> If you run into a compilation problem. Always run `make clean` and try recompiling to see if it helps.

### Folder structure

| Folder           | Content                                                      |
| ---------------- | ------------------------------------------------------------ |
| `bin`            | The actual compiled executable file for tetris game. (`tetris`) |
| `obj`            | Object files emitted by compiler                             |
| `src`            | C++ source code for this project                             |
| `include/tetris` | Header files for this project                                |
| `local`          | Compiled `ncurses` library                                   |
| `ncurses`        | `ncurses` sources fetched on github                          |

## Compliance to Project Requirements

### Random Events

Random events are implemented during the block spawning process. The following code gives an example.

```c++
// Returns a integer randomly bonded by 0 and max.
int randomInteger(int max)
{
        int x = rand() % max;
	return x;
}

// Return a string which contained a block description
string spawnNewBlockString()
{
        string blocks[BLOCK_SHAPE_COUNT];
        getBlocksReady(blocks);
        
        return rotateBlock(randomInteger(ROTATIONS), blocks[randomInteger(BLOCK_SHAPE_COUNT)]);
}
```

(`game.cpp`)

The `randomInteger()` function returns a randomly-chosen index to determine which block to spawn. The random seed is defined at the initialization stage of game. 

This concludes the implementation of **random events**.

### Use of Arrays

Arrays are widely used throughout the game. For example, the representation of the tetris board is actually a fixed size integer array (`board[BOARD_HEIGHT][BOARD_WIDTH]`). A example code for its manipulation is as follows.

```c++
// Returns a string converted from a 2D integer array of size BOARD_HEIGHT x BOARD_WIDTH
string gameboardToString(int board[BOARD_HEIGHT][BOARD_WIDTH], int xmax)
{
        string output = "";
        for (int i = 0; i < BOARD_HEIGHT; ++i)
        {
                output;
                for (int j = 0; j < BOARD_WIDTH; ++j)
                {
                        output += to_string(board[i][j]);
                }
                output += " \n";
        }
        return output;
}
```

(`game.cpp`)

This function converts the tetris board from an integer array to a string. It converts its representative data type. The purpose of this conversion is to allow it to be printed onto a `ncurses` `WINDOW *`.

This concludes the implementation of the **use of arrays**.

### Dynamic Memory management

Dynamic memory management is implicitly used extensively due to the inclusion of `ncurses` library and `vector`. (Since the `WINDOW` in `ncurses` is actually a pointer). However, this project has explicit use of dynamic memory management in terms of link lists.

#### `struct` declaration of Link Lists

```c++
struct scoreEntry
{
        string dateTimeString;
        int playerScore;
        scoreEntry * next;
};
```

#### Usage scenario

```c++
// Appends a scoreEntry record link list. Receieves a scoreEntry pointer name head and tail
// and the integer of the score
void appendRecordToTable(string currentLocalTimeString, int score)
{
        // Steps to add a new record to tableOfScores
        // 1. Create new scoreEntry pointer
        // 2. Fill that pointer with dateTime, and score
        // 3. Put that to the end of of the linked list
        
        // New scoreEntry pointer
        scoreEntry * currentEntry = new scoreEntry;

        // Fill information to the currentEntryPointer
        currentEntry->playerScore       = score                         ;       // The score that player won
        currentEntry->dateTimeString    = currentLocalTimeString        ;

        currentEntry->next              = NULL                          ;

        // Append link List
        // Check whether HEAD is NULL. If it is then append that add that to HEAD.
        // Also the tail is the same entry

        if (head == NULL)               // If the head does not even exist
        {
                head = currentEntry;    // Set the head and tail to be the currenEntry
                tail = currentEntry;
        }
        else                            // If head exists
        {
                tail->next = currentEntry;      // Set the next item in the existing tail to currentEntry
                tail = currentEntry;            // Update the tail entry to be the current
        }
}
```

(`bookkeeper.cpp`)

Dynamic memory management is used for window management and managing the play history.

This concludes the use of **Dynamic Memory Management in this project**.

### File Management

File management is achieved in the mechanism of writing and reading play history records. The following gives an example of file management in this project.

```c++
// Writes a text file named HISTORY_FILENAME. Returns true if successful. 
// false if writing is not successful.
bool writeToHistoryTextFile(string dateTimeString, int playerScore)
{
        ofstream fout;                          // ofstream object for writing file
        fout.open(HISTORY_FILENAME,ios::app);   // Open the file as append mode
        if ( !fout )                            // If the file could not be opened
        {
                return false;                   // Return false for error handling
        }
        fout << dateTimeString + "," + to_string(playerScore) << endl;
        return true;
}
```

(`bookkeeper.cpp`)

The above code writes an entry about the current date and time in string, and an integer of player's score.

This concludes the **File management** capabilities in our game.

### Modularity

The code in our game is modular, since the code is split to multiple files where each file holds functions for their respective tasks.

```
[uname@hostname src]$ ls
blocks.cpp  bookkeeper.cpp  controller.cpp  debug.cpp  game.cpp  window.cpp
```

The source code is split across 6 cpp files with 8 header files.

#### Ease of reading code

The source code in this program tried to not to embed literals within the code. Instead, we used a header file called `constants.h` and put all constant literal with `#define` statements.

The advantage of doing so is to make the code more human readable, such that when you read the code, you know what the numbers and literals are, and what they are for.

For example,

```c++
// Clear a specified line
void clearLine(int lineNumber, int board[BOARD_HEIGHT][BOARD_WIDTH])
{
(...)
        if 
        (
                lineNumber == NO_LINE_TO_CLEAR || // Safety checking for invalid lines
                lineNumber < Y_LOWER_BOUNDARY  || // Too low
                lineNumber > Y_UPPER_BOUNDARY     // Line number does not even exist
(...)
```

If the `Y_LOWER_BOUNDARY` and `Y_UPPER_BOUNDARY` are changed to their static literals,
**It is much less intuitive and much less self-explanatory**

```c++
// Clear a specified line
void clearLine(int lineNumber, int board[BOARD_HEIGHT][BOARD_WIDTH])
{
(...)
        if 
        (
                lineNumber == -1			   || // Safety checking for invalid lines
                lineNumber < 0  			   || // Too low
                lineNumber > 20                   // Line number does not even exist
(...)
```

(`game.cpp`)

This creates confusion and unnecessary complexity for the code reader. Also, since the literals are not used only once. If an update for those value is need, it will be very tedious to lookup every one of them throughout the source.

### In code comments

In code comments are everywhere in the source code. Particularly, for every functions, there is at least a line of comment describing its purpose, input and output.

```c++
// Display a menu of choices, and returns an integer representing the user's choice
// string choices[]:    The menu to be printed
// int choicesCount:    The number of choices available
// int ymax:            Height of stdscr
// int xmax:            Width of stdscr
// -> _int:             The choice that user made
int showMenu(string choices[], int choicesCount, int ymax, int xmax);
```

(`window.cpp`)

## References

This code has referred and/or directly quoted code from the following sources

-  https://youtu.be/8OK8_tHeCIA

- https://stackoverflow.com/questions/20326356/how-to-remove-all-the-occurrences-of-a-char-in-c-string
- https://stackoverflow.com/questions/16909164/function-pointer-multiple-definition
- https://stackoverflow.com/questions/23970721/how-to-shift-2d-array-elements-down-in-c
- https://www.tutorialspoint.com/cplusplus/cpp_date_time.htm
- https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
- https://www.geeksforgeeks.org/csv-file-management-using-c/

## Tested Platforms 

1. macOS 10.15.4 Catalina (amd64)
2. Archlinux (Linux5.6 amd64)
3. Android (Termux, aarch64)
4. Ubuntu 18.04 (amd64)
5. FreeBSD 11.2 (amd64)

