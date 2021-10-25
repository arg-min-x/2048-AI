#include "lib2048.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ncurses.h>

void print_game_board(uint8_t *game_board);

void print_game_boardw(uint8_t *game_board);

// ========================================================================================
// Move Right
uint8_t *move_right(uint8_t *game_board, uint8_t *move_board);

// ========================================================================================
// Move the Game Board left
uint8_t *move_left(uint8_t *game_board, uint8_t *move_board);

// ========================================================================================
// Move the Game Board up
uint8_t *move_up(uint8_t *game_board, uint8_t *move_board);

// ========================================================================================
// Move the Game Board down
uint8_t *move_down(uint8_t *game_board, uint8_t *move_board);

// ========================================================================================
// Count the number of zeros on the board
uint8_t count_zeros(uint8_t *game_board);

// Compare to boards return 1 if they are the same
uint8_t compare_board(uint8_t *board1, uint8_t *board2);

// ========================================================================================
// Create a board with two or four added at a zero locations
uint8_t *create_random_board(uint8_t *game_board, int *last_zero_ind, uint8_t rand_value);

// ========================================================================================
// adds a 2 or 4 to the board in a random location
uint8_t *add_random_number(uint8_t *game_board);

