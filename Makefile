# 	DATE
# 	2020-05-08

# 	FILENAME
# 	Makefile

# 	VIEW
#	Tabsize:        8
#	Indentation:    TAB

# Predefined variables
CXXFLAGS=-I local/include/ncursestw -I local/include -I include -pthread -std=c++11 -pedantic-errors -Ofast
LDFLAGS=-L local/lib -std=c++11 -pedantic-errors -lncursestw -ldl -pthread
OBJECTS=obj/blocks.o obj/debug.o obj/game.o obj/controller.o obj/bookkeeper.o obj/window.o 
HEADERS=include/tetris/blocks.h include/tetris/game.h include/tetris/constants.h include/tetris/types.h include/tetris/controller.h include/tetris/debug.h include/tetris/options.h include/tetris/bookkeeper.h include/tetris/window.h

# Build targets
all: ncurses $(OBJECTS) $(HEADERS)
	g++ $(OBJECTS) -o bin/tetris $(LDFLAGS)
	@echo "Compiled! Check it out in bin/tetris!"

obj/game.o: src/game.cpp $(HEADERS)
	g++ $(CXXFLAGS) $< -c -o $@ 

obj/controller.o: src/controller.cpp $(HEADERS)
	g++ $(CXXFLAGS) $< -c -o $@ 

obj/blocks.o: src/blocks.cpp $(HEADERS)
	g++ $(CXXFLAGS) $< -c -o $@ 

obj/debug.o: src/debug.cpp $(HEADERS)
	g++ $(CXXFLAGS) $< -c -o $@ 

obj/bookkeeper.o: src/bookkeeper.cpp $(HEADERS)
	g++ $(CXXFLAGS) $< -c -o $@ 

obj/window.o : src/window.cpp $(HEADERS)
	g++ $(CXXFLAGS) $< -c -o $@ 

# Clean the binaries
clean:
	rm -rfv bin/*
	rm -rfv obj/*

# Clean all including ncurses
clean_all:
	rm -rfv ncurses
	rm -rfv local
	rm -rfv bin/*
	rm -rfv obj/*
	
# Compile ncurses
ncurses:
	chmod +x buildncurses.sh
	./buildncurses.sh

.PHONY: ncurses clean all
