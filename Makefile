.PHONY: ncurses clean

controller:
	g++ src/*.cpp -I include -I local/include/ncursestw -I local/include -L local/lib -lncursestw -ldl -pthread -o bin/controller.o && cd bin && ./controller.o

clean:
	rm -rfv ncurses-6.2.tar.gz
	rm -rfv ncurses-6.2
	rm -rfv local/*
	rm -rfv bin/*.o
	
ncurses:
	chmod +x buildncurses.sh
	./buildncurses.sh

