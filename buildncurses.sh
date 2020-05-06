#!/bin/bash

# If it is not downloaded
if [ ! -d ncurses ]; then
        git clone https://github.com/mirror/ncurses.git
fi

cd ncurses
# Referred from https://stackoverflow.com/questions/60325432/using-ncurses-without-installing-it
./configure --prefix $(pwd)/../local --enable-widec --with-pthread
make -j
make -j install