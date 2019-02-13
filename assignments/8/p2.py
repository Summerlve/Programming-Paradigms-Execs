#-*- coding:utf-8 -*-
from random import choice, seed

def getMaxPoints(gameBoard):
    # gameBoard is N x N list
    maxPoint = 0
    N = len(gameBoard)

    # list 'removedPoint' records removed point's coord infomation 
    # [(1, 2), (3, 4)]
    # every square is available in initial moment
    removedPoint = []
    curmax = getCurMaxFromGameBoard(gameBoard, removedPoint)




    return maxPoint
    
def getCurMaxFromGameBoard(gameBoard, removedPoint):
    # gameBoard is N x N list
    N = len(gameBoard)
    maxNum = 0
    l = []
    curmax = [] 

    for row in range(0, N):
        for col in range(0, N):
            if gameBoard[row][col] >= maxNum:
                maxNum = gameBoard[row][col]
                l.append({
                    "maxNum": maxNum,
                    "coord": (row, col)
                })

    for i in range(0, len(l)):
        if l[i]["maxNum"] == maxNum and not (l[i]["coord"]) in removedPoint:
            curmax.append(l[i])

    return curmax 

def getNeighbors(N, coordValue):
    # coordValue is a tuple: (x, y)
    # N is NxN's N
    neighbors = []
    x, y = coordValue

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

def genRandomGameBoard():
    N = choice(range(3, 16)) # N belongs to [3, 15]
    gameBoard = []

    for i in range(0, N):
        gameBoard.append([])
        for j in range(0, N):
            gameBoard[i].append(choice(range(10, 100))) # square belongs to [10, 99]

    return gameBoard
    
seed()
gameBoard = genRandomGameBoard()
