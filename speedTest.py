# from gameTree import *
from Tools2048 import *

game_board = np.array([[0, 4, 4, 16], [0, 0, 8, 32], [2, 4, 4, 2], [4, 2, 2, 4]],dtype='int')
# game_board = np.array([[0, 0, 0, 16], [0, 0, 8, 0], [0, 4, 4, 2], [4, 2, 2, 4]],dtype='int')

rand_node = RandomNode(game_board, 3,1)