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
// Node with random board states as children
typedef struct move_node {
    struct rand_node *move1, *move2;
    uint8_t game_board[16];
}move_node;

// Node with left up right down children
typedef struct rand_node {
    struct move_node *left, *right, *up, *down;
    uint8_t game_board[16];
}rand_node;

void create_children_move_node(struct move_node *leaf);

// Create The tree
void create_tree(struct rand_node *root, int depth);

// Call the create tree function on each random node
void create_tree_next_level(struct move_node *leaf,int depth);

// Destroy random node children of a move node
void destroy_rand_node_children(struct move_node *child);

// Destroy the tree
void destroy_tree(struct rand_node *root);

// Destroy the next level of the tree
void destroy_tree_next_level(struct move_node *leaf);

#endif /* lib2048_h */
