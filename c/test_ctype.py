from ctypes import *
import os
import numpy as np


lib2048 = cdll.LoadLibrary(os.path.abspath("./lib2048.so"))

# Make an array of size 16 uint8_t integers
array = c_uint8 * 16
input = array(*[i for i in range(16)])

# define argtypes and return types
lib2048.print_game_board.argtypes = POINTER(c_uint8),
lib2048.print_game_board.restype = None
lib2048.print_game_board(input)

# test move board
array = c_uint8 * 16
input = array(*[i for i in range(16)])
output = array(*[i for i in range(16)])
lib2048.move_left.argtypes = POINTER(c_uint8),
lib2048.move_left.restype = POINTER(c_uint8)
output = lib2048.move_left(input, input)
lib2048.print_game_board(output)

# define array class
board_array = c_uint8 * 16

# Define input args to the function
lib2048.create_tree.argtypes = (POINTER(c_uint8), )
lib2048.create_tree.restype = POINTER(c_uint8)

max = []
b_sum = []
for ii in range(20):
    # Initialize to zero
    input = array(*[0 for i in range(16)])

    # Call the function on the input
    lib2048.play_2048(input)

    # Unpack the
    return_array = np.array([i for i in input])
    return_array = return_array.reshape(4, 4)
    return_array = np.power(2, return_array)
    max.append(return_array.max())
    b_sum.append(return_array.sum())
print("max")
print(max)

print('sum')
print(b_sum)
print(return_array)
