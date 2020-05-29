#!/bin/bash
SDKURL="https://github.com/theos/sdks/archive/master.zip"
SDKZIP=master.zip
NCURSES_DIR="/usr/lib/libncursesw.dylib"
PROJECTDIR=$(basename "$PWD")
PACKAGES="wget clang libclang-common-10-dev libclang-cpp10 libc++-dev make unzip"
SDK_DIR="../sdks-master"

# Check privilleges
if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

# Install packages
# apt update
echo "Installing necessary packages..."
apt install $PACKAGES 1>/dev/null

# Install finished
# Now download sdk
if ! [ -d $SDK_DIR ]; then
	cd ..
	wget $SDKURL

	# Decompress them
	echo "Decompressing SDKs..."
	unzip $SDKZIP 1>/dev/null

	cd $PROJECTDIR

fi

# Copy ncurses to project dir for linking
if ! [ -e $NCURSES_DIR ]; then
        echo "ncurses dynamic library cannot be found."
	exit
fi

cp -rv $NCURSES_DIR .

mkdir bin
mkdir obj