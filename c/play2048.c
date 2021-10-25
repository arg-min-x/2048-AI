#include <stdio.h>
#include <stdlib.h>
#include "lib2048.h"
#include <stdint.h>
#include <time.h>
#include <ncurses.h>
#include <math.h>


// Test the tree creation and deletion
int main(int argc, const char * argv[]) {


uint8_t game_board_orig[16] = { 0, 0, 0, 0,
                                0, 0, 0, 0,
                                0, 0, 0, 0,
                                0, 0, 0, 0};
uint8_t *game_board_fin;
game_board_fin = play_2048(game_board_orig);
}
