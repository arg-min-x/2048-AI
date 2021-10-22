//
//  lib2048.c
//  2048-AI
//
//  Created by Adam Rich on 4/11/16.
//  Copyright © 2016 Adam Rich. All rights reserved.
//

#include "lib2048.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>
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

void print_game_board(uint8_t *game_board){
    for (int ind = 0; ind < 4; ind++) {
        printf("%d\t%d\t%d\t%d\n",game_board[0+4*ind],game_board[1+4*ind],game_board[2+4*ind]
               ,game_board[3+4*ind]);
    }
}

// ========================================================================================
// Move Right
uint8_t *move_right(uint8_t *game_board, uint8_t *move_board){
    
    // Allocate the new board and copy the old board into it
    for (int ind = 0; ind<16; ind++) {
        move_board[ind] = game_board[ind];
    }
    
    for (int repeat = 0; repeat < 3; repeat++) {
        // Shift out zeros
        for (int offset = 0; offset<13; offset +=4) {
            for (int ind = 3; ind >0; ind--) {
                
                // If The current value is zero
                if (move_board[ind+offset] == 0) {
                    // shift value
                    move_board[ind+offset] = move_board[ind+offset-1];
                    move_board[ind-1+offset] = 0;
                }
            }
        }
    }
    
    // Combine like blocks and shift
    for (int offset = 0; offset<13; offset +=4) {
        for (int ind = 3; ind >0; ind--) {
            
            // if the two adjecent values are equal
            if (move_board[ind+offset] == move_board[ind-1+offset] && move_board[ind+offset]>0) {
                move_board[ind+offset] = move_board[ind+offset] + 1;
                move_board[ind-1+offset] = 0;
            }
        }
    }

    // Shift out zeros
    for (int offset = 0; offset<13; offset +=4) {
        for (int ind = 3; ind >0; ind--) {
            
            // If The current value is zero
            if (move_board[ind+offset] == 0) {
                // shift value
                move_board[ind+offset] = move_board[ind+offset-1];
                move_board[ind-1+offset] = 0;
            }
        }
    }
    
    return move_board;
}

// ========================================================================================
// Move the Game Board left
uint8_t *move_left(uint8_t *game_board, uint8_t *move_board){
    
    // uint8_t *move_board = malloc(16*sizeof(uint8_t));
    for (int ind = 0; ind<16; ind++) {
        move_board[ind] = game_board[ind];
    }
    
    // Shift out zeros
    for (int repeat = 0; repeat<3; repeat++) {
        for (int offset = 0; offset<13; offset +=4) {
            for (int ind = 0; ind < 3; ind++) {
                
                // If The current value is zero
                if (move_board[ind+offset] == 0) {
                    // shift value
                    move_board[ind+offset] = move_board[ind+offset+1];
                    move_board[ind+1+offset] = 0;
                }
            }
        }
    }
    
    // Combine like blocks and shift
    for (int offset = 0; offset<13; offset +=4) {
        for (int ind = 0; ind < 3; ind++) {
            
            // if the two adjecent values are equal
            if (move_board[ind+offset] == move_board[ind+1+offset] && move_board[ind+offset]>0) {
                move_board[ind+offset] +=1;
                move_board[ind+1+offset] = 0;
            }
        }
    }
    
    // Shift out zeros
    for (int offset = 0; offset<13; offset +=4) {
        for (int ind = 0; ind < 3; ind++) {
            
            // If The current value is zero
            if (move_board[ind+offset] == 0) {
                
                // shift value
                move_board[ind+offset] = move_board[ind+offset+1];
                move_board[ind+1+offset] = 0;
            }
        }
    }
    return move_board;
}

// ========================================================================================
// Move the Game Board up
uint8_t *move_up(uint8_t *game_board, uint8_t *move_board){
    
    for (int ind = 0; ind<16; ind++) {
        move_board[ind] = game_board[ind];
    }
    
    // Shift out zeros
    for (int repeat = 0; repeat<4; repeat++) {
        for (int offset = 0; offset<4; offset ++) {
            for (int ind = 0; ind < 12; ind+=4) {
                
                // If The current value is zero
                if (move_board[ind+offset] == 0) {
                    // shift value
                    move_board[ind+offset] = move_board[ind+offset+4];
                    move_board[ind+4+offset] = 0;
                }
            }
        }
    }
    
    // Combine like blocks and shift
    for (int offset = 0; offset<4; offset ++) {
        for (int ind = 0; ind < 12; ind+=4) {
            
            // if the two adjecent values are equal
            if (move_board[ind+offset] == move_board[ind+4+offset] && move_board[ind+offset]>0) {
                move_board[ind+offset] +=1;
                move_board[ind+4+offset] = 0;
            }
        }
    }

    // Shift out zeros
    for (int repeat = 0; repeat<2; repeat++) {
        for (int offset = 0; offset<4; offset ++) {
            for (int ind = 0; ind < 12; ind+=4) {
                
                // If The current value is zero
                if (move_board[ind+offset] == 0) {
                    // shift value
                    move_board[ind+offset] = move_board[ind+offset+4];
                    move_board[ind+4+offset] = 0;
                }
            }
        }
    }
    return move_board;
}

