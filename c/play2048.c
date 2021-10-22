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
// uint8_t game_board_orig[16] = { 2, 4, 2, 4,
//                                 4, 2, 4, 2,
//                                 2, 4, 2, 2,
//                                 4, 2, 4, 4};
// uint8_t game_board_orig[16] = {3,       9,     1,    2,
// 							   2,       8,     2,       3,
// 							   7,     6,      0,       5,
// 							   2,       5,      2,       2};
uint8_t *move_board;
uint8_t *tmp_board = malloc(16*sizeof(uint8_t));
int rand_num = 0;
int keep_moving = 1;
int num_zeros = 15;
int num_moves = 0;
move_board = add_random_number(game_board_orig);
initscr();
print_game_boardw(move_board);
printf("\n");

int ii = 0;
while (keep_moving>0){
		struct rand_node *root;
        root = malloc(sizeof(rand_node));
        root->game_board = &move_board[0];
        
        float move = -1;
        // Create tree
		if (num_zeros>6){
        	move = create_tree(root,3, 1);
		}else if (num_zeros<=6){
        	move = create_tree(root,4, 1);
		}
		// }else if (num_zeros==0){
        // 	move = create_tree(root,5, 1);
		// }
		free(root);
		char next_move = 'a';

        if (move == 0)
        {
        	next_move = 'l';
        }if (move == 1)
        {
        	next_move = 'r';
        }if (move == 2)
        {
        	next_move = 'u';
        }if (move == 3)
        {
        	next_move = 'd';
        }

		if (next_move=='u'){
			move_board = move_up(move_board, move_board);
			move_board = add_random_number(move_board);
			print_game_boardw(move_board);
			printf("\n");
		}else if(next_move=='d'){
			move_board = move_down(move_board, move_board);
			move_board = add_random_number(move_board);
			print_game_boardw(move_board);
			printf("\n");
		}
		else if(next_move=='l'){
			move_board = move_left(move_board, move_board);
			move_board = add_random_number(move_board);
			print_game_boardw(move_board);
			printf("\n");
		}
		else if(next_move=='r'){
			move_board = move_right(move_board, move_board);
			move_board = add_random_number(move_board);
			print_game_boardw(move_board);
			printf("\n");
		}

		// If all the moves result in an identical board state
		tmp_board = move_left(move_board, tmp_board);
		if (compare_board(tmp_board,move_board)){
			tmp_board = move_right(move_board, tmp_board);
			if (compare_board(tmp_board,move_board)){
				tmp_board = move_up(move_board, tmp_board);
				if (compare_board(tmp_board,move_board)){
					tmp_board = move_down(move_board, tmp_board);
					if (compare_board(tmp_board,move_board)){
						keep_moving = 0;
					}
				}
			}
		}
		num_zeros = count_zeros(move_board);
		num_moves++;
		ii++;
}
free(tmp_board);
printf("\nYou Lose\n");
endwin();
print_game_board(move_board);
}
