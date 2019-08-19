from multiprocessing.pool import Pool
import numpy as np
import keras


cost_model = keras.models.load_model('bad_model')
# cost_model.summary()
costEval = lambda x: -cost_model.predict(np.reshape(np.array(x), (1, 16)))
# costEval = lambda x: costEval2(x)
# Evaluate the cost function of a game board
def costEval(game_board):
    # Calculate the game score
    game_board = np.array(game_board)
    # print(game_board)
    # print(np.reshape(np.array(game_board), (1, 16)))
    # score = np.sum(game_board)
    # print 'Your Score is %d' %(score)

    # # Calculate distance between largest values on the board
    # indices = np.array(np.where(game_board == game_board.max()), dtype='double')
    # distance1 = 0
    # for j in range(np.size(indices, axis=1)):
    #     for i in range(j, np.size(indices, axis=1)-1):
    #         distance1 = distance1 + calcDistance(indices[0:2, j], indices[0:2, i+1])
    #
    # if game_board.max() > 8:
    #     indices = np.array(np.where(game_board == game_board.max()/2), dtype='double')
    #     distance2 = 0
    #     for j in range(np.size(indices, axis=1)):
    #         for i in range(j, np.size(indices, axis=1)-1):
    #             distance2 = distance2 + calcDistance(indices[0:2, j], indices[0:2, i+1])
    # else:
    #     distance2 = 0
    #
    # if game_board.max() > 16:
    #     indices = np.array(np.where(game_board == game_board.max()/4), dtype='double')
    #     distance3 = 0
    #     for j in range(np.size(indices, axis=1)):
    #         for i in range(j, np.size(indices, axis=1)-1):
    #             distance3 = distance3 + calcDistance(indices[0:2, j], indices[0:2, i+1])
    # else:
    #     distance3 = 0
    #
    # if game_board.max() > 32:
    #     indices = np.array(np.where(game_board == game_board.max()/8), dtype='double')
    #     distance4 = 0
    #     for j in range(np.size(indices, axis=1)):
    #         for i in range(j, np.size(indices, axis=1)-1):
    #             distance4 = distance4 + calcDistance(indices[0:2, j], indices[0:2, i+1])
    # else:
    #     distance4 = 0

    # Prefer solutions where the maximum is in the corner
    if game_board.max() > 4:
        indices = np.array(np.where(game_board == game_board.max()), dtype='double')
        cornerDist = np.zeros(4)
        cornerDist[0] = calcDistance(indices[0:2, 0], [0, 0])
        cornerDist[1] = calcDistance(indices[0:2, 0], [0, 3])
        cornerDist[2] = calcDistance(indices[0:2, 0], [3, 0])
        cornerDist[3] = calcDistance(indices[0:2, 0], [3, 3])

        cornerDist = cornerDist.min() * np.log2(game_board.max())
        # print cornerDist
    else:
        cornerDist = 0

    # # Pefure smooth solutions
    # h_grad = game_board.copy()
    # h_grad[:,0:-1] =  game_board[:,1:] - game_board[:,0:-1]
    # h_grad[:,-1] = 0
    # # print game_board
    # # print h_grad
    # # print np.sum(np.abs(h_grad))
    # h_cost = np.sum(np.abs(h_grad))
    #
    # v_grad = game_board.copy()
    # v_grad[0:-1,:] =  game_board[1:,:] - game_board[0:-1,:]
    # v_grad[-1,:] = 0
    # v_cost = np.sum(np.abs(v_grad))
    # # print v_grad
    # # print np.sum(np.abs(v_grad))

    mono_col = np.hstack((game_board[:, 0], game_board[-1:-5:-1, 1], game_board[:, 2], game_board[-1:-5:-1, 3]))
    mon_col_asc = np.log2(np.sum(np.abs(mono_col[1:16] - mono_col[0:15] + 1)))
    # mon_col_dec = np.sum(np.log2(np.abs(mono_col[1:16] - 1/2*mono_col[0:15]+1)))

    # print mono_col[1:16]
    # print 2*mono_col[0:15]

    mono_row = np.transpose(game_board)
    mono_row = np.hstack((mono_row[:, 0], mono_row[-1:-5:-1, 1], mono_row[:, 2], mono_row[-1:-5:-1, 3]))
    mon_row_asc = np.log2(np.sum(np.abs(mono_row[1:16] - mono_row[0:15] + 1)))

    # print game_board
    # print ''
    # print mono_row[1:16]
    # print 2*mono_row[0:15]

    # find the number of open spaces
    numblocks = np.where(game_board == 0)
    numblocks = np.size(numblocks, axis=1)
    numblocks = 15 - numblocks

    # Calculate Cost
    # cost = distance1 + distance2 + distance3 + distance4 + numblocks + cornerDist - game_board.max()
    # cost = 2*numblocks + cornerDist + 4*np.log2(h_cost) + 4*np.log2(v_cost) - game_board.max()
    cost = 2 * numblocks + 2 * cornerDist + 8 * np.min([mon_row_asc, mon_col_asc]) - game_board.max()
    return cost


