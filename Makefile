.PHONY: ncurses clean all

all:
	make ncurses
	make controller

controller:
	g++ src/*.cpp -I include -I local/include/ncursestw -I local/include -L local/lib -lncursestw -ldl -pthread -o bin/controller.o && cd bin && ./controller.o

clean:
	rm -rfv local/
	rm -rfv ncurses/
	rm -rfv bin/*.o
	
ncurses:
	chmod +x buildncurses.sh
	./buildncurses.sh
