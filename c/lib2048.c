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
void create_tree_root(struct rand_node *root, int depth){
    
    // If at the end of a tree
    // Alocate the child nodes
    struct move_node *left, *right, *up, *down;
    left = malloc(sizeof(move_node));
    right = malloc(sizeof(move_node));
    up = malloc(sizeof(move_node));
    down = malloc(sizeof(move_node));
    
    uint8_t tmp_board 
    // Asign child nodes
    root->left = left;
    root->right = right;
    root->up = up;
    root->down = down;
    
    // Asign child move boards
    root->left->game_board = move_left(root->game_board);
    root->right->game_board = move_right(root->game_board);
    root->up->game_board = move_up(root->game_board);
    root->down->game_board = move_down(root->game_board);
    
    // Set the number of moves
    root->left->num_moves = 2*count_zeros(root->left->game_board);
    root->right->num_moves = 2*count_zeros(root->right->game_board);
    root->up->num_moves = 2*count_zeros(root->up->game_board);
    root->down->num_moves = 2*count_zeros(root->down->game_board);
    
    // Construct child nodes
    create_children_move_node(left);
    create_children_move_node(right);
    create_children_move_node(up);
    create_children_move_node(down);

    // Asign random move boards

//         create random board for right node
    int last_ind = 0;
    uint8_t rand_val = 1;
//        printf("\n");
//        printf("start random boards\n");
//        print_game_board(root->left->game_board);
//        printf("\n");
    for (int ind = 0; ind < root->left->num_moves/2; ind++) {
        root->left->moves[ind]->game_board = create_random_board(root->left->game_board,&last_ind,rand_val);
//            print_game_board(root->left->moves[ind]->game_board);
//            printf("\n");
    }
    last_ind = 0;
    rand_val = 2;
//        printf("\n");
//        printf("start random boards\n");
//        print_game_board(root->left->game_board);
//        printf("\n");
    for (int ind = root->left->num_moves/2; ind < root->left->num_moves; ind++) {
        root->left->moves[ind]->game_board = create_random_board(root->left->game_board,&last_ind,rand_val);
//            print_game_board(root->left->moves[ind]->game_board);
//            printf("\n");
    }
    
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

//        for (int ind = 0; ind < root->left->num_moves; ind++) {
//            root->left->moves[ind]->game_board = move_left(root->game_board);
//        }
//        
//        for (int ind = 0; ind < root->right->num_moves; ind++) {
//            root->right->moves[ind]->game_board = move_left(root->game_board);
//        }
//        
//        for (int ind = 0; ind < root->up->num_moves; ind++) {
//            root->up->moves[ind]->game_board = move_left(root->game_board);
//        }
//        
//        for (int ind = 0; ind < root->down->num_moves; ind++) {
//            root->down->moves[ind]->game_board = move_left(root->game_board);
//        }
    if (depth >1) {

		omp_set_num_threads(4);
        // Call Constructor on the next level
		#pragma omp parallel
		#pragma omp sections nowait
		{
			#pragma omp section
			{
				for (int ind = 0; ind < root->left->num_moves; ind++) {
				    create_tree(root->left->moves[ind], depth-1);
				}
		    }
		
			#pragma omp section
			{
				for (int ind = 0; ind < root->right->num_moves; ind++) {
				    create_tree(root->right->moves[ind], depth-1);
				}
		    }

			#pragma omp section
			{
				for (int ind = 0; ind < root->up->num_moves; ind++) {
				    create_tree(root->up->moves[ind], depth-1);
				}
		    }

			#pragma omp section
			{
				for (int ind = 0; ind < root->down->num_moves; ind++) {
				    create_tree(root->down->moves[ind], depth-1);
				}
			}
		}
    }   
}