# Calculate the distance between two values
def calcDistance(coords1, coords2):
    distance = np.power(np.power(coords1[0] - coords2[0], 2) + np.power(coords1[1] - coords2[1], 2), 0.5)
    return distance


# Generate all possible game boards from a given board state
def genBoards(game_board):
    zeroInd = np.array(np.where(np.array(game_board) == 0))
    num_boards = np.size(zeroInd, axis=1)
    game_board = np.array(game_board)
    boards = np.tile(game_board[:, :, np.newaxis], [1, 1, 2 * num_boards])

    # All boards with a 2  and added
    for ind in range(0, num_boards):
        boards[zeroInd[0, ind], zeroInd[1, ind], ind] = 2
    for ind in range(0, num_boards):
        boards[zeroInd[0, ind], zeroInd[1, ind], ind + num_boards] = 4

    return boards


# A class for the Random chance nodes in the game tree.  Also the root node
class RandomNode:

    # Build the tree
    def __init__(self, game_board, depth, root):

        self.parent = []
        self.expectimax_val = []
        self.move = []

        # If root node
        if root:

            # Use parallel execution
            self.game_board = game_board
            usePool = 1

            if usePool:

                # Calculate the tree using a multiple process
                pool = Pool(processes=4)
                validMove = checkValidMoves(game_board)
                try:
                    # Evaluate the branches of the tree
                    if validMove[0]:
                        # self.right = MoveNode(right_move_return(game_board), depth)
                        right_game_board = right_move_return(game_board)
                        right_depth = depth
                        right_result = pool.apply_async(MoveNode, (right_game_board, right_depth))
                    else:
                        self.right = []
                    if validMove[1]:
                        # self.left = MoveNode(left_move_return(game_board), depth)
                        left_game_board = left_move_return(game_board)
                        left_depth = depth
                        left_result = pool.apply_async(MoveNode, (left_game_board, left_depth))
                    else:
                        self.left = []
                    if validMove[2]:
                        # self.up = MoveNode(up_move_return(game_board), depth)
                        up_game_board = up_move_return(game_board)
                        up_depth = depth
                        up_result = pool.apply_async(MoveNode, (up_game_board, up_depth))
                    else:
                        self.up = []
                    if validMove[3]:
                        # self.down = MoveNode(down_move_return(game_board), depth)
                        down_game_board = down_move_return(game_board)
                        down_depth = depth
                        down_result = pool.apply_async(MoveNode, (down_game_board, down_depth))
                    else:
                        self.down = []

                    # Get the results from parallel pool
                    if validMove[0]:
                        self.right = right_result.get()
                    if validMove[1]:
                        self.left = left_result.get()
                    if validMove[2]:
                        self.up = up_result.get()
                    if validMove[3]:
                        self.down = down_result.get()

                # Close the pool on exception
                except:
                    pool.close()
                    pool.terminate()
                    pool.join()
                # Close the pool
                pool.close()

            # Not using the parallel pool
            else:
                validMove = checkValidMoves(game_board)
            if validMove[0]:
                self.right = MoveNode(right_move_return(game_board), depth)
            else:
                self.right = []
            if validMove[1]:
                self.left = MoveNode(left_move_return(game_board), depth)
            else:
                self.left = []
            if validMove[2]:
                self.up = MoveNode(up_move_return(game_board), depth)
            else:
                self.up = []
            if validMove[3]:
                self.down = MoveNode(down_move_return(game_board), depth)
            else:
                self.down = []

        # Build branches not from the root node
        else:
            # self.left = MoveNode(left_move_return(game_board), depth)
            # self.right = MoveNode(right_move_return(game_board), depth)
            # self.up = MoveNode(up_move_return(game_board), depth)
            # self.down = MoveNode(down_move_return(game_board), depth)
            validMove = checkValidMoves(game_board)

            if validMove[0]:
                self.right = MoveNode(right_move_return(game_board), depth)
            else:
                self.right = []
            if validMove[1]:
                self.left = MoveNode(left_move_return(game_board), depth)
            else:
                self.left = []
            if validMove[2]:
                self.up = MoveNode(up_move_return(game_board), depth)
            else:
                self.up = []
            if validMove[3]:
                self.down = MoveNode(down_move_return(game_board), depth)
            else:
                self.down = []

    # Evaluate the expectimax value from each branch
    def eval_cost(self, isroot):

        # Check if at root node
        if isroot:
            # Find the valid next moves, and only calculate cost for valid moves
            validMoves = checkValidMoves(self.game_board)
            usePool = 1
            # Use a parralel pool
            if usePool:
                pool = Pool(processes=4)
                if validMoves[0]:
                    if self.right:
                        # right_cost = self.right.eval_cost()
                        right_eval = pool.apply_async(self.right.eval_cost)
                    else:
                        right_cost = 100000
                else:
                    right_cost = 100000

                if validMoves[1]:
                    if self.left:
                        # left_cost  = self.left.eval_cost()
                        left_eval = pool.apply_async(self.left.eval_cost)
                    else:
                        left_cost = 100000
                else:
                    left_cost = 100000

                if validMoves[2]:
                    if self.up:
                        # up_cost = self.up.eval_cost()
                        up_eval = pool.apply_async(self.up.eval_cost)
                    else:
                        up_cost = 100000
                else:
                    up_cost = 100000

                if validMoves[3]:
                    if self.down:
                        # down_cost = self.down.eval_cost()
                        down_eval = pool.apply_async(self.down.eval_cost)
                    else:
                        down_cost = 100000
                else:
                    down_cost = 100000

                # Gather results form the parallel pool
                if validMoves[0]:
                    right_cost = right_eval.get()
                if validMoves[1]:
                    left_cost = left_eval.get()
                if validMoves[2]:
                    up_cost = up_eval.get()
                if validMoves[3]:
                    down_cost = down_eval.get()

                # Close the pool
                pool.close()

            # Use single threaded
            else:
                if validMoves[0]:
                    if self.right:
                        right_cost = self.right.eval_cost()
                        # right_eval = pool.apply_async(self.right.eval_cost)
                    else:
                        right_cost = 100000
                else:
                    right_cost = 100000

                if validMoves[1]:
                    if self.left:
                        left_cost = self.left.eval_cost()
                        # left_eval = pool.apply_async(self.left.eval_cost)
                    else:
                        left_cost = 100000
                else:
                    left_cost = 100000

                if validMoves[2]:
                    if self.up:
                        up_cost = self.up.eval_cost()
                        # up_eval = pool.apply_async(self.up.eval_cost)
                    else:
                        up_cost = 100000
                else:
                    up_cost = 100000

                if validMoves[3]:
                    if self.down:
                        down_cost = self.down.eval_cost()
                        # down_eval = pool.apply_async(self.down.eval_cost)
                    else:
                        down_cost = 100000
                else:
                    down_cost = 100000

        # If not the root node
        else:
            if self.left:
                left_cost = self.left.eval_cost()
            else:
                left_cost = 100000
            if self.right:
                right_cost = self.right.eval_cost()
            else:
                right_cost = 100000
            if self.up:
                up_cost = self.up.eval_cost()
            else:
                up_cost = 100000
            if self.down:
                down_cost = self.down.eval_cost()
            else:
                down_cost = 100000

        # Calculate the min cost of the children
        cost = np.array([left_cost, right_cost, up_cost, down_cost])
        ind = np.where(cost == cost.min())
        cost = cost.min()

        # pick the move with the minimum expected cost
        if ind[0][0] == 0:
            self.move = 'left'
        if ind[0][0] == 1:
            self.move = 'right'
        if ind[0][0] == 2:
            self.move = 'up'
        if ind[0][0] == 3:
            self.move = 'down'

        return cost

    # Set an attribute dynamically
    def __setattr__(self, name, value):
        self.__dict__[name] = value

    # Get an attribute dynamically
    def __getattr__(self, name):
        try:
            return self.__dict__[name]
        except KeyError:
            msg = "'{0}' object has no attribute '{1}'"
            raise AttributeError(msg.format(type(self).__name__, name))


