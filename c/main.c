
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
#include <stdint.h>

//int main(int argc, const char * argv[]) {
//
//    uint8_t game_board_orig[16] = { 2, 5, 5, 6,
//                                    2, 2, 0, 2,
//                                    2, 0, 0, 2,
//                                    2, 2, 2, 3};
//    
//    uint8_t *game_board = malloc(16*sizeof(uint8_t));
//    uint8_t *right_board;
//    uint8_t *left_board;
//    uint8_t *up_board;
//    uint8_t *down_board;
//    uint8_t zero_count = 0;
//    
//    for (int ind = 0; ind<16; ind++) {
//        game_board[ind] = game_board_orig[ind];
//    }
//    
//    for (int ind = 0; ind<1; ind++) {
//        // Move right and print
//        printf("moving right\n");
//        print_game_board(game_board);
//        printf("\n\n");
//        
////        zero_count = count_zeros(game_board);
////        printf("zero count =\%d\n",zero_count);
//        
//        right_board = move_right(game_board);
//        print_game_board(right_board);
//        printf("\n\n");
//        
////        zero_count = count_zeros(right_board);
////        printf("zero count =\%d\n",zero_count);
//        
//        // Move left and print
//        printf("moving left\n");
//        print_game_board(game_board);
//        printf("\n\n");
//        
//        left_board = move_left(game_board);
//        print_game_board(left_board);
//        printf("\n\n");
//        
//        // Move up and print
//        printf("moving up\n");
//        print_game_board(game_board);
//        printf("\n\n");
//        
//        up_board = move_up(game_board);
//        print_game_board(up_board);
//        printf("\n\n");
//        
//        // Move down and print
//        // Move up and print
////        printf("moving down\n");
////        print_game_board(game_board);
////        printf("\n\n");
//        
//        down_board = move_down(game_board);
////        print_game_board(down_board);
////        printf("\n\n");
//        free(right_board);
//        free(left_board);
//        free(up_board);
//        free(down_board);
//    }


//    // Free Memory
//    free(game_board);
//
//    return 0;
//}

//// Test the tree creation and deletion
//int main(int argc, const char * argv[]) {
//    
//    uint8_t game_board_orig[16] = { 2, 2, 0, 8,
//        8, 0, 1, 0,
//        0, 3, 0, 4,
//        5, 0, 3, 1};
////    float cost = 0;
//
//    // Allocate
//    struct rand_node *root;
//    root = malloc(sizeof(rand_node));
//    root->game_board = &game_board_orig[0];
//    create_tree(root,4);
//    
//    printf("%lu",sizeof(rand_node));
//    
//    //Destory Tree
//    destroy_tree(root);
//    free(root);
//    
//    return 0;
//}


// Test the tree creation and deletion
int main(int argc, const char * argv[]) {
    
    uint8_t game_board_orig[16] = { 2, 2, 0, 0,
                                    3, 3, 0, 0,
                                    1, 1, 0, 4,
                                    2, 2, 3, 0};
    
//    uint8_t game_board_orig[16] = { 2, 3, 4, 8,
//                                    0, 5, 0, 7,
//                                    4, 3, 2, 1,
//                                    5, 7, 0, 0};
    float cost = 0;
//    printf("%lu\n",sizeof(float));
//    printf("%lu\n",sizeof(double));
//    printf("%lu\n",sizeof(long double));
//    printf("\n%lu\n",sizeof(rand_node));
//    printf("\n%lu\n",sizeof(move_node));
    
    for (int ind = 0; ind<1; ind++) {
        // Allocate
        struct rand_node *root;
        root = malloc(sizeof(rand_node));
        root->game_board = &game_board_orig[0];
        
//        printf("root game board\n");
//        print_game_board(root->game_board);
        
        // Create tree
        create_tree(root,3);
        
//        printf("left game board\n");
//        print_game_board(root->left->game_board);
//        printf("number of random moves = %d\n",root->left->num_moves);
//
//        printf("right game board\n");
//        print_game_board(root->right->game_board);
//        printf("number of random moves = %d\n",root->right->num_moves);
//        
//        printf("up game board\n");
//        print_game_board(root->up->game_board);
//        printf("number of random moves = %d\n",root->up->num_moves);
//        
//        printf("down game board\n");
//        print_game_board(root->down->game_board);
//        printf("number of random moves = %d\n",root->down->num_moves);
        char next_move = 'a';
        next_move = eval_next_move_root(root);
        printf("next move = %c",next_move);
        
        //Destory Tree
        destroy_tree(root);
        free(root);
    }
    
    return 0;
}

//// Test the tree creation and deletion
//int main(int argc, const char * argv[]) {
//    
//    for (int ind = 0; ind<1; ind++) {
//        // Allocate
//        struct rand_node *root;
//        root = malloc(sizeof(rand_node));
//        
//        // Create tree
//        create_tree(root,2);
//        
//        //Destory Tree
//        destroy_tree(root);
//        free(root);
//    }
//    
//    return 0;
//}

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






