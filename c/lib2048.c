//
//  lib2048.c
//  2048-AI
//
//  Created by Adam Rich on 4/11/16.
//  Copyright © 2016 Adam Rich. All rights reserved.
//

#include "lib2048.h"
#include "gameboard2048.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include <ncurses.h>

// ========================================================================================
//          Game Tree Structures and functions
// ========================================================================================
void set_move_node_null(struct rand_node *leaf){
    leaf->left = NULL;
    leaf->right = NULL;
    leaf->up = NULL;
    leaf->down = NULL;
    leaf->game_board = NULL;
}

void create_children_move_node(struct move_node *leaf){
    // Allocate the pointer array
    struct rand_node **moves_array = malloc(leaf->num_moves*sizeof(struct rand_node *));
    
    // Allocate the child move nodes
    for (int ind = 0; ind < leaf->num_moves; ind++) {
        moves_array[ind] = malloc(sizeof(rand_node));
        set_move_node_null(moves_array[ind]);
    }
    
    // Asign the move array pointer to the parent
    leaf->moves = moves_array;
}

// Create The tree
float create_tree(struct rand_node *root, int depth, int isroot){
    
    // If at the end of a tree
    // Alocate the child nodes
    struct move_node *left, *right, *up, *down;
    float left_cost = -100000;
    float right_cost = -100000;
    float up_cost = -100000;
    float down_cost = -100000;

	root->left = NULL;
	root->right = NULL;
	root->up = NULL;
	root->down = NULL;
    
	int last_ind = 0;
	uint8_t rand_val = 1;
	omp_set_num_threads(4);
	uint8_t *tmp_board = malloc(16*sizeof(uint8_t));
	tmp_board = move_left(root->game_board, tmp_board);
    // Call Constructor on the next level
	#pragma omp parallel
	#pragma omp sections nowait
	{
	#pragma omp section
	{
		if (!compare_board(tmp_board,root->game_board)){
			left_cost = 0;
			float tmp_cost = 0;
			left = malloc(sizeof(move_node));
			// Asign child nodes
			root->left = left;
			
			// Asign child game board
			uint8_t *left_board = malloc(16*sizeof(uint8_t));
			root->left->game_board = move_left(root->game_board, left_board);
			
			// Set the number of moves
			root->left->num_moves = 2*count_zeros(root->left->game_board);
			
			// Construct child nodes
			create_children_move_node(left);

			// create random board for right node
			for (int ind = 0; ind < root->left->num_moves/2; ind++) {
			    root->left->moves[ind]->game_board = create_random_board(root->left->game_board,&last_ind,rand_val);

			}
			last_ind = 0;
			rand_val = 2;

			for (int ind = root->left->num_moves/2; ind < root->left->num_moves; ind++) {
			    root->left->moves[ind]->game_board = create_random_board(root->left->game_board,&last_ind,rand_val);
			}

			// Asign random move boards
			if (depth > 1) {

			    // Call Constructor on the next level
				for (int ind = 0; ind < root->left->num_moves/2; ind++) {
					left_cost += create_tree(root->left->moves[ind], depth-1, 0);
				}					

				left_cost = 0.9*left_cost;
				for (int ind = root->left->num_moves/2; ind < root->left->num_moves; ind++) {
					left_cost += 0.1*create_tree(root->left->moves[ind], depth-1, 0);
				}
				left_cost = left_cost/root->left->num_moves;
				
				// Destroy the move at the end
				destroy_move_node(left);

			}else{
				// Evaluate cost if at the terminal node
				for (int ind = 0; ind < root->left->num_moves/2; ind++) {
					left_cost += eval_cost_new(root->left->moves[ind]->game_board);
				}
				left_cost = 0.9*left_cost;
				for (int ind = root->left->num_moves/2; ind < root->left->num_moves; ind++) {
					left_cost += 0.1*eval_cost_new(root->left->moves[ind]->game_board);
				}
				left_cost = left_cost/root->left->num_moves;
				
				// Destroy the move at the end
				destroy_move_node(left);
			} 
		}
	}

	#pragma omp section
	{
		tmp_board = move_right(root->game_board, tmp_board);
		if (!compare_board(tmp_board,root->game_board)){
			right_cost = 0;
			float tmp_cost = 0;
			right = malloc(sizeof(move_node));
			// Asign child nodes
			root->right = right;
			
			// Asign child move boards
			uint8_t *right_board = malloc(16*sizeof(uint8_t));
			root->right->game_board = move_right(root->game_board, right_board);
			
			// Set the number of moves
			root->right->num_moves = 2*count_zeros(root->right->game_board);
			
			// Construct child nodes
			create_children_move_node(right);

			// create random board for right node
			last_ind = 0;
			rand_val = 1;
			for (int ind = 0; ind < root->right->num_moves/2; ind++) {
			    root->right->moves[ind]->game_board = create_random_board(root->right->game_board,&last_ind,rand_val);
			}
			last_ind = 0;
			rand_val = 2;
			for (int ind = root->right->num_moves/2; ind < root->right->num_moves; ind++) {
			    root->right->moves[ind]->game_board = create_random_board(root->right->game_board,&last_ind,rand_val);
			}
			// Asign random move boards
			if (depth >1) {

			    // Call Constructor on the next level
				for (int ind = 0; ind < root->right->num_moves/2; ind++) {
					right_cost += create_tree(root->right->moves[ind], depth-1, 0);
				}					

				right_cost = 0.9*right_cost;
				for (int ind = root->right->num_moves/2; ind < root->right->num_moves; ind++) {
					right_cost += 0.1*create_tree(root->right->moves[ind], depth-1, 0);
				}
				right_cost = right_cost/root->right->num_moves;
				
				// Destroy the move at the end
				destroy_move_node(right);
			}else{
				// Evaluate cost if at the terminal node
				for (int ind = 0; ind < root->right->num_moves/2; ind++) {
					right_cost += eval_cost_new(root->right->moves[ind]->game_board);
				}
				right_cost = 0.9*right_cost;
				for (int ind = root->right->num_moves/2; ind < root->right->num_moves; ind++) {
					right_cost += 0.1*eval_cost_new(root->right->moves[ind]->game_board);
				}
				right_cost = right_cost/root->right->num_moves;

				// Destroy the move at the end
				destroy_move_node(right);
			}  
		}
	}

	#pragma omp section
	{
		tmp_board = move_up(root->game_board, tmp_board);
		if (!compare_board(tmp_board,root->game_board)){
			up_cost = 0;
			up = malloc(sizeof(move_node));
			// Asign child nodes
			root->up = up;
			
			// Asign child move boards
			uint8_t *up_board = malloc(16*sizeof(uint8_t));
			root->up->game_board = move_up(root->game_board, up_board);
			
			// Set the number of moves
			root->up->num_moves = 2*count_zeros(root->up->game_board);
			
			// Construct child nodes
			create_children_move_node(up);

			// create random board for up node
			last_ind = 0;
			rand_val = 1;
			for (int ind = 0; ind < root->up->num_moves/2; ind++) {
			    root->up->moves[ind]->game_board = create_random_board(root->up->game_board,&last_ind,rand_val);
			}
			last_ind = 0;
			rand_val = 2;
			for (int ind = root->up->num_moves/2; ind < root->up->num_moves; ind++) {
			    root->up->moves[ind]->game_board = create_random_board(root->up->game_board,&last_ind,rand_val);
			}

			// Asign random move boards
			if (depth >1) {
			    // Call Constructor on the next level
				for (int ind = 0; ind < root->up->num_moves/2; ind++) {
					up_cost += create_tree(root->up->moves[ind], depth-1, 0);
				}					

				up_cost = 0.9*up_cost;
				for (int ind = root->up->num_moves/2; ind < root->up->num_moves; ind++) {
					up_cost += 0.1*create_tree(root->up->moves[ind], depth-1, 0);
				}
				up_cost = up_cost/root->up->num_moves;

				// Destroy the move at the end
				destroy_move_node(up);
			}else{
				// Evaluate cost if at the terminal node
				for (int ind = 0; ind < root->up->num_moves/2; ind++) {
					up_cost += eval_cost_new(root->up->moves[ind]->game_board);
				}
				up_cost = 0.9*up_cost;
				for (int ind = root->up->num_moves/2; ind < root->up->num_moves; ind++) {
					up_cost += 0.1*eval_cost_new(root->up->moves[ind]->game_board);
				}
				up_cost = up_cost/root->up->num_moves;
				
				// Destroy the move at the end
				destroy_move_node(up);
			} 
		}
	}

	#pragma omp section
	{
		tmp_board = move_down(root->game_board, tmp_board);
		if (!compare_board(tmp_board,root->game_board)){
			down_cost = 0;
			down = malloc(sizeof(move_node));
			// Asign child nodes
			root->down = down;
			
			// Asign child move boards
			uint8_t *down_board = malloc(16*sizeof(uint8_t));
			root->down->game_board = move_down(root->game_board, down_board);
			
			// Set the number of moves
			root->down->num_moves = 2*count_zeros(root->down->game_board);
			
			// Construct child nodes
			create_children_move_node(down);

			// create random board for down node
			last_ind = 0;
			rand_val = 1;
			for (int ind = 0; ind < root->down->num_moves/2; ind++) {
			    root->down->moves[ind]->game_board = create_random_board(root->down->game_board,&last_ind,rand_val);
			}
			last_ind = 0;
			rand_val = 2;
			for (int ind = root->down->num_moves/2; ind < root->down->num_moves; ind++) {
			    root->down->moves[ind]->game_board = create_random_board(root->down->game_board,&last_ind,rand_val);
			}

			// Asign random move boards
			if (depth >1) {
			    // Call Constructor on the next level
				for (int ind = 0; ind < root->down->num_moves/2; ind++) {
					down_cost += create_tree(root->down->moves[ind], depth-1, 0);
				}					

				down_cost = 0.9*down_cost;
				for (int ind = root->down->num_moves/2; ind < root->down->num_moves; ind++) {
					down_cost += 0.1*create_tree(root->down->moves[ind], depth-1, 0);
				}
				down_cost = down_cost/root->down->num_moves;

				// Destroy the move at the end
				destroy_move_node(down);
			}else{
				// Evaluate cost if at the terminal node
				for (int ind = 0; ind < root->down->num_moves/2; ind++) {
					down_cost += eval_cost_new(root->down->moves[ind]->game_board);
				}
				down_cost = 0.9*down_cost;
				for (int ind = root->down->num_moves/2; ind < root->down->num_moves; ind++) {
					down_cost += 0.1*eval_cost_new(root->down->moves[ind]->game_board);
				}
				down_cost = down_cost/root->down->num_moves;

				// Destroy the move at the end
				destroy_move_node(down);
			}
		}
	}
}

	/* code */

	float max_cost = left_cost;
	char mv;

	if (!isroot){
		mv = 'l';
		if (right_cost > max_cost){
			max_cost = right_cost;
			mv = 'r';
		}if (up_cost > max_cost){
			max_cost = up_cost;
			mv = 'u';
		}if (down_cost > max_cost){
			max_cost = down_cost;
			mv = 'd';
		}

	// free the root node
	}else{
		// printf("left cost %f\n", left_cost);
		// printf("right cost %f\n", right_cost);
		// printf("up cost %f\n", up_cost);
		// printf("down cost %f\n", down_cost);
		float mcost = 0;
		if (right_cost > max_cost){
			max_cost = right_cost;
			mcost = 1;
		}if (up_cost > max_cost){
			max_cost = up_cost;
			mcost = 2;
		}if (down_cost > max_cost){
			max_cost = down_cost;
			mcost = 3;
		}
		max_cost = mcost;
	}
	free(tmp_board);
	return max_cost;
}