class MoveNode:

    def __init__(self, game_board, depth):

        self.depth = depth
        self.boards = genBoards(game_board)

        if (depth - 1) >= 0:
            boards = genBoards(game_board)
            for ind in range(0, np.size(boards, axis=2)):
                name = 'child%d' % (ind)
                self.__setattr__(name, RandomNode(boards[:, :, ind], depth - 1, 0))
                # self.__setattr__(name,boards[:, :, ind])
        # else:
        #     self.boards = genBoards(game_board)

    def eval_cost(self):

        num_boards = np.size(self.boards, axis=2)
        costs = np.zeros([num_boards])

        # Terminal nodes return expected value of game boards
        if self.depth == 1:
            for ind in range(0, num_boards):
                costs[ind] = costEval(self.boards[:, :, ind])

        # Non terminal nodes call the children's cost eval function
        else:
            for ind in range(0, np.size(self.boards, axis=2)):
                name = 'child%d' % (ind)
                costs[ind] = self.__getattr__(name).eval_cost(0)

        if num_boards != 0:
            costs = (1 / float(num_boards)) * (0.9 * np.sum(costs[0:int(num_boards / 2)]) + 0.1 * np.sum(
                costs[int(num_boards / 2):num_boards]))
        else:
            costs = 100000
        return costs

    # Set an attribute dynamically
    def __setattr__(self, name, value):
        self.__dict__[name] = value

    # Get an attribute dynamically
    def __getattr__(self, name):
        try:
            return self.__dict__[name]
        except KeyError:
            msg = "'{0}' object has no attribute '{1}'"
            raise AttributeError(msg.format(type(self).__name__, name))


