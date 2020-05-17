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

   - Provided that you also have standard c (and c++) library installed on your system.


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



## References

This code has referred and/or directly quoted code from the following sources

- Ncurses library

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
6. iOS 13 (aarch64, Jailbroken)
7. Windows subsystem for Linux (WSL)

