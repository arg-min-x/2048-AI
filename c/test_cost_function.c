#include <stdio.h>
#include <stdlib.h>
#include "lib2048.h"
#include <stdint.h>
#include <time.h>

// Test the tree creation and deletion
int main(int argc, const char * argv[]) {


uint8_t game_board_orig[16] = { 1, 2, 3, 4,
                                0, 5, 4, 3,
                                3, 2, 3, 1,
                                11, 10, 8, 5};

eval_cost(game_board_orig);
return 0;

}
