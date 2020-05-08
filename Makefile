CXXFLAGS=-I local/include/ncursestw -I local/include -I include -L local/lib -lncursestw -ldl -pthread -pedantic-errors -std=c++11 -Ofast
LDFLAGS=-I local/include/ncursestw -I local/include -I include -L local/lib -pedantic-errors -std=c++11 -lncursestw -ldl -pthread
OBJECTS=obj/blocks.o obj/debug.o obj/game.o obj/controller.o
HEADERS=include/tetris/blocks.h include/tetris/game.h include/tetris/constants.h include/tetris/types.h include/tetris/controller.h include/tetris/debug.h include/tetris/options.h

all: ncurses obj/blocks.o obj/debug.o obj/game.o obj/controller.o $(HEADERS)
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


clean:
	rm -rfv bin/*
	rm -rfv obj/*

clean_all:
	rm -rfv ncurses
	rm -rfv local
	rm -rfv bin/*
	rm -rfv obj/*
	
ncurses:
	chmod +x buildncurses.sh
	./buildncurses.sh


.PHONY: ncurses clean all