// ========================================================================================
// Move the Game Board down
uint8_t *move_down(uint8_t *game_board, uint8_t *move_board){
    
    for (int ind = 0; ind<16; ind++) {
        move_board[ind] = game_board[ind];
    }
    
    // Shift out zeros
    for (int repeat = 0; repeat<4; repeat++) {
        for (int offset = 0; offset<4; offset ++) {
            for (int ind = 12; ind > 3 ; ind-=4) {
                
                // If The current value is zero
                if (move_board[ind+offset] == 0) {
                    // shift value
                    move_board[ind+offset] = move_board[ind+offset-4];
                    move_board[ind-4+offset] = 0;
                }
            }
        }
    }
    
    // Combine like blocks and shift
    for (int offset = 0; offset<4; offset ++) {
        for (int ind = 12; ind > 3 ; ind-=4) {
            
            // if the two adjecent values are equal
            if (move_board[ind+offset] == move_board[ind-4+offset] && move_board[ind+offset]>0) {
                move_board[ind+offset] +=1;
                move_board[ind-4+offset] = 0;
            }
        }
    }
    
    // Shift out zeros
    for (int repeat = 0; repeat<2; repeat++) {
        for (int offset = 0; offset<4; offset ++) {
            for (int ind = 12; ind > 3 ; ind-=4) {
                
                // If The current value is zero
                if (move_board[ind+offset] == 0) {
                    // shift value
                    move_board[ind+offset] = move_board[ind+offset-4];
                    move_board[ind-4+offset] = 0;
                }
            }
        }
    }
    return move_board;
}

// ========================================================================================
// Count the number of zeros on the board
uint8_t count_zeros(uint8_t *game_board){
    uint8_t zero_count = 0;
    
    for (int ind =0; ind<16; ind++) {
        if (game_board[ind]==0) {
            zero_count++;
        }
    }
    return zero_count;
}

// Compare to boards return 1 if they are the same
uint8_t compare_board(uint8_t *board1, uint8_t *board2){
	int identical = 1;
	for (int ind = 0; ind<16; ind++){
		if (board1[ind] != board2[ind]){
			identical = 0;
		}
	}
return identical;
}

// ========================================================================================
// Create a board with two or four added at a zero locations
uint8_t *create_random_board(uint8_t *game_board, int *last_zero_ind, uint8_t rand_value){
    int ind;
    uint8_t *rand_board = malloc(16*sizeof(uint8_t));
    for (int ind = 0; ind<16; ind++) {
        rand_board[ind] = game_board[ind];
    }
    
    for (ind = *last_zero_ind; ind<16; ind++) {
        if (rand_board[ind]==0) {
            rand_board[ind] = rand_value;
            break;
        }
    }
    *last_zero_ind = ind+1;
    return rand_board;
}

// ========================================================================================
// adds a 2 or 4 to the board in a random location
uint8_t *add_random_number(uint8_t *game_board){
    int num_zeros, ind_z, rand_ind, rand_num;
	ind_z = 0;
	num_zeros = count_zeros(game_board);
	int *zero_inds;
	zero_inds = malloc(sizeof(int)*num_zeros);

	for (int ind =0; ind<16; ind++){
		if (game_board[ind] ==0) {
			zero_inds[ind_z] = ind;
			ind_z++;
		}	
	}
	
	srand(time(NULL));
	rand_ind = rand() % num_zeros + 0;
	rand_num = rand() % 100 + 1;
	if (rand_num <11){
		rand_num = 2;
	}else{
		rand_num = 1;
	}

	game_board[zero_inds[rand_ind]] = rand_num;
	//printf("\n rand ind %d  zero ind %d rand num %d\n",rand_ind, zero_inds[rand_ind], rand_num);

	
	//for (int ind = 0; ind<num_zeros; ind++){
	//	printf("%d ",zero_inds[ind]);	
	//}
	free(zero_inds);
    return game_board;
}