def evalNextMove(game_board):
    zeroInd = np.array(np.where(np.array(game_board) == 0), dtype='int16')
    num_boards = np.size(zeroInd, axis=1)

    if num_boards > 10:
        depth = 1
    elif num_boards > 3:
        depth = 2
    else:
        depth = 3
    rand_node = RandomNode(game_board, depth, 1)
    rand_node.eval_cost(1)
    next_move = rand_node.move

    # # if moving right is valid, get expected cost
    # if validMoves[0]:
    #     # Boards for the right move
    #     right_move_board = right_move_return(game_board)
    #     right_new_boards = genBoards(right_move_board)
    #     numBoards = np.size(right_new_boards,axis=2)
    #     right_costs = np.zeros([numBoards])
    #     for ind in range(0,numBoards):
    #         right_costs[ind] = costEval(right_new_boards[:,:,ind])
    #
    #     # right_costs = np.max(right_costs)
    #     right_cost = 0.9*np.sum(right_costs[0:numBoards/2:1]) + 0.1*np.sum(right_costs[numBoards/2:numBoards:1])
    # else:
    #     right_cost = 1000000
    #
    # # if moving left is valid, get expected cost
    # if validMoves[1]:
    #     # Boards for the left move
    #     left_move_board = left_move_return(game_board)
    #     left_new_boards = genBoards(left_move_board)
    #     numBoards = np.size(left_new_boards,axis=2)
    #     left_costs = np.zeros([numBoards])
    #
    #     for ind in range(0,numBoards):
    #         left_costs[ind] = costEval(left_new_boards[:,:,ind])
    #
    #     # left_costs = np.max(left_costs)
    #     left_cost = 0.9*np.sum(left_costs[0:numBoards/2:1]) + 0.1*np.sum(left_costs[numBoards/2:numBoards:1])
    # else:
    #     left_cost = 1000000
    #
    # # if moving up is valid, get expected cost
    # if validMoves[2]:
    #     # Boards for the up move
    #     up_move_board = up_move_return(game_board)
    #     up_new_boards = genBoards(up_move_board)
    #     numBoards = np.size(up_new_boards,axis=2)
    #     up_costs = np.zeros([numBoards])
    #
    #     for ind in range(0,numBoards):
    #         up_costs[ind] = costEval(up_new_boards[:,:,ind])
    #
    #     # up_cost = np.max(up_costs)
    #     up_cost = 0.9*np.sum(up_costs[0:numBoards/2:1]) + 0.1*np.sum(up_costs[numBoards/2:numBoards:1])
    # else:
    #     up_cost = 1000000
    #
    # # if moving down is valid, get expected cost
    # if validMoves[3]:
    #     # Boards for the down move
    #     down_move_board = down_move_return(game_board)
    #     down_new_boards = genBoards(down_move_board)
    #     numBoards = np.size(down_new_boards,axis=2)
    #     down_costs = np.zeros([numBoards])
    #     for ind in range(0,numBoards):
    #         down_costs[ind] = costEval(down_new_boards[:,:,ind])
    #
    #     # down_costs = np.max(down_costs)
    #     down_cost = 0.9*np.sum(down_costs[0:numBoards/2:1]) + 0.1*np.sum(down_costs[numBoards/2:numBoards:1])
    # else:
    #     down_cost = 1000000
    #
    # # Pick the move with the lowest cost
    # new_move = np.array([right_cost,left_cost,up_cost,down_cost])
    # new_move = np.where(new_move==new_move.min())
    #
    # if new_move[0][0] == 0:
    #     next_move = 'right'
    # elif new_move[0][0] == 1:
    #     next_move = 'left'
    # elif new_move[0][0] == 2:
    #     next_move = 'up'
    # elif new_move[0][0] == 3:
    #     next_move = 'down'
    # # print next_move

    # print 'rightC = %f\t leftC = %f \t upC = %f \t downC = %f'%(right_costs,left_costs,up_costs,down_costs)
    return next_move


