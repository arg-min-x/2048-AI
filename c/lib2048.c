//
//  lib2048.c
//  2048-AI
//
//  Created by Adam Rich on 4/11/16.
//  Copyright © 2016 Adam Rich. All rights reserved.
//

#include "lib2048.h"
#include <stdlib.h>

// ========================================================================================
//          Game Tree Structures and functions
// ========================================================================================
void set_move_node_null(struct rand_node *leaf){
    leaf->left = NULL;
    leaf->right = NULL;
    leaf->up = NULL;
    leaf->down = NULL;
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
void create_tree(struct rand_node *root, int depth){
    
    int number_moves = 14;
    
    // If at the end of a tree
    if (depth == 1) {
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
        
        // Set the number of moves
        root->left->num_moves = number_moves;
        root->right->num_moves = number_moves;
        root->up->num_moves = number_moves;
        root->down->num_moves = number_moves;
        
        // Construct child nodes
        create_children_move_node(left);
        create_children_move_node(right);
        create_children_move_node(up);
        create_children_move_node(down);
        
    }else if (depth >1) {
        
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
        
        // Set the number of moves
        root->left->num_moves = number_moves;
        root->right->num_moves = number_moves;
        root->up->num_moves = number_moves;
        root->down->num_moves = number_moves;
        
        // Construct child nodes
        create_children_move_node(left);
        create_children_move_node(right);
        create_children_move_node(up);
        create_children_move_node(down);
        
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
        
        // Destroy random nodes
        for (int ind=0; ind<root->left->num_moves; ind++) {
            free(root->left->moves[ind]);
        }
        free(root->left->moves);
        
        // destroy move nodes and root
        free(root->left);
    }
    if (root->right != 0) {
        
        // Keep going down the tree if not empty
        for (int ind = 0; ind<root->right->num_moves; ind++) {
            destroy_tree(root->right->moves[ind]);
        }
        
        // Destroy random nodes
        for (int ind=0; ind<root->right->num_moves; ind++) {
            free(root->right->moves[ind]);
        }
        free(root->right->moves);
        
        // destroy move nodes and root
        free(root->right);
    }
    if (root->up != 0) {
        
        // Keep going down the tree if empty
        for (int ind = 0; ind<root->up->num_moves; ind++) {
            destroy_tree(root->up->moves[ind]);
        }
        
        // Destroy random nodes
        for (int ind=0; ind<root->up->num_moves; ind++) {
            free(root->up->moves[ind]);
        }
        free(root->up->moves);
        
        // destroy move nodes and root
        free(root->up);
    }
    if (root->down != 0) {
        
        // Keep going down the tree if empty
        for (int ind = 0; ind<root->down->num_moves; ind++) {
            destroy_tree(root->down->moves[ind]);
        }
        
        // Destroy random nodes
        for (int ind=0; ind<root->down->num_moves; ind++) {
            free(root->down->moves[ind]);
        }
        free(root->down->moves);
        
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
