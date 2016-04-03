from Term2048 import Term2048
from Tools2048 import *
import numpy as np
import time
import sys

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    BLACK = '\33[30m'
    GOLD = '\33[33m'
    LBLUE = '\33[36m'
    LGREY = '\33[37m'

def getMarixString(m):
    x = '\t'
    for row in m:
        x += '\t'.join(str(item) for item in row)
        x += "\n\n\n\n\t"

    # Set some colors
    x = x.replace('0',bcolors.LGREY + '.' + bcolors.ENDC)
    x = x.replace('2',bcolors.WARNING + '2' + bcolors.ENDC)
    x = x.replace('4',bcolors.FAIL + '4' + bcolors.ENDC)
    x = x.replace('8',bcolors.LBLUE + '8' + bcolors.ENDC)
    x = x.replace('16',bcolors.LBLUE + '16' + bcolors.ENDC)
    x = x.replace('32',bcolors.LBLUE + '32' + bcolors.ENDC)
    x = x.replace('64',bcolors.LBLUE + '64' + bcolors.ENDC)
    x = x.replace('128',bcolors.LBLUE + '128' + bcolors.ENDC)
    x = x.replace('256',bcolors.LBLUE + '256' + bcolors.ENDC)
    x = x.replace('512',bcolors.LBLUE + '512' + bcolors.ENDC)
    x = x.replace('1024',bcolors.LBLUE + '1024' + bcolors.ENDC)
    x = x.replace('2024',bcolors.LBLUE + '2024' + bcolors.ENDC)
    return x


game = Term2048()
lose = 0

# Print Game Board
sys.stdout.write("\x1b[2J\x1b[H")
sys.stdout.write(getMarixString(game.game_board))


while not lose:

    # Take next move
    next_move = evalNextMove(game.game_board)
    game.makeNextMove(next_move)
    board = game.game_board

    # Print Game Board
    sys.stdout.write("\x1b[2J\x1b[H")
    sys.stdout.write(getMarixString(game.game_board))

    # Check if we lost
    validMoves = checkValidMoves(game.game_board)
    if np.sum(validMoves)==0:
        lose = 1

print('You lose!')
