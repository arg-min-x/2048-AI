#include <stdio.h>
#include <stdlib.h>
#include "lib2048.h"
#include <stdint.h>
#include <time.h>
#include <ncurses.h>
#include <math.h>

void print_game_boardw(uint8_t *game_board){
	clear();
	int n_board[16];
	for (int i = 0; i < 16; ++i)
	{
		n_board[i] = pow(2, game_board[i]);
		if (n_board[i]==1)
		{
			n_board[i] = 0;
		}
	}
    for (int ind = 0; ind < 4; ind++) {
        printw("%d\t%d\t%d\t%d\n",n_board[0+4*ind],n_board[1+4*ind],n_board[2+4*ind]
               ,n_board[3+4*ind]);
    }
    refresh();
}

// Test the tree creation and deletion
int main(int argc, const char * argv[]) {


uint8_t game_board_orig[16] = { 0, 0, 0, 0,
                                0, 0, 0, 0,
                                0, 0, 0, 0,
                                0, 0, 0, 0};
uint8_t *move_board,*tmp_board;
int rand_num = 0;
int keep_moving = 1;
int num_zeros = 15;
int num_moves = 0;
move_board = add_random_number(game_board_orig);
initscr();
print_game_boardw(move_board);
printf("\n");

while (keep_moving>0){
//printf("\e[1;1H\e[2J");
/*for (int ind =0; ind<100; ind++){*/
		struct rand_node *root;
        root = malloc(sizeof(rand_node));
        root->game_board = &move_board[0];
        
        // Create tree
		if (num_zeros>6){
        	create_tree_root(root,3);
		}else if (num_zeros<=6 && num_zeros > 0){
        	create_tree_root(root,4);
		}else if (num_zeros==0){
        	create_tree_root(root,5);
		}
        char next_move = 'a';
		char up, down, left, right;
		up = 'u';
		down = 'd';
		left = 'l';
		right = 'r';
        next_move = eval_next_move_root(root);
        // printf("next move = %c, number of moves = %d,\n",next_move,num_moves);
        
        //Destory Tree
        destroy_tree(root);
        free(root);

		if (next_move==up){
			move_board = move_up(move_board);
			move_board = add_random_number(move_board);
			print_game_boardw(move_board);
			printf("\n");
		}else if(next_move==down){
			move_board = move_down(move_board);
			move_board = add_random_number(move_board);
			print_game_boardw(move_board);
			printf("\n");
		}
		else if(next_move==left){
			move_board = move_left(move_board);
			move_board = add_random_number(move_board);
			print_game_boardw(move_board);
			printf("\n");
		}
		else if(next_move==right){
			move_board = move_right(move_board);
			move_board = add_random_number(move_board);
			print_game_boardw(move_board);
			printf("\n");
		}

		// If all the moves result in an identical board state
		tmp_board = move_left(move_board);
		if (compare_board(tmp_board,move_board)){
			tmp_board = move_right(move_board);
			if (compare_board(tmp_board,move_board)){
				tmp_board = move_up(move_board);
				if (compare_board(tmp_board,move_board)){
					tmp_board = move_down(move_board);
					if (compare_board(tmp_board,move_board)){
						keep_moving = 0;
					}
				}
			}
		}
		num_zeros = count_zeros(move_board);
		num_moves++;
		// system("@cls||clear");
}
printf("\nYou Lose\n");
}

/*srand(time(NULL));*/
/*for (int ind =0; ind<100; ind++){*/
/*	num_zeros = count_zeros(move_board);*/
/*	if (num_zeros>0){*/
/*		rand_num = rand() % 3 + 0;*/
/*		if (rand_num==0){*/
/*			move_board = move_up(move_board);*/
/*			move_board = add_random_number(move_board);*/
/*			print_game_board(move_board);*/
/*			printf("\n");*/
/*		}else if(rand_num==1){*/
/*			move_board = move_down(move_board);*/
/*			move_board = add_random_number(move_board);*/
/*			print_game_board(move_board);*/
/*			printf("\n");*/
/*		}*/
/*		else if(rand_num==2){*/
/*			move_board = move_left(move_board);*/
/*			move_board = add_random_number(move_board);*/
/*			print_game_board(move_board);*/
/*			printf("\n");*/
/*		}*/
/*		else if(rand_num==3){*/
/*			move_board = move_right(move_board);*/
/*			move_board = add_random_number(move_board);*/
/*			print_game_board(move_board);*/
/*			printf("\n");*/
/*		}*/
/*	}else{*/
/*		printf("\nyou lose\n");*/
/*		break;*/
/*	}*/
/*}*/
/*}*/
