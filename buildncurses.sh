#!/bin/bash

# If it is not downloaded
if ! test -d ncurses; then
        git clone https://github.com/mirror/ncurses.git
fi

# Check for compiled ncurses
if test -f "ncurses/.ncurses_ready"; then

        # Validate whether the compiled binary is appliable
        if [[ $(cat ncurses/.ncurses_ready) != $(uname -a)  ]]; then
                clear
                echo "Recompiling ncurses required."
                echo "Please run make again by typing making again."
                echo "Press any key to continue"
                read X
                make clean_all
        fi

        echo "ncurses is ready"
        exit 0;
fi

cd ncurses
# Referred from https://stackoverflow.com/questions/60325432/using-ncurses-without-installing-it

if ! test -f .config; then
        ./configure --prefix $(pwd)/../local --enable-widec --with-pthread --cache-file=.config
fi 

make
make install

echo $(uname -a) > ".ncurses_ready"