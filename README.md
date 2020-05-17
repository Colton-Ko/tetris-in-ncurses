# Ncurses Tetris

<img src="README.assets/image-20200509153732356.png" alt="image-20200509153732356" style="zoom:50%;" />

## Running the code

### Requirements

1. Active Internet connection (during compilation)

2. Terminal size larger than dimension of 50x24

3. Running on either one of the environment below

   1. Linux with GCC installed (`g++`)
   2. macOS with Apple LLVM C++ compiler installed (`clang++`)
   3. FreeBSD with LLVM C++ compiler installed (`c++`)

   - Provided that you also have standard c library installed on your system.


### Compiling from source

#### Tested Platforms

These platforms are tested to be working.

1. Mac OS X
2. Windows Subsystem for Linux (WSL)
3. Linux
4. FreeBSD
5. Android (Termux)
6. iOS 13 (Jailbroken, Unc0ver, checkra1n)

#### For Mac OS X/macOS, WSL, Linux, FreeBSD, Android (Termux)

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

#### For Jailbroken iOS/iPadOS (64 bit only)

Compiling on jailbroken iOS devices is a bit different than on normal platform. We shall use the Compiled ncurses library supplied by Cydia and link against it.

To compile, run the following command first.

```shell
bash getiOSSDK.sh
```

The above command will set up the environment ready for compiling on iOS. After that, run

```shell
make
```

It will finish the compilation and linking for you.

### Compiling options

- **Clean** existing binaries (Except `ncurses`)

  ```sh
  make clean
  ```

- **Clean all** existing binaries (Including `ncurses`)

  ```sh
  make clean_all
  ```
  
- Compile only `ncurses` **(Not appliable for Jailbroken iOS)**

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