# Find all the valid moves
def checkValidMoves(game_board):
    # Generate all game boards after all moves
    right_game_board = right_move_return(game_board)
    left_game_board = left_move_return(game_board)
    up_game_board = up_move_return(game_board)
    down_game_board = down_move_return(game_board)

    # Store true false in an array 0 = right, 1 = left, 2 = up, 3 = down
    isValid = np.ones([4, 1])
    if np.array_equal(np.array(game_board), np.array(right_game_board)):
        isValid[0] = 0
    if np.array_equal(np.array(game_board), np.array(left_game_board)):
        isValid[1] = 0
    if np.array_equal(np.array(game_board), np.array(up_game_board)):
        isValid[2] = 0
    if np.array_equal(np.array(game_board), np.array(down_game_board)):
        isValid[3] = 0

    return isValid


# -------------------------- Function for calculating down move --------------------------
def down_move_return(game_board):
    # Create the 2D array to store the board
    new_game_board = [[0 for x in range(4)] for x in range(4)]
    # new_game_board = np.zeros([4,4],dtype='uint8')

    # Move the rows down
    for col_ind in range(0, 4):

        temp_ind = 3

        for row_ind in range(3, -1, -1):

            if game_board[row_ind][col_ind] != 0:
                new_game_board[temp_ind][col_ind] = game_board[row_ind][col_ind]
                temp_ind = temp_ind - 1

    # Combine like blocks
    for col_ind in range(0, 4):

        row_ind_cond = 3

        while row_ind_cond != 0:

            if new_game_board[row_ind_cond][col_ind] == new_game_board[row_ind_cond - 1][col_ind]:

                new_game_board[row_ind_cond][col_ind] = 2 * new_game_board[row_ind_cond - 1][col_ind]

                for row_ind in range(row_ind_cond - 1, 0, -1):
                    new_game_board[row_ind][col_ind] = new_game_board[row_ind - 1][col_ind]

                new_game_board[0][col_ind] = 0

            row_ind_cond = row_ind_cond - 1

    return new_game_board