void destroy_move_node(struct move_node *move){
        
        // Destroy the game boards for random nodes
        if (move->moves[0]->game_board !=0) {
            for (int ind = 0; ind < move->num_moves; ind++) {
                free(move->moves[ind]->game_board);
				move->moves[ind]->game_board = NULL;
            }
        }
        
        // Destroy random nodes
        for (int ind=0; ind<move->num_moves; ind++) {
            free(move->moves[ind]);
            move->moves[ind] = NULL;
        }
		free(move->moves);
        move->moves = NULL;
        
		free(move->game_board);
        move->game_board = NULL;
		
        // destroy move nodes and root
        free(move);
        move = NULL;
}

// Destroy the tree
void destroy_tree(struct rand_node *root){
    
    if (root->left != 0) {
        
        // Keep going down the tree if not empty
        for (int ind = 0; ind<root->left->num_moves; ind++) {
            destroy_tree(root->left->moves[ind]);
        }
        
		destroy_move_node(root->left);
        // // Destroy the game boards for random nodes
        // if (root->left->moves[0]->game_board !=0) {
        //     for (int ind = 0; ind < root->left->num_moves; ind++) {
        //         free(root->left->moves[ind]->game_board);
        //     }
        // }
        
        // // Destroy random nodes
        // for (int ind=0; ind<root->left->num_moves; ind++) {
        //     free(root->left->moves[ind]);
        // }
        // free(root->left->game_board);
        // free(root->left->moves);
        
        // // destroy move nodes and root
        // free(root->left);
    }
    if (root->right != 0) {
        
        // Keep going down the tree if not empty
        for (int ind = 0; ind<root->right->num_moves; ind++) {
            destroy_tree(root->right->moves[ind]);
        }
        destroy_move_node(root->right);
        // if (root->right->moves[0]->game_board !=0) {
        //     for (int ind = 0; ind < root->right->num_moves; ind++) {
        //         free(root->right->moves[ind]->game_board);
        //     }
        // }
        
        // // Destroy random nodes
        // for (int ind=0; ind<root->right->num_moves; ind++) {
        //     free(root->right->moves[ind]);
        // }

        // free(root->right->moves);
        // free(root->right->game_board);
        
        // // destroy move nodes and root
        // free(root->right);
    }
    if (root->up != 0) {
        
        // Keep going down the tree if empty
        for (int ind = 0; ind<root->up->num_moves; ind++) {
            destroy_tree(root->up->moves[ind]);
        }

        destroy_move_node(root->up);

        // if (root->up->moves[0]->game_board !=0) {
        //     for (int ind = 0; ind < root->up->num_moves; ind++) {
        //         free(root->up->moves[ind]->game_board);
        //     }
        // }
        
        // // Destroy random nodes
        // for (int ind=0; ind<root->up->num_moves; ind++) {
        //     free(root->up->moves[ind]);
        // }

        // free(root->up->moves);
        // free(root->up->game_board);
        
        // // destroy move nodes and root
        // free(root->up);
    }
    if (root->down != 0) {
        
        // Keep going down the tree if empty
        for (int ind = 0; ind<root->down->num_moves; ind++) {
            destroy_tree(root->down->moves[ind]);
        }
        destroy_move_node(root->down);
        
        // if (root->down->moves[0]->game_board !=0) {
        //     for (int ind = 0; ind < root->down->num_moves; ind++) {
        //         free(root->down->moves[ind]->game_board);
        //     }
        // }
        
        // // Destroy random nodes
        // for (int ind=0; ind<root->down->num_moves; ind++) {
        //     free(root->down->moves[ind]);
        // }

        // free(root->down->moves);
        // free(root->down->game_board);
        
        // // destroy move nodes and root
        // free(root->down);
    }
	// free(NULL);
	// printf("done\n");
}

