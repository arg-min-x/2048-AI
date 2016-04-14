
//
//  main.c
//  2048-AI
//
//  Created by Adam Rich on 4/10/16.
//  Copyright © 2016 Adam Rich. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "lib2048.h"


int main(int argc, const char * argv[]) {
    
    printf("blah");
    for (int ind = 0; ind<1; ind++) {
        // Allocate
        struct rand_node *root;
        root = malloc(sizeof(rand_node));
        
        // Create tree
        create_tree(root,4);
//        printf("blah");
        
        //Destory Tree
        destroy_tree(root);
        free(root);
    }
    
    printf("blah");
    
    return 0;
}



//
//// Create The tree
//void create_tree(struct rand_node *root, int depth){
//    
//    if (depth==1) {
//        // Alocate the child nodes
//        struct move_node *left, *right, *up, *down;
//        left = malloc(sizeof(move_node));
//        right = malloc(sizeof(move_node));
//        up = malloc(sizeof(move_node));
//        down = malloc(sizeof(move_node));
//        
//        // Asign child nodes
//        root->left = left;
//        root->right = right;
//        root->up = up;
//        root->down = down;
//        
//        // Construct child nodes
//        create_tree_move_node(left);
//        create_tree_move_node(right);
//        create_tree_move_node(up);
//        create_tree_move_node(down);
//    }
//    
//}


//// Destroy the tree
//void destroy_tree(struct rand_node *root){
//    
//    // Destroy child random nodes
//    destroy_rand_node_children(root->left);
//    destroy_rand_node_children(root->right);
//    destroy_rand_node_children(root->up);
//    destroy_rand_node_children(root->down);
//    
//    // Destroy child move nodes
//    free(root->left);
//    free(root->right);
//    free(root->up);
//    free(root->down);
//    
//    // Destroy Root
//    free(root);
//}






