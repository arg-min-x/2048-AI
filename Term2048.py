import random
from Tools2048 import *
import numpy as np
import math


class Term2048:

    # Override the initialization
    def __init__(self, **kwargs):

        self.cols = 4
        self.rows = 4

        # Create the 2D array to store the board
        self.game_board = [[0 for x in range(4)] for x in range(4)]

        # Create the previous game board
        self.previous_game_board = [[0 for x in range(4)] for x in range(4)]

        # Add the grid blocks
        self.display_board = [[0 for x in range(4)] for x in range(4)]

        # Initialize the board
        self._initialize_board()

        # Update the board
        self._update_board()

    # Make the next move on based on the input move
    def makeNextMove(self,move):
        # Update the board if the down arrow is pressed
        if move == 'down':
            self.previous_game_board = self.game_board
            self._down_move()
            if self._board_changed():
                self._add_new_block()
            self._update_board()

        # Update the board if the up arrow is pressed
        if move == 'up':
            self.previous_game_board = self.game_board
            self._up_move()
            if self._board_changed():
                self._add_new_block()
            self._update_board()

        # Update the board if the left arrow is pressed
        if move == 'left':
            self.previous_game_board = self.game_board
            self._left_move()
            if self._board_changed():
                self._add_new_block()
            self._update_board()

        # Update the board if the right arrow is pressed
        if move == 'right':
            self.previous_game_board = self.game_board
            self._right_move()
            if self._board_changed():
                self._add_new_block()
            self._update_board()
    # -------------------------- Function for initializing the game board --------------------------
    def _initialize_board(self):

        for row_ind in range(0,4):

            for col_ind in range(0,4):

                self.game_board[row_ind][col_ind] = 0

        row_ind = random.randint(0,3)
        col_ind = random.randint(0,3)
        rand_num = random.randint(0,1)

        if rand_num == 0:
            self.game_board[row_ind][col_ind] = 2
        else:
            self.game_board[row_ind][col_ind] = 4

    # -------------------------- Function for adding new block --------------------------
    def _add_new_block(self):

        open_space = 0

        while open_space == 0:

            row_ind = random.randint(0,3)
            col_ind = random.randint(0,3)

            if self.game_board[row_ind][col_ind] == 0:

                open_space = 1

        rand_num = random.randint(0,9)

        if rand_num == 0:
            self.game_board[row_ind][col_ind] = 4
        else:
            self.game_board[row_ind][col_ind] = 2

    # -------------------------- Function for updating the game board --------------------------
    def _update_board(self):

        return 0
        # for row_ind in range(0,4):

            # for col_ind in range(0,4):

                # if self.game_board[row_ind][col_ind] == 0:
                #     self.display_board[row_ind][col_ind].text = ' '
                # else:
                #     self.display_board[row_ind][col_ind].text = str(self.game_board[row_ind][col_ind])

    # -------------------------- Function for calculating down move --------------------------
    def _down_move(self):

        # Create the 2D array to store the board
        new_game_board = [[0 for x in range(4)] for x in range(4)]

        # Move the rows down
        for col_ind in range(0,4):

            temp_ind = 3;

            for row_ind in range(3,-1,-1):

                if self.game_board[row_ind][col_ind] != 0:

                    new_game_board[temp_ind][col_ind] = self.game_board[row_ind][col_ind]
                    temp_ind = temp_ind - 1

        # Combine like blocks
        for col_ind in range(0,4):

            row_ind_cond = 3

            while row_ind_cond != 0:

                if new_game_board[row_ind_cond][col_ind] == new_game_board[row_ind_cond-1][col_ind]:

                    new_game_board[row_ind_cond][col_ind] = 2*new_game_board[row_ind_cond-1][col_ind]

                    for row_ind in range(row_ind_cond-1,0,-1):

                        new_game_board[row_ind][col_ind] = new_game_board[row_ind-1][col_ind]

                    new_game_board[0][col_ind] = 0

                row_ind_cond = row_ind_cond - 1

        self.game_board = new_game_board

    # -------------------------- Function for calculating right move --------------------------
    def _right_move(self):

        # Create the 2D array to store the board
        new_game_board = [[0 for x in range(4)] for x in range(4)]

        # Move the rows down
        for col_ind in range(0,4):

            temp_ind = 3;

            for row_ind in range(3,-1,-1):

                if self.game_board[col_ind][row_ind] != 0:

                    new_game_board[col_ind][temp_ind] = self.game_board[col_ind][row_ind]
                    temp_ind = temp_ind - 1

        # Combine like blocks
        for col_ind in range(0,4):

            row_ind_cond = 3

            while row_ind_cond != 0:

                if new_game_board[col_ind][row_ind_cond] == new_game_board[col_ind][row_ind_cond-1]:

                    new_game_board[col_ind][row_ind_cond] = 2*new_game_board[col_ind][row_ind_cond-1]

                    for row_ind in range(row_ind_cond-1,0,-1):

                        new_game_board[col_ind][row_ind] = new_game_board[col_ind][row_ind-1]

                    new_game_board[col_ind][0] = 0

                row_ind_cond = row_ind_cond - 1

        self.game_board = new_game_board

    # -------------------------- Function for calculating up move --------------------------
    def _up_move(self):

        # Create the 2D array to store the board
        new_game_board = [[0 for x in range(4)] for x in range(4)]

        # Move the rows down
        for col_ind in range(0,4):

            temp_ind = 0;

            for row_ind in range(0,4):

                if self.game_board[row_ind][col_ind] != 0:

                    new_game_board[temp_ind][col_ind] = self.game_board[row_ind][col_ind]
                    temp_ind = temp_ind + 1

        # Combine like blocks
        for col_ind in range(0,4):

            row_ind_cond = 0

            while row_ind_cond != 3:

                if new_game_board[row_ind_cond][col_ind] == new_game_board[row_ind_cond+1][col_ind]:

                    new_game_board[row_ind_cond][col_ind] = 2*new_game_board[row_ind_cond+1][col_ind]

                    for row_ind in range(row_ind_cond+1,3):

                        new_game_board[row_ind][col_ind] = new_game_board[row_ind+1][col_ind]

                    new_game_board[3][col_ind] = 0

                row_ind_cond = row_ind_cond + 1

        self.game_board = new_game_board

    # -------------------------- Function for calculating left move --------------------------
    def _left_move(self):

        # Create the 2D array to store the board
        new_game_board = [[0 for x in range(4)] for x in range(4)]

        # Move the rows down
        for col_ind in range(0,4):

            temp_ind = 0;

            for row_ind in range(0,4):

                if self.game_board[col_ind][row_ind] != 0:

                    new_game_board[col_ind][temp_ind] = self.game_board[col_ind][row_ind]
                    temp_ind = temp_ind + 1

        # Combine like blocks
        for col_ind in range(0,4):

            row_ind_cond = 0

            while row_ind_cond != 3:

                if new_game_board[col_ind][row_ind_cond] == new_game_board[col_ind][row_ind_cond+1]:

                    new_game_board[col_ind][row_ind_cond] = 2*new_game_board[col_ind][row_ind_cond+1]

                    for row_ind in range(row_ind_cond+1,3):

                        new_game_board[col_ind][row_ind] = new_game_board[col_ind][row_ind+1]

                    new_game_board[col_ind][3] = 0

                row_ind_cond = row_ind_cond + 1

        self.game_board = new_game_board


    # -------------------------- Function for checking if the board has changed --------------------------
    def _board_changed(self):

        changed = 0

        for col_ind in range(0,4):

            for row_ind in range(0,4):

                if self.previous_game_board[row_ind][col_ind] != self.game_board[row_ind][col_ind]:

                    changed = 1

        return changed

