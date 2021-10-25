play_2048: play2048.c lib2048.o
	gcc -fopenmp -std=c99 play2048.c lib2048.c -lm -lncurses -O3 -o play_2048
	
lib2048: lib2048.c
	gcc -fopenmp -std=c99 lib2048.c -lm -lncurses -O3

lib2048_shared: lib2048.c
	gcc -shared -fPIC -fopenmp -std=c99 lib2048.c -lm -lncurses -O3 -o lib2048.so