// ========================================================================================
//          Game Board Manipulation
// ========================================================================================

// ========================================================================================
// Evalutate Cost function for a leaf
float eval_cost_new(uint8_t *game_board){
	float cost = 0;
	float cb_ideal_cost = 0;
	uint8_t ideal_board[16] = {0};
    cost = ((double)count_zeros(game_board))/16;

	// Find the maximum of the board
    int max = 0;
	uint8_t max_ind = 0;
	float max_ind_cost = 0;
    for (int ind = 0; ind<16; ind++) {
        if (max<(int)game_board[ind]) {
            max = (int)game_board[ind];
			max_ind = ind;
        }
    }

	// Calculate the ideal game board
	calc_ideal_board(game_board, ideal_board);

	float cb_dist = 0;
	float norm = 0;
	float tmp = 0;
	for (int val = max; val > -1; val--)
	{
		tmp = (float)val*(float)val;
		cb_dist += tmp*get_cb_between_ideal(game_board, ideal_board, val);
		norm += tmp;
	}
	cb_dist = cb_dist/norm;

	// Put the maximum in the bottom right corner
	if (max_ind==15){
		max_ind_cost = 1;
	}

   	// cost = cost;
	cost = 2*cost - 8*cb_dist + max_ind_cost;
	// cost = cost - 5*cb_dist;
	// cost = -cb_dist;

    return cost;
}

