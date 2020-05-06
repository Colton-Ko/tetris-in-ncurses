controller:
	rm -rfv *.o
	g++ *.cpp -o controller.o -lncurses && ./controller.o
	stty sane
	clear

blocks:
	rm -rfv *.o
	g++ blocks.cpp -o blocks.o -lncurses && ./blocks.o
