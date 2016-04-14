//
//  lib2048.c
//  2048-AI
//
//  Created by Adam Rich on 4/11/16.
//  Copyright © 2016 Adam Rich. All rights reserved.
//

#include "lib2048.h"
#include <stdlib.h>

void create_children_move_node(struct move_node *leaf){
    struct rand_node *move1, *move2;
    move1 = malloc(sizeof(rand_node));
    move2 = malloc(sizeof(rand_node));
    leaf->move1 = move1;
    leaf->move2 = move2;
    
    leaf->move1->left = NULL;
    leaf->move1->right = NULL;
    leaf->move1->up = NULL;
    leaf->move1->down = NULL;
    
    leaf->move2->left = NULL;
    leaf->move2->right = NULL;
    leaf->move2->up = NULL;
    leaf->move2->down = NULL;
}

// Create The tree
void create_tree(struct rand_node *root, int depth){
    
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
        
        // Construct child nodes
        create_children_move_node(left);
        create_children_move_node(right);
        create_children_move_node(up);
        create_children_move_node(down);
        
        // Call Constructor on the next level
        create_tree_next_level(root->left, depth-1);
        create_tree_next_level(root->right, depth-1);
        create_tree_next_level(root->up, depth-1);
        create_tree_next_level(root->down, depth-1);
    }
    
}

// Call the create tree function on each random node of the parent
void create_tree_next_level(struct move_node *leaf,int depth){
    create_tree(leaf->move1, depth);
    create_tree(leaf->move2, depth);
    
}

// Destroy random node children of a move node
void destroy_rand_node_children(struct move_node *child){
    free(child->move1);
    free(child->move2);
}


// Destroy the tree
void destroy_tree(struct rand_node *root){
    
    if (root->left != 0) {
        
        // Keep going down the tree if empty
        destroy_tree_next_level(root->left);
        
        // Destroy random nodes
        destroy_rand_node_children(root->left);
        
        // destroy move nodes and root
        free(root->left);
    }
    if (root->right != 0) {
        
        // Keep going down the tree if empty
        destroy_tree_next_level(root->right);
        
        // Destroy random nodes
        destroy_rand_node_children(root->right);
        
        // destroy move nodes and root
        free(root->right);
    }
    if (root->up != 0) {
        
        // Keep going down the tree if empty
        destroy_tree_next_level(root->up);
        
        // Destroy random nodes
        destroy_rand_node_children(root->up);
        
        // destroy move nodes and root
        free(root->up);
    }
    if (root->left != 0) {
        
        // Keep going down the tree if empty
        destroy_tree_next_level(root->down);
        
        // Destroy random nodes
        destroy_rand_node_children(root->down);
        
        // destroy move nodes and root
        free(root->down);
    }
}

void destroy_tree_next_level(struct move_node *leaf){
    destroy_tree(leaf->move1);
    destroy_tree(leaf->move2);
}