float calc_cb_distances(uint8_t *game_board, int val){
    uint8_t indx[15];
    uint8_t indy[15];
    uint8_t jj = 0;

    // Get the x y locations of val
    for (uint8_t i = 0; i < 16; ++i)
    {
    	if ((int)game_board[i] == val)
    	{
    		indx[jj] = i % 4;
    		indy[jj] = i/4;
    		// printf("max %d max indx %d max in y%d\n", val, indx[jj], indy[jj]);
    		jj++;
    	}
    }

    float dist = 10000000;
    float dist_new = 0;
    if (jj > 1)
    {
    	for (int ex_ind = 0; ex_ind < jj; ++ex_ind)
    	{
	    	for (int ind = 0; ind < jj; ++ind)
	    	{
	    		if (ind != ex_ind)
	    		{
		    		dist_new += abs(indx[ex_ind] - indx[ind]) + abs(indy[ex_ind] - indy[ind]);
	    			
	    		}
	    	}
	    	if (dist_new < dist)
	    	{
	    		dist = dist_new;
	    	}
		    // printf("%f\n", dist);
		    dist_new = 0;
    	}
		dist = dist/6;

    }else{
    	dist = 0;
    }
    return dist;
}

float calc_cb_distances_next(uint8_t *game_board, int val, int val2){
    uint8_t indx[16], indx2[16];
    uint8_t indy[16], indy2[16];
    uint8_t jj, kk;
    jj = 0;
    kk = 0;

    // Get the x y locations of val
    for (uint8_t i = 0; i < 16; ++i)
    {
    	if ((int)game_board[i] == val)
    	{
    		indx[jj] = i % 4;
    		indy[jj] = i/4;
    		// printf("max %d max indx %d max in y%d\n", val, indx[jj], indy[jj]);
    		jj++;
    	}
    }

    // Get the x y locations of val
    for (uint8_t i = 0; i < 16; ++i)
    {
    	if ((int)game_board[i] == val2)
    	{
    		indx2[kk] = i % 4;
    		indy2[kk] = i/4;
    		// printf("max %d max indx %d max in y%d\n", val, indx[jj], indy[jj]);
    		kk++;
    	}
    }
    
    float dist = 0;
    float dist_new = 0;
    if (jj > 0 && kk > 0)
    {
    	for (int ex_ind = 0; ex_ind < kk; ++ex_ind)
    	{
	    	for (int ind = 0; ind < jj; ++ind)
	    	{
		    	dist_new += abs(indx2[ex_ind] - indx[ind]) + abs(indy2[ex_ind] - indy[ind]);
	    	}
	    	dist += dist_new;
		    dist_new = 0;
    	}

    }else{
    	dist = 0;
    }
    return dist;
}

