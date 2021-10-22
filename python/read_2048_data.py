import csv
import numpy as np
import os
import pathlib

s_path = pathlib.Path(r'D:\2048-AI\data\random')
files = os.listdir(s_path)
game_boards = np.zeros((0, 16))
scores = np.zeros((0, 2))
for file in files:
    print('reading file ' + str(s_path/file))
    game_data = np.zeros((0, 18))
    with open(s_path/file, 'r') as csvfile:
        data2048 = csv.reader(csvfile, delimiter=',')
        for ind, row in enumerate(data2048):
            game_data = np.concatenate([game_data, np.reshape(list(map(float, row)), [1, 18])], axis=0)
    game_data[:, -1] = game_data[-1, -1]
    game_data[:, -2] = -game_data[:, -2] + game_data[-1, -2]

    game_boards = np.concatenate([game_boards, game_data[:, 0:16]], axis=0)
    scores = np.concatenate([scores, game_data[:, 16:18]], axis=0)

np.save('game_board_rand', game_boards)
np.save('scores_rand', scores)
