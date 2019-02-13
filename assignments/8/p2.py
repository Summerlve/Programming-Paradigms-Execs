#-*- coding:utf-8 -*-
from random import choice, seed

def getMaxPoints(gameBoard):
    # gameBoard is N x N list
    maxPoint = 0

    curmax = getMaxNumAndCoordFromGameBoard(gameBoard)




    return maxPoint
    
def getMaxNumAndCoordFromGameBoard(gameBoard):
    # gameBoard is N x N list
    N = len(gameBoard)
    maxNum = 0
    coordx = coordy = -1

    for row in range(0, N):
        for col in range(0, N):
            if gameBoard[row][col] > maxNum:
                maxNum = gameBoard[row][col]
                coordx, coordy = row, col
    
    return {
        "maxNum": maxNum,
        "coord": (coordx, coordy)
    }

def getNeighbors(gameBoard, coordX):
    neighbors = []
    N = len(gameBoard)

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