// ========================================================================================
// Evalutate Cost function for a leaf
float get_cb_between_ideal(uint8_t *game_board, uint8_t *ideal_board, int val){
    uint8_t ideal_indx[15];
    uint8_t ideal_indy[15];
    uint8_t game_indx[15];
    uint8_t game_indy[15];
    uint8_t jj = 0;
    uint8_t kk = 0;

    // printf("\nval = %d\n", val);
    // printf("Game Board locs\n");
    // Get the x y locations of val in game board
    for (int i = 0; i < 16; ++i)
    {
        if ((int)game_board[i] == val)
        {
            game_indx[jj] = i % 4;
            game_indy[jj] = i/4;
            // printf("%d, %d\n", game_indx[jj], game_indy[jj]);
            jj++;

        }
    }

    // Get the x y locations of val in ideal board
    // printf("Ideal Board locs\n");
    for (int i = 0; i < 16; ++i)
    {
        if ((int)ideal_board[i] == val)
        {
            ideal_indx[kk] = i % 4;
            ideal_indy[kk] = i/4;
            // printf("%d, %d\n", ideal_indx[kk], ideal_indy[kk]);
            kk++;

        }
    }

    // Get cb distance to nearest vale in ideal board
    int cb_dist = 100;
    int cb_dist_tmp = 100;
    float ret_cb = 0;
    float *cb_arr;
    cb_arr = malloc(jj*sizeof(float));
    for (int game_ind = 0; game_ind < jj; game_ind++)
    {
        int cb_dist = 100;
        int cb_dist_tmp = 100;
        for (int ideal_ind = 0; ideal_ind < kk; ideal_ind++)
        {

            cb_dist_tmp = abs(game_indx[game_ind] - ideal_indx[ideal_ind]) + abs(game_indy[game_ind] - ideal_indy[ideal_ind]);
            // printf("dist x%d, y%d, x%d, y%d dist%d\n", game_indx[game_ind], game_indy[game_ind], ideal_indx[ideal_ind], ideal_indy[ideal_ind], cb_dist_tmp);
            if (cb_dist_tmp < cb_dist)
            {
                cb_dist = cb_dist_tmp;
            }
        }
        cb_arr[game_ind] = (float)cb_dist/6;
        
    }

    for (int i = 0; i < jj; i++)
    {
        // printf("cb arr %f\n", cb_arr[i]);
        ret_cb += cb_arr[i];
    }
    // printf("return cb %f\n", ret_cb);
    free(cb_arr);
    return(ret_cb);
}