# -------------------------- Function for calculating right move --------------------------
def right_move_return(game_board):
    # Create the 2D array to store the board
    new_game_board = [[0 for x in range(4)] for x in range(4)]

    # Move the rows down
    for col_ind in range(0, 4):

        temp_ind = 3

        for row_ind in range(3, -1, -1):

            if game_board[col_ind][row_ind] != 0:
                new_game_board[col_ind][temp_ind] = game_board[col_ind][row_ind]
                temp_ind = temp_ind - 1

    # Combine like blocks
    for col_ind in range(0, 4):

        row_ind_cond = 3

        while row_ind_cond != 0:

            if new_game_board[col_ind][row_ind_cond] == new_game_board[col_ind][row_ind_cond - 1]:

                new_game_board[col_ind][row_ind_cond] = 2 * new_game_board[col_ind][row_ind_cond - 1]

                for row_ind in range(row_ind_cond - 1, 0, -1):
                    new_game_board[col_ind][row_ind] = new_game_board[col_ind][row_ind - 1]

                new_game_board[col_ind][0] = 0

            row_ind_cond = row_ind_cond - 1

    return new_game_board


# -------------------------- Function for calculating up move --------------------------
def up_move_return(game_board):
    # Create the 2D array to store the board
    new_game_board = [[0 for x in range(4)] for x in range(4)]

    # Move the rows down
    for col_ind in range(0, 4):

        temp_ind = 0;

        for row_ind in range(0, 4):

            if game_board[row_ind][col_ind] != 0:
                new_game_board[temp_ind][col_ind] = game_board[row_ind][col_ind]
                temp_ind = temp_ind + 1

    # Combine like blocks
    for col_ind in range(0, 4):

        row_ind_cond = 0

        while row_ind_cond != 3:

            if new_game_board[row_ind_cond][col_ind] == new_game_board[row_ind_cond + 1][col_ind]:

                new_game_board[row_ind_cond][col_ind] = 2 * new_game_board[row_ind_cond + 1][col_ind]

                for row_ind in range(row_ind_cond + 1, 3):
                    new_game_board[row_ind][col_ind] = new_game_board[row_ind + 1][col_ind]

                new_game_board[3][col_ind] = 0

            row_ind_cond = row_ind_cond + 1

    return new_game_board


# -------------------------- Function for calculating left move --------------------------
def left_move_return(game_board):
    # Create the 2D array to store the board
    new_game_board = [[0 for x in range(4)] for x in range(4)]

    # Move the rows down
    for col_ind in range(0, 4):

        temp_ind = 0

        for row_ind in range(0, 4):

            if game_board[col_ind][row_ind] != 0:
                new_game_board[col_ind][temp_ind] = game_board[col_ind][row_ind]
                temp_ind = temp_ind + 1

    # Combine like blocks
    for col_ind in range(0, 4):

        row_ind_cond = 0

        while row_ind_cond != 3:

            if new_game_board[col_ind][row_ind_cond] == new_game_board[col_ind][row_ind_cond + 1]:

                new_game_board[col_ind][row_ind_cond] = 2 * new_game_board[col_ind][row_ind_cond + 1]

                for row_ind in range(row_ind_cond + 1, 3):
                    new_game_board[col_ind][row_ind] = new_game_board[col_ind][row_ind + 1]

                new_game_board[col_ind][3] = 0

            row_ind_cond = row_ind_cond + 1

    return new_game_board
