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
#include <stdint.h>
#include <math.h>
#include <ncurses.h>
#include <unistd.h>

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
}rand_node;

// Set LRUD struct pointers to null
void set_move_null(struct rand_node *leaf);

// Create The tree
float create_tree(struct rand_node *root, int depth, int isroot);

// Call the create tree function on each random node
void create_tree_next_level(struct move_node *leaf,int depth);

// Destroy move node
void destroy_move_node(struct move_node *move);

// Destroy the tree
void destroy_tree(struct rand_node *root);

// Destory a move node
void destroy_move_node(struct move_node *move);

// Evalutate Cost function for a leaf
float eval_cost_new(uint8_t *game_board);

// Calculates the city block distance between all game board locs with val
float calc_cb_distances(uint8_t *game_board, int val);

// Calculates the city block distance between all game board locs with val
float calc_cb_distances_next(uint8_t *game_board, int val, int val2);

// ========================================================================================
//          Game Board Manipulation
// ========================================================================================

float get_cb_between_ideal(uint8_t *game_board, uint8_t *ideal_board, int val);

int comp (const void * elem1, const void * elem2);

void calc_ideal_board(uint8_t * game_board, uint8_t *ideal_board);

float gradient(uint8_t *game_b);

uint8_t *play_2048(uint8_t *game_board);
#endif /* lib2048_h */
