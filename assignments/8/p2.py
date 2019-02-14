#-*- coding:utf-8 -*-
from random import choice, seed

def getMaxPoints(gameBoard, removedSquares, curPoints):
    # gameBoard is a N x N list

    # list 'removedSquare' records removed points' coord infomation 
    # such as: [(1, 2), (3, 4)]
    # every square is available in initial moment

    # curPoints: current points sum

    N = len(gameBoard)
    curMaxSquares = getCurMaxSquaresFromGameBoard(gameBoard, removedSquares)
    pointsCollection = []

    for square in curMaxSquares:
        if not square in removedSquares: removedSquares.append(square)            
        for neighbor in getNeighbors(N, square):
            if not neighbor in removedSquares:
                removedSquares.append(neighbor)
        row, col = square
        curPoints += gameBoard[row][col] 
        restPoints = getMaxPoints(gameBoard, list(removedSquares), 0)
        curPoints += restPoints
        pointsCollection.append(curPoints)

    if pointsCollection: return max(pointsCollection)
    else: return 0

def getCurMaxSquaresFromGameBoard(gameBoard, removedSquares):
    # curMaxSquares is a list
    # curMaxSquares and tempMaxSquares: [(0, 0), (1, 0)]
    # gameBoard is N x N list
    N = len(gameBoard)
    maxPoint = 0
    tempMaxSquares = []
    curMaxSquares = [] 

    for row in range(0, N):
        for col in range(0, N):
            if gameBoard[row][col] >= maxPoint and not (row, col) in removedSquares:
                maxPoint = gameBoard[row][col]
                tempMaxSquares.append((row, col))
                
    for i in range(0, len(tempMaxSquares)):
        row, col = tempMaxSquares[i]
        if gameBoard[row][col] == maxPoint :
            curMaxSquares.append(tempMaxSquares[i])

    return curMaxSquares 

def getNeighbors(N, square):
    # square is a tuple: (x, y)
    # N is NxN's N
    # neighbors: [(0, 0), (0, 1),...]
    neighbors = []
    x, y = square

    if x - 1 >= 0:
        if y - 1 >=0: neighbors.append((x - 1, y - 1))
        neighbors.append((x - 1, y))
        if y + 1 <= N - 1: neighbors.append((x - 1, y + 1))

    if y - 1 >= 0:
        neighbors.append((x, y - 1)) 
    if y + 1 <= N - 1:
        neighbors.append((x, y + 1))

    if x + 1 <= N - 1:
        if y - 1 >= 0: neighbors.append((x + 1, y - 1))
        neighbors.append((x + 1, y))
        if y + 1 <= N - 1: neighbors.append((x + 1, y + 1))

    return neighbors

def isNeighbor(coordA, coordB):
    """
    coord is a tuple
    coordA: (xa, ya)
    coordB: (xb, yb)
    """
    isNb = False
    xa, ya = coordA
    xb, yb = coordB 

    if xb == xa:
        if yb == ya - 1: isNb = True
        if yb == ya + 1: isNb = True
    if xb == xa + 1:
        if yb == ya: isNb = True
        if yb == ya - 1: isNb = True
        if yb == ya + 1: isNb = True
    if xb == xa - 1:
        if yb == ya: isNb = True
        if yb == ya - 1: isNb = True
        if yb == ya + 1: isNb = True

    return isNb

def getInput():
    pass

def genGameBoardFromInput():
    gameBoard = []
    return gameBoard

def genGameBoardInRandom():
    N = choice(range(3, 16)) # N belongs to [3, 15]
    gameBoard = []

    for i in range(0, N):
        gameBoard.append([])
        for j in range(0, N):
            gameBoard[i].append(choice(range(10, 100))) # square belongs to [10, 99]

    return gameBoard
    
seed()
# gameBoard = genGameBoardInRandom()
gameBoard_1 = [
    [71, 24, 95, 56, 54],
    [85, 50, 74, 94, 28],
    [92, 96, 23, 71, 10],
    [23, 61, 31, 30, 46],
    [64, 33, 32, 95, 89]
]
gameBoard_2 = [
    [10, 10, 10],
    [10, 10, 10],
    [10, 10, 10]
]
print getMaxPoints(gameBoard_1, [], 0)