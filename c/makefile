play_2048: lib2048.o gameboard2048.o lib2048.so
	gcc -fopenmp -std=c99 play2048.c lib2048.o gameboard2048.o -lm -lncurses -O3 -o play_2048
	
lib2048.o: gameboard2048.o
	gcc -fopenmp -std=c99 -c lib2048.c -lm -lncurses -O3 -o lib2048.o

gameboard2048.o:
	gcc -fopenmp -std=c99 -c gameboard2048.c -lm -lncurses -O3 -o gameboard2048.o

lib2048.so:
	gcc -shared -fPIC -fopenmp -std=c99 lib2048.c gameboard2048.c -lm -lncurses -O3 -o lib2048.so

clean:
	rm -f *.o *.so play_2048