int comp (const void * elem1, const void * elem2) 
{
    int f = *((uint8_t*)elem1);
    int s = *((uint8_t*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}


void calc_ideal_board(uint8_t * game_board, uint8_t *ideal_board){
    uint8_t tmp_row[4] = {0};

    // copy game board into ideal boar
    for (int i = 0; i < 16; i++)
    {
        ideal_board[i] = game_board[i];
    }


    // sort the ideal board
    qsort(ideal_board, 16, sizeof(uint8_t), comp);

    // reverse first row
    for (int i = 3; i >-1; i--)
    {
        tmp_row[3-i] = ideal_board[i];
    }
    for (int i = 0; i < 4; i++)
    {
        ideal_board[i] = tmp_row[i];
    }

    // Reverse 3rd row
    for (int i = 11; i >7; i--)
    {
        tmp_row[11-i] = ideal_board[i];
    }

    for (int i = 8; i < 12; i++)
    {
        ideal_board[i] = tmp_row[i-8];
    }
}



// Test the tree creation and deletion
uint8_t *play_2048(uint8_t * game_board) {


uint8_t *move_board;
uint8_t *tmp_board = malloc(16*sizeof(uint8_t));
int keep_moving = 1;
int num_zeros = 15;
int num_moves = 0;
move_board = add_random_number(game_board);

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

        // move = create_tree(root,1, 1);
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
