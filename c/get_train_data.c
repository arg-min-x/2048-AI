#include <stdio.h>
#include <stdlib.h>
#include "lib2048.h"
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
int print_board(FILE *log_ptr,uint8_t *game_board, int num_moves){

    int max = 0;
	uint8_t max_ind = 0;
	float max_ind_cost = 0;
    for (int ind = 0; ind<16; ind++) {
        if (max<(int)game_board[ind]) {
            max = (int)game_board[ind];
			max_ind = ind;
        }
    }

	for(int ind = 0;ind<16;ind++){
		fprintf(log_ptr,"%d, ",game_board[ind]);
	}
	fprintf(log_ptr,"%d, ",num_moves);
	fprintf(log_ptr,"%d\n",max);
	return 0;
}
// Test the tree creation and deletion
int main(int argc, const char * argv[]) {

for (int ind = 0;ind<500;ind++){
	sleep(1);
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
	print_game_board(move_board);
	printf("\n");

	int seed = time(NULL);
	srand(seed+1);

	FILE *log_ptr;
	char file_path[60];
	sprintf(file_path,"/home/adam/2048-AI/data/%d.csv",ind);
	log_ptr = fopen(file_path,"w");

	while (keep_moving>0){
		
		// Choose a random move
		rand_num = rand() % 4 + 1;
		char next_move = 'a';
		char up, down, left, right;
		up = 'u';
		down = 'd';
		left = 'l';
		right = 'r';
		if (rand_num==1){
			next_move = up;
		}else if (rand_num==2){
			next_move = down;
		}else if (rand_num ==3){
			next_move = left;
		}else if (rand_num ==4){
			next_move = right;
		}

		tmp_board = move_up(move_board);
		if (next_move==up && !compare_board(tmp_board,move_board)){
			move_board = move_up(move_board);
			move_board = add_random_number(move_board);
			print_game_board(move_board);
			printf("\n");
			num_moves++;
			print_board(log_ptr,move_board,num_moves);
		}
		tmp_board = move_down(move_board);
		if(next_move==down && !compare_board(tmp_board,move_board)){
			move_board = move_down(move_board);
			move_board = add_random_number(move_board);
			print_game_board(move_board);
			printf("\n");
			num_moves++;
			print_board(log_ptr,move_board,num_moves);
		}
		tmp_board = move_left(move_board);
		if(next_move==left && !compare_board(tmp_board,move_board)){
			move_board = move_left(move_board);
			move_board = add_random_number(move_board);
			print_game_board(move_board);
			printf("\n");
			num_moves++;
			print_board(log_ptr,move_board,num_moves);
		}
		tmp_board = move_right(move_board);
		if(next_move==right && !compare_board(tmp_board,move_board)){
			move_board = move_right(move_board);
			move_board = add_random_number(move_board);
			print_game_board(move_board);
			printf("\n");
			num_moves++;
			print_board(log_ptr,move_board,num_moves);
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
	}
	printf("\nYou Lose\n");
	printf(file_path);
	fclose(log_ptr);
}

}