// Create The tree
void create_tree(struct rand_node *root, int depth){
    
    // If at the end of a tree
    // Alocate the child nodes
    struct move_node *left, *right, *up, *down;
    left = malloc(sizeof(move_node));
    right = malloc(sizeof(move_node));
    up = malloc(sizeof(move_node));
    down = malloc(sizeof(move_node));
    
    // Asign child nodes
    root->left = left;
    root->right = right;
    root->up = up;
    root->down = down;
    
    // Asign child move boards
    root->left->game_board = move_left(root->game_board);
    root->right->game_board = move_right(root->game_board);
    root->up->game_board = move_up(root->game_board);
    root->down->game_board = move_down(root->game_board);
    
    // Set the number of moves
    root->left->num_moves = 2*count_zeros(root->left->game_board);
    root->right->num_moves = 2*count_zeros(root->right->game_board);
    root->up->num_moves = 2*count_zeros(root->up->game_board);
    root->down->num_moves = 2*count_zeros(root->down->game_board);
    
    // Construct child nodes
    create_children_move_node(left);
    create_children_move_node(right);
    create_children_move_node(up);
    create_children_move_node(down);

    // Asign random move boards

//         create random board for right node
    int last_ind = 0;
    uint8_t rand_val = 1;
//        printf("\n");
//        printf("start random boards\n");
//        print_game_board(root->left->game_board);
//        printf("\n");
    for (int ind = 0; ind < root->left->num_moves/2; ind++) {
        root->left->moves[ind]->game_board = create_random_board(root->left->game_board,&last_ind,rand_val);
//            print_game_board(root->left->moves[ind]->game_board);
//            printf("\n");
    }
    last_ind = 0;
    rand_val = 2;
//        printf("\n");
//        printf("start random boards\n");
//        print_game_board(root->left->game_board);
//        printf("\n");
    for (int ind = root->left->num_moves/2; ind < root->left->num_moves; ind++) {
        root->left->moves[ind]->game_board = create_random_board(root->left->game_board,&last_ind,rand_val);
//            print_game_board(root->left->moves[ind]->game_board);
//            printf("\n");
    }
    
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

//        for (int ind = 0; ind < root->left->num_moves; ind++) {
//            root->left->moves[ind]->game_board = move_left(root->game_board);
//        }
//        
//        for (int ind = 0; ind < root->right->num_moves; ind++) {
//            root->right->moves[ind]->game_board = move_left(root->game_board);
//        }
//        
//        for (int ind = 0; ind < root->up->num_moves; ind++) {
//            root->up->moves[ind]->game_board = move_left(root->game_board);
//        }
//        
//        for (int ind = 0; ind < root->down->num_moves; ind++) {
//            root->down->moves[ind]->game_board = move_left(root->game_board);
//        }
    if (depth >1) {

        // Call Constructor on the next level
		for (int ind = 0; ind < root->left->num_moves; ind++) {
		    create_tree(root->left->moves[ind], depth-1);
		}

		for (int ind = 0; ind < root->right->num_moves; ind++) {
		    create_tree(root->right->moves[ind], depth-1);
		}

		for (int ind = 0; ind < root->up->num_moves; ind++) {
		    create_tree(root->up->moves[ind], depth-1);
		}

		for (int ind = 0; ind < root->down->num_moves; ind++) {
		    create_tree(root->down->moves[ind], depth-1);
		}
    }   
}
// Destroy the tree
void destroy_tree(struct rand_node *root){
    
    if (root->left != 0) {
        
        // Keep going down the tree if not empty
        for (int ind = 0; ind<root->left->num_moves; ind++) {
            destroy_tree(root->left->moves[ind]);
        }
        
        // Destroy the game boards for random nodes
        if (root->left->moves[0]->game_board !=0) {
            for (int ind = 0; ind < root->left->num_moves; ind++) {
                free(root->left->moves[ind]->game_board);
            }
        }
        
        // Destroy random nodes
        for (int ind=0; ind<root->left->num_moves; ind++) {
            free(root->left->moves[ind]);
        }
        free(root->left->game_board);
        free(root->left->moves);
        
        // destroy move nodes and root
        free(root->left);
    }
    if (root->right != 0) {
        
        // Keep going down the tree if not empty
        for (int ind = 0; ind<root->right->num_moves; ind++) {
            destroy_tree(root->right->moves[ind]);
        }
        
        if (root->right->moves[0]->game_board !=0) {
            for (int ind = 0; ind < root->right->num_moves; ind++) {
                free(root->right->moves[ind]->game_board);
            }
        }
        
        // Destroy random nodes
        for (int ind=0; ind<root->right->num_moves; ind++) {
            free(root->right->moves[ind]);
        }

        free(root->right->moves);
        free(root->right->game_board);
        
        // destroy move nodes and root
        free(root->right);
    }
    if (root->up != 0) {
        
        // Keep going down the tree if empty
        for (int ind = 0; ind<root->up->num_moves; ind++) {
            destroy_tree(root->up->moves[ind]);
        }

        if (root->up->moves[0]->game_board !=0) {
            for (int ind = 0; ind < root->up->num_moves; ind++) {
                free(root->up->moves[ind]->game_board);
            }
        }
        
        // Destroy random nodes
        for (int ind=0; ind<root->up->num_moves; ind++) {
            free(root->up->moves[ind]);
        }

        free(root->up->moves);
        free(root->up->game_board);
        
        // destroy move nodes and root
        free(root->up);
    }
    if (root->down != 0) {
        
        // Keep going down the tree if empty
        for (int ind = 0; ind<root->down->num_moves; ind++) {
            destroy_tree(root->down->moves[ind]);
        }
        
        if (root->down->moves[0]->game_board !=0) {
            for (int ind = 0; ind < root->down->num_moves; ind++) {
                free(root->down->moves[ind]->game_board);
            }
        }
        
        // Destroy random nodes
        for (int ind=0; ind<root->down->num_moves; ind++) {
            free(root->down->moves[ind]);
        }

        free(root->down->moves);
        free(root->down->game_board);
        
        // destroy move nodes and root
        free(root->down);
    }
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
uint8_t *move_right(uint8_t *game_board){
    
    // Allocate the new board and copy the old board into it
    uint8_t *move_board = malloc(16*sizeof(uint8_t));
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
uint8_t *move_left(uint8_t *game_board){
    
    uint8_t *move_board = malloc(16*sizeof(uint8_t));
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
uint8_t *move_up(uint8_t *game_board){
    
    uint8_t *move_board = malloc(16*sizeof(uint8_t));
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
uint8_t *move_down(uint8_t *game_board){
    
    uint8_t *move_board = malloc(16*sizeof(uint8_t));
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
    cost = ((double)count_zeros(game_board))/15;

	// Find the maximum of the board
    int max = 0;
	uint8_t max_ind = 0;
	uint8_t max_ind_cost = 0;
    for (int ind = 0; ind<16; ind++) {
        if (max<game_board[ind]) {
            max = (int)game_board[ind];
			max_ind = ind;
        }
    }
	if (max_ind==0){
		max_ind_cost = 1;
	}

/*	// apply a smoothness constraint*/
/*    int grad[18];*/
/*	int grad_sum=0;*/
/*	int ind_2 = 0;*/
/*	grad[0] = game_board[0];*/
/*	grad[17] = game_board[12];*/
/*	*/
/*	for (int ind = 0; ind<4; ind++){*/
/*		grad[ind+1] = game_board[ind];*/
/*	}*/
/*	for (int ind = 0; ind<4; ind++){*/
/*		grad[ind+1] = game_board[ind];*/
/*	}*/
/*	ind_2 = 7;*/
/*	for (int ind = 4; ind<8; ind++){*/
/*		grad[ind+1] = game_board[ind_2];*/
/*		ind_2--;*/
/*	}*/
/*	for (int ind = 8; ind<12; ind++){*/
/*		grad[ind+1] = game_board[ind];*/
/*	}*/
/*	ind_2 = 15;*/
/*	for (int ind = 12; ind<16; ind++){*/
/*		grad[ind+1] = game_board[ind_2];*/
/*		ind_2--;*/
/*	}*/

/*	for (int ind = 0; ind<17; ind++){*/
/*		grad[ind] = abs(grad[ind]);*/
/*	}*/
/*	grad[17] = 0;*/

/*	for (int ind = 0; ind<17; ind++){*/
/*		grad_sum =+ grad[ind]*grad[ind]*grad[ind];*/
/*	}*/

		// apply a smoothness constraint
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


    
    cost = cost - 1.5*(double)grad_sum + max_ind_cost;
    return cost;
}

// ========================================================================================
// Eval next move
void eval_next_move(struct rand_node *root){
    int is_terminal = 0;
    float left_cost, right_cost, up_cost, down_cost;
    left_cost = 0;
    right_cost = 0;
    up_cost = 0;
    down_cost = 0;
    
    if (root->left != 0) {
        
        // Keep going down the tree if not at the end
        for (int ind = 0; ind<root->left->num_moves; ind++) {
            eval_next_move(root->left->moves[ind]);
        }
        
        // Calculate the expectimax value from the previous value
        for (int ind = 0; ind<root->left->num_moves/2; ind++) {
            left_cost += 0.9*root->left->moves[ind]->cost/root->left->num_moves/2.0;
        }

        for (int ind = root->left->num_moves/2; ind<root->left->num_moves; ind++) {
            left_cost += 0.1*root->left->moves[ind]->cost/root->left->num_moves/2.0;
        }
        
    }
    
    if (root->right != 0) {
        
        // Keep going down the tree if not at the end
        for (int ind = 0; ind<root->right->num_moves; ind++) {
            eval_next_move(root->right->moves[ind]);
        }
        
        // Calculate the expectimax value from the previous value
        for (int ind = 0; ind<root->right->num_moves/2; ind++) {
            right_cost += 0.9*root->right->moves[ind]->cost/root->right->num_moves/2.0;
        }
        
        for (int ind = root->right->num_moves/2; ind<root->right->num_moves; ind++) {
            right_cost += 0.1*root->right->moves[ind]->cost/root->right->num_moves/2.0;
        }
    }
    if (root->up != 0) {
            
        // Keep going down the tree if not at the end
        for (int ind = 0; ind<root->up->num_moves; ind++) {
            eval_next_move(root->up->moves[ind]);
        }
        
        // Calculate the expectimax value from the previous value
        for (int ind = 0; ind<root->up->num_moves/2; ind++) {
            up_cost += 0.9*root->up->moves[ind]->cost/root->up->num_moves/2.0;
        }
        
        for (int ind = root->up->num_moves/2; ind<root->up->num_moves; ind++) {
            up_cost += 0.1*root->up->moves[ind]->cost/root->up->num_moves/2.0;
        }
    }
    
    if (root->down != 0) {
            
        // Keep going down the tree if not at the end
        for (int ind = 0; ind<root->down->num_moves; ind++) {
            eval_next_move(root->down->moves[ind]);
        }
        
        // Calculate the expectimax value from the previous value
        for (int ind = 0; ind<root->down->num_moves/2; ind++) {
            down_cost += 0.9*root->down->moves[ind]->cost/root->down->num_moves/2.0;
        }
        
        for (int ind = root->down->num_moves/2; ind<root->down->num_moves; ind++) {
            down_cost += 0.1*root->down->moves[ind]->cost/root->down->num_moves/2.0;
        }
    }
    
    // If at the end of the tree eveluate the cost of the leaf
    if (root->left == 0 && root->right == 0 && root->up == 0 && root->down == 0) {
        root->cost = eval_cost(root->game_board);
        is_terminal = 1;
    }

    // Pick the best cost here
    if (is_terminal==0) {
        float max = 0;
        
        max = left_cost;
        if (right_cost>max) {
            max = right_cost;
        }
        if (up_cost>max) {
            max = up_cost;
        }
        if (down_cost>max) {
            max = down_cost;
        }
        root->cost = max;
//        printf("root cost %f\n",root->cost);
//        printf("left cost%f\n",left_cost);
//        printf("right cost%f\n",right_cost);
//        printf("up cost%f\n",up_cost);
//        printf("down cost%f\n",down_cost);
    }
}

// ========================================================================================
// Eval next move
char eval_next_move_root(struct rand_node *root){
    int is_terminal = 0;
    float left_cost, right_cost, up_cost, down_cost;
    char next_move = 'l';
    left_cost = 0;
    right_cost = 0;
    up_cost = 0;
    down_cost = 0;
    
	#pragma omp parallel
	#pragma omp sections nowait
	{
		#pragma omp section
		{
			if (root->left != 0) {
				
				// Keep going down the tree if not at the end
				for (int ind = 0; ind<root->left->num_moves; ind++) {
				    eval_next_move(root->left->moves[ind]);
				}
				
				// Calculate the expectimax value from the previous value
				for (int ind = 0; ind<root->left->num_moves/2; ind++) {
				    left_cost += 0.9*root->left->moves[ind]->cost/root->left->num_moves/2.0;
				}
				
				for (int ind = root->left->num_moves/2; ind<root->left->num_moves; ind++) {
				    left_cost += 0.1*root->left->moves[ind]->cost/root->left->num_moves/2.0;
				}
				
			}
		}

		#pragma omp section
		{
			if (root->right != 0) {
			
				// Keep going down the tree if not at the end
				for (int ind = 0; ind<root->right->num_moves; ind++) {
					eval_next_move(root->right->moves[ind]);
				}
			
				// Calculate the expectimax value from the previous value
				for (int ind = 0; ind<root->right->num_moves/2; ind++) {
					right_cost += 0.9*root->right->moves[ind]->cost/root->right->num_moves/2.0;
				}
			
				for (int ind = root->right->num_moves/2; ind<root->right->num_moves; ind++) {
					right_cost += 0.1*root->right->moves[ind]->cost/root->right->num_moves/2.0;
				}
			}
		}

		#pragma omp section
		{
			if (root->up != 0) {
				
				// Keep going down the tree if not at the end
				for (int ind = 0; ind<root->up->num_moves; ind++) {
				    eval_next_move(root->up->moves[ind]);
				}
				
				// Calculate the expectimax value from the previous value
				for (int ind = 0; ind<root->up->num_moves/2; ind++) {
				    up_cost += 0.9*root->up->moves[ind]->cost/root->up->num_moves/2.0;
				}
				
				for (int ind = root->up->num_moves/2; ind<root->up->num_moves; ind++) {
				    up_cost += 0.1*root->up->moves[ind]->cost/root->up->num_moves/2.0;
				}
			}
    	}

		#pragma omp section
		{
			if (root->down != 0) {
				
				// Keep going down the tree if not at the end
				for (int ind = 0; ind<root->down->num_moves; ind++) {
				    eval_next_move(root->down->moves[ind]);
				}
				
				// Calculate the expectimax value from the previous value
				for (int ind = 0; ind<root->down->num_moves/2; ind++) {
				    down_cost += 0.9*root->down->moves[ind]->cost/root->down->num_moves/2.0;
				}
				
				for (int ind = root->down->num_moves/2; ind<root->down->num_moves; ind++) {
				    down_cost += 0.1*root->down->moves[ind]->cost/root->down->num_moves/2.0;
				}
			}
		}
	}
    
    // Pick the best cost here
    if (is_terminal==0) {
        float max = 0;
        max = left_cost;
        next_move = 'l';
        if (right_cost>max) {
            max = right_cost;
            next_move = 'r';
        }
        if (up_cost>max) {
            max = up_cost;
            next_move = 'u';
        }
        if (down_cost>max) {
            max = down_cost;
            next_move = 'd';
        }
        root->cost = max;
/*        printf("root cost %f\n",root->cost);*/
/*        printf("left cost%f\n",left_cost);*/
/*        printf("right cost%f\n",right_cost);*/
/*        printf("up cost%f\n",up_cost);*/
/*        printf("down cost%f\n",down_cost);*/
    }
    return next_move;
}
