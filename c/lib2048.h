//
//  lib2048.h
//  2048-AI
//
//  Created by Adam Rich on 4/11/16.
//  Copyright © 2016 Adam Rich. All rights reserved.
//

#ifndef lib2048_h
#define lib2048_h

#include <stdio.h>
#include <stdlib.h>

// ========================================================================================
//          Game Tree Structures and functions
// ========================================================================================

// Node with random board states as children
typedef struct move_node {
    struct rand_node **moves;
    uint8_t *game_board;
    uint8_t num_moves;
}move_node;

// Node with left up right down children
typedef struct rand_node {
    struct move_node *left, *right, *up, *down;
    uint8_t *game_board;
    float cost;
}rand_node;

// Set LRUD struct pointers to null
void set_move_null(struct rand_node *leaf);

// Create The tree
void create_tree(struct rand_node *root, int depth);

// Call the create tree function on each random node
void create_tree_next_level(struct move_node *leaf,int depth);

// Destroy the tree
void destroy_tree(struct rand_node *root);

// Evalutate Cost function for a leaf
float eval_cost(uint8_t *game_board);
    
// Eval next move
void eval_next_move(struct rand_node *root);

// ========================================================================================
//          Game Board Manipulation
// ========================================================================================

// Print the game board
void print_game_board(uint8_t *game_board);

// Move the game board left return pointer to new board
uint8_t *move_left(uint8_t *game_board);

// Move the game board right and return pointer to the new board
uint8_t *move_right(uint8_t *game_board);

// Move the game board up and return pointer to the new board
uint8_t *move_up(uint8_t *game_board);

// Move the game board down and return pointer to the new board
uint8_t *move_down(uint8_t *game_board);

// Count the number of zeros on the board
uint8_t count_zeros(uint8_t *game_board);

// Create a board with two or four added at a zero locations
uint8_t *create_random_board(uint8_t *game_board, int *last_zero_ind, uint8_t rand_value);

#endif /* lib2048_h */
