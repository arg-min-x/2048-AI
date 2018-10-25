#include <stdio.h>
#include <stdlib.h>
#include "lib2048.h"
#include <stdint.h>
#include <time.h>

// Test the tree creation and deletion
int main(int argc, const char * argv[]) {


/*uint8_t game_board_orig[16] = { 1, 2, 3, 4,*/
/*                                0, 5, 4, 0,*/
/*                                3, 2, 3, 1,*/
/*                                11, 10, 8, 5};*/
uint8_t game_board_orig[16] = { 4,	0,	0,	0,
								4,	3,	0,	0,
								3,	4,	2,	1,
								1,	2,	1,	0};

struct rand_node *root;
char next_move;
root = malloc(sizeof(rand_node));
root->game_board = &game_board_orig[0];
create_tree_root(root,1);
next_move = eval_next_move_root(root);
printf("next_move %c\n",next_move);
//Destory Tree
destroy_tree(root);
free(root);
return 0;

}