// ========================================================================================
// Evalutate Cost function for a leaf
float eval_cost(uint8_t *game_board){
	float cost = 0;
	// float cost2 = 0;
	int next = 0;
	int keep_looking = 1;
	int ind_on_board = 0;
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

    for (int ind = max; ind > 0; --ind)
    {
    	// Calculate cb distance for same values
    	cost += ind * calc_cb_distances(game_board, ind);
    	// printf("cost = %f\n", cost);
    	// Check if the curren int is on the game board
    	ind_on_board = 0;
    	for (int bind = 0; bind < 16; ++bind)
    	{
    		if (game_board[bind] == ind)
    		{
    			ind_on_board = 1;
    			// printf("ind on board %d\n", ind);
    			break;
    		}
    	}

    	// If the ind is on game board and ind > 1
    	if ( ind > 1 && ind_on_board){
    		keep_looking = 1;
    		next = ind-1;
    		
    		while (keep_looking){
    			for (int bind = 0; bind < 16; ++bind)
    			{
    				if (game_board[bind] == next)
    				{
    					keep_looking = 0;
    					break;
    				}
    			}
    			if (keep_looking)
    			{
    				next--;
    			}
    			if (next == 0)
    			{
    				keep_looking = 0;
    			}
    		}

    		// printf("ind = %d, next=%d\n", ind, next);
    		cost += 0.3*next*calc_cb_distances_next(game_board, ind, next);
    		// printf("cost2 = %f\n", cost2);
    	}
    }
    
    uint8_t num_zeros = 0;
    num_zeros = count_zeros(game_board);

	// Put the maximum in the bottom right corner
	if (max_ind==15 || max_ind == 3 || max_ind == 0 || max_ind == 12){
		max_ind_cost = 1000.0;
	}else{
		max_ind_cost = 0;
	}

    return -cost;
    // return -cost;
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
float eval_cost_new(uint8_t *game_board){
	float cost = 0;
    cost = ((double)count_zeros(game_board))/15;

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
	// Put the maximum in the bottom right corner
	if (max_ind==15){
		max_ind_cost = 1;
	}else if(max_ind==11||max_ind==14){
		max_ind_cost = 0.3;
	}else if(max_ind==10||max_ind==13||max_ind==7){
		max_ind_cost = 0.1;
	}else if(max_ind==9||max_ind==6||max_ind==12||max_ind==3){
		max_ind_cost = 0.1;
	}else if(max_ind==9||max_ind==6||max_ind==12||max_ind==3){
		max_ind_cost = 0.0;
	}else if(max_ind==5||max_ind==8||max_ind==2){
		max_ind_cost = 0.0;
	}else if(max_ind==4||max_ind==1){
		max_ind_cost = 0.0;
	}else if(max_ind==0){
		max_ind_cost = 0.0;
	}

	// // apply a smoothness constraint
 //    int grad[18];
	// int grad_sum=0;
	// int ind_2 = 0;
	// grad[0] = game_board[0];
	// grad[17] = game_board[12];
	
	// for (int ind = 0; ind<4; ind++){
	// 	grad[ind+1] = game_board[ind];
	// }
	// for (int ind = 0; ind<4; ind++){
	// 	grad[ind+1] = game_board[ind];
	// }
	// ind_2 = 7;
	// for (int ind = 4; ind<8; ind++){
	// 	grad[ind+1] = game_board[ind_2];
	// 	ind_2--;
	// }
	// for (int ind = 8; ind<12; ind++){
	// 	grad[ind+1] = game_board[ind];
	// }
	// ind_2 = 15;
	// for (int ind = 12; ind<16; ind++){
	// 	grad[ind+1] = game_board[ind_2];
	// 	ind_2--;
	// }

	// for (int ind = 0; ind<17; ind++){
	// 	if (ind >11){
	// 		grad[ind] = 2.0*abs(grad[ind]-grad[ind+1]);
	// 	}else{
	// 		grad[ind] = abs(grad[ind]-grad[ind+1]);
	// 	}
	// }
	// grad[17] = 0;

	// for (int ind = 0; ind<17; ind++){
	// 	grad_sum =+ grad[ind];
	// }

	//// apply a smoothness constraint*/
    int grad[16];
	int grad_sum=0;
	int ind_2 = 0;
	
	for (int ind = 0; ind<4; ind++){
		grad[ind] = game_board[ind]*max;
		if (max>0){
			max--;
		}
	}
	ind_2 = 7;
	for (int ind = 4; ind<8; ind++){
		grad[ind] = game_board[ind_2]*max;
		ind_2--;
		if (max>0){
			max--;
		}
	}
	for (int ind = 8; ind<12; ind++){
		grad[ind] = game_board[ind]*max;
		if (max>0){
			max--;
		}
	}
	ind_2 = 15;

	for (int ind = 12; ind<16; ind++){
		grad[ind] = game_board[ind_2]*max;
		if (max>0){
			max--;
		}
		ind_2--;
	}

	for (int ind = 0; ind<17; ind++){
		grad[ind] = abs(grad[ind]);
	}
	grad[17] = 0;

	for (int ind = 0; ind<17; ind++){
		grad_sum =+ grad[ind]*grad[ind]*grad[ind];
	}


   	cost = cost;
/*    cost =  1.5*cost + max/16 + 100*max_ind_cost - 0.25*grad_sum;
/*	cost =  cost + max/16 - 0.5*grad_sum ;*/
/*   cost =  cost + 2*max_ind_cost*max_ind_cost;*/
/*	printf("cost %f\n",cost);*/
/*	cost = max_ind_cost;*/
    return cost;
}
