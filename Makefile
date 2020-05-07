CXXFLAGS=-I include -I local/include/ncursestw -I local/include -L local/lib -lncursestw -ldl -pthread
CXX=g++

.PHONY: ncurses clean all

all:
	make ncurses
	make controller

controller:
	g++ src/*.cpp -I include -I local/include/ncursestw -I local/include -L local/lib -lncursestw -ldl -pthread -o bin/controller.o && cd bin && ./controller.o

clean:
	rm -rfv local/
	rm -rfv ncurses/
	rm -rfv .ncurses-ready
	rm -rfv bin/*.o

test:
	g++ test/unit-blockToBlockMatrix.cpp -I include -I local/include/ncursestw -I local/include -L local/lib -lncursestw -o test/a.out 

ncurses:
	chmod +x buildncurses.sh
	./buildncurses.sh
