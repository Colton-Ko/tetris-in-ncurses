# ENGG1340 Group140
### Group Members

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

1. Random Events
	- The falling blocks are randomly determined.
2. Arrays
	-  The shape of the blocks and game board are stored and manipulated in arrays.
3. Dynamic Memory management
	- The file loading and saving feature is implemented using dynamic memory.
4. File Management
	- Loading and saving score history involves file management.
5. Modularity
	- The code will be split into multiple files for better modularity and manageability.

## Running the code

### Requirements

1. Active internet connection (during compiling)
2. Terminal size larger than dimension of 50x24

### Compilation from source

To compile the code, simply run the following in your shell (Terminal emulator)

```sh
make
```

After compiling the sources, the executable is in `bin` folder. To run the game, type

```sh
./bin/tetris
```

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

