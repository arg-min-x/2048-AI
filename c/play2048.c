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
		int a = 6;
		int b = 1;
		if (num_zeros>a){
        	move = create_tree(root,3, 1);
		}else if (num_zeros<=a && num_zeros>b){
        	move = create_tree(root,4, 1);
		}else{
        	move = create_tree(root,5, 1);
		}
		free(root);
		char next_move = 'a';

        if (move == 0){
        	next_move = 'l';
        }if (move == 1){
        	next_move = 'r';
        }if (move == 2){
        	next_move = 'u';
        }if (move == 3){
        	next_move = 'd';
        }

		if (next_move=='u'){
			move_board = move_up(move_board, move_board);
			move_board = add_random_number(move_board);
			print_game_boardw(move_board);
		}else if(next_move=='d'){
			move_board = move_down(move_board, move_board);
			move_board = add_random_number(move_board);
			print_game_boardw(move_board);
		}
		else if(next_move=='l'){
			move_board = move_left(move_board, move_board);
			move_board = add_random_number(move_board);
			print_game_boardw(move_board);
		}
		else if(next_move=='r'){
			move_board = move_right(move_board, move_board);
			move_board = add_random_number(move_board);
			print_game_boardw(move_board);
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
endwin();
}
