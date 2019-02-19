#-*- coding:utf-8 -*-
from random import choice, seed

# --- force algo start ---
# force algo iterate every path and figure out points, and get the max points
def forceAlgoReRe(gameBoard, restSquares = None, startSquares = None):
    N = len(gameBoard)
    isStart = False

    if restSquares is None:
        restSquares = []
        for row in range(0, N):
            for col in range(0, N):
                restSquares.append((row, col))
    if startSquares is None:
        startSquares = []
        isStart = True
        for row in range(0, N):
            for col in range(0, N):
                if col <= row:
                    startSquares.append((row, col))

    pointsCollection = []
    iterationSquares = []

    if isStart:
        iterationSquares = startSquares
    else:
        iterationSquares = restSquares

    for square in iterationSquares:
        restSquaresInSubPath = list(restSquares)

        restSquaresInSubPath.remove(square)
        for neighbor in getNeighbors(N, square):
           if neighbor in restSquaresInSubPath: restSquaresInSubPath.remove(neighbor)
            
        row, col = square 
        points = 0
        restPoints = 0

        if len(restSquaresInSubPath) == 0:
            restPoints = 0      
        elif len(restSquaresInSubPath) == 1:
            lastOne = restSquaresInSubPath[0]
            # restSquaresInSubPath.remove(lastOne)
            restPoints = gameBoard[lastOne[0]][lastOne[1]]
        elif len(restSquaresInSubPath) == 2:
            squareA = restSquaresInSubPath[0]
            squareB = restSquaresInSubPath[1]
            pointA = gameBoard[squareA[0]][squareA[1]]
            pointB = gameBoard[squareB[0]][squareB[1]]

            isNb = isNeighbor(squareA, squareB)
            if isNb:
                restPoints = max(pointA, pointB)  
            else:
                restPoints = pointA + pointB

            # restSquaresInSubPath.remove(squareA)
            # restSquaresInSubPath.remove(squareB)
        elif len(restSquaresInSubPath) == 3:
            squareA = restSquaresInSubPath[0]
            squareB = restSquaresInSubPath[1]
            squareC = restSquaresInSubPath[2]
            pointA = gameBoard[squareA[0]][squareA[1]]
            pointB = gameBoard[squareB[0]][squareB[1]]
            pointC = gameBoard[squareC[0]][squareC[1]]
            
            isAB = isNeighbor(squareA, squareB)                
            isAC = isNeighbor(squareA, squareC)                
            isBC = isNeighbor(squareB, squareC)                

            if (not isAB) and (not isAC) and (not isBC):
                restPoints = pointA + pointB + pointC
            if isAB and (not isAC) and (not isBC):
                restPoints = max(pointA, pointB) + pointC
            if isAC and (not isAB) and (not isBC):
                restPoints = max(pointA, pointC) + pointB 
            if isBC and (not isAB) and (not isAC):
                restPoints = max(pointB, pointC) + pointA
            if isAC and isBC and (not isAB):
                restPoints = max(pointA + pointB, pointC)
            if isAB and isBC and (not isAC):
                restPoints = max(pointA + pointC, pointB)
            if isAB and isAC and (not isBC):
                restPoints = max(pointB + pointC, pointA)
            if isAB and isBC and isAC:
                restPoints = max(pointA, pointB, pointC)
        else:
            restPoints = forceAlgoReRe(gameBoard, list(restSquaresInSubPath), [])

        points += gameBoard[row][col]
        points += restPoints
        pointsCollection.append(points)

    if pointsCollection: return max(pointsCollection)
    else: return 0

def getRestSquares(gameBoard, removedSquares):
    rest = []
    for square in gameBoard:
        if not square in removedSquares:
            rest.append(square) 
    return rest

def forceAlgoRe(gameBoard, removedSquares = None):
    if removedSquares is None: removedSquares = []
    N = len(gameBoard)
    pointsCollection = []

    for row in range(0, N):
        for col in range(0, N):
            square = (row, col)
            points = 0
            removedSquaresInSubPath = list(removedSquares)

            if square in removedSquaresInSubPath: continue
            removedSquaresInSubPath.append(square)
            for neighbor in getNeighbors(N, square):
                if not neighbor in removedSquaresInSubPath:
                    removedSquaresInSubPath.append(neighbor)

            points += gameBoard[row][col]
            restPoints = forceAlgoRe(gameBoard, list(removedSquaresInSubPath))
            points += restPoints
            pointsCollection.append(points)

    if pointsCollection: return max(pointsCollection)
    else: return 0 

def forceAlgo(gameBoard):
    N = len(gameBoard)
    pointsCollection = []

    for row in range(0, N):
        for col in range(0, N):
            startSquare = (row, col)
            points = 0
            removedSquares = []

            points += gameBoard[row][col]
            removedSquares.append(startSquare)
            for neighbor in getNeighbors(N, startSquare):
                removedSquares.append(neighbor)
            restPoints = figureOutRestPoints(gameBoard, removedSquares)
            points += restPoints
            print "startSquare: ", startSquare, "max point is: ", points
            pointsCollection.append(points)
            
    return max(pointsCollection)

def figureOutRestPoints(gameBoard, removedSquares):
    N = len(gameBoard)
    pointsCollection = [] 

    for row in range(0, N):
        for col in range(0, N):
            removedSquaresInSubPath = list(removedSquares)
            points = 0
            curSquare = (row, col)
            if curSquare in removedSquaresInSubPath: continue
            removedSquaresInSubPath.append(curSquare) 
            for neighbor in getNeighbors(N, curSquare):
                if not neighbor in removedSquaresInSubPath:
                    removedSquaresInSubPath.append(neighbor)
            curPoints = gameBoard[row][col]
            restPoints = figureOutRestPoints(gameBoard, list(removedSquaresInSubPath))
            points = curPoints + restPoints
            # print "in figureOutRestPoints, points: ", points
            pointsCollection.append(points)

    if pointsCollection: return max(pointsCollection)
    else: return 0 
# --- force algo end ---

# --- choice max square everytime in available squares start ---
# policy: get max point everytime from available squares, if it has serval max point square onetime, it will find the better way to get higher points
def choiceMaxEveryTimeAlgo(gameBoard):
    return getMaxPoints(gameBoard, [])
    
def getMaxPoints(gameBoard, removedSquares):
    # gameBoard is a N x N list

    # list 'removedSquare' records removed points' coord infomation 
    # such as: [(1, 2), (3, 4)]
    # every square is available in initial moment

    N = len(gameBoard)
    curMaxSquares = getCurMaxSquaresFromGameBoard(gameBoard, removedSquares)
    pointsCollection = []

    for square in curMaxSquares:
        if not square in removedSquares: removedSquares.append(square)            
        for neighbor in getNeighbors(N, square):
            if not neighbor in removedSquares:
                removedSquares.append(neighbor)
        row, col = square
        curPoints = gameBoard[row][col] 
        restPoints = getMaxPoints(gameBoard, list(removedSquares) )
        pointsCollection.append(curPoints + restPoints)

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
# --- choice max square everytime in available squares end ---

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

def genGameBoardFromRandom():
    seed()
    N = choice(range(3, 16)) # N belongs to [3, 15]
    gameBoard = []

    for i in range(0, N):
        gameBoard.append([])
        for j in range(0, N):
            gameBoard[i].append(choice(range(10, 100))) # square belongs to [10, 99]

    return gameBoard
    
gameBoardTest_1 = [
    [1, 1, 1],
    [1, 1, 1],
    [1, 1, 1]
]
gameBoardTest_2 = [
    [71, 24, 95, 56, 54],
    [85, 50, 74, 94, 28],
    [92, 96, 23, 71, 10],
    [23, 61, 31, 30, 46],
    [64, 33, 32, 95, 89]
]
gameBoardTest_3 = [
    [78, 78, 11, 55, 20, 11],
    [98, 54, 81, 43, 39, 97],
    [12, 15, 79, 99, 58, 10],
    [13, 79, 83, 65, 34, 17],
    [85, 59, 61, 12, 58, 97],
    [40, 63, 97, 85, 66, 90]
]
gameBoardTest_4 = [
    [33, 49, 78, 79, 30, 16, 34, 88, 54, 39, 26],
    [80, 21, 32, 71, 89, 63, 39, 52, 90, 14, 89],
    [49, 66, 33, 19, 45, 61, 31, 29, 84, 98, 58],
    [36, 53, 35, 33, 88, 90, 19, 23, 76, 23, 76],
    [77, 27, 25, 42, 70, 36, 35, 91, 17, 79, 43],
    [33, 85, 33, 59, 47, 46, 63, 75, 98, 96, 55],
    [75, 88, 10, 57, 85, 71, 34, 10, 59, 84, 45],
    [29, 34, 43, 46, 75, 28, 47, 63, 48, 16, 19],
    [62, 57, 91, 85, 89, 70, 80, 30, 19, 38, 14],
    [61, 35, 36, 20, 38, 18, 89, 64, 63, 88, 83],
    [45, 46, 89, 53, 83, 59, 48, 45, 87, 98, 21],
]
gameBoardTest_5 = [
    [15, 95, 24, 35, 79, 35, 55, 66, 91, 95, 86, 87],
    [94, 15, 84, 42, 88, 83, 64, 50, 22, 99, 13, 32],
    [85, 12, 43, 39, 41, 23, 35, 97, 54, 98, 18, 85],
    [84, 61, 77, 96, 49, 38, 75, 95, 16, 71, 22, 14],
    [18, 72, 97, 94, 43, 18, 59, 78, 33, 80, 68, 59],
    [26, 94, 78, 87, 78, 92, 59, 83, 26, 88, 91, 91],
    [34, 84, 53, 98, 83, 49, 60, 11, 55, 17, 51, 75],
    [29, 80, 14, 79, 15, 18, 94, 39, 69, 24, 93, 41],
    [66, 64, 88, 82, 21, 56, 16, 41, 57, 74, 51, 79],
    [49, 15, 59, 21, 37, 27, 78, 41, 38, 82, 19, 62],
    [54, 91, 47, 29, 38, 67, 52, 92, 81, 99, 11, 27],
    [31, 62, 32, 97, 42, 93, 43, 79, 88, 44, 54, 48]
]

# print "Algo: choice max square everytime in available squares:"
# print choiceMaxEveryTimeAlgo(gameBoardTest_1)
# print choiceMaxEveryTimeAlgo(gameBoardTest_2)
# print choiceMaxEveryTimeAlgo(gameBoardTest_3)
# print choiceMaxEveryTimeAlgo(gameBoardTest_4)
# print choiceMaxEveryTimeAlgo(gameBoardTest_5)

# print "Algo: get every path and find out max points:"
# print "This algo is optimal solution"
# print forceAlgo(gameBoardTest_1)
# print forceAlgo(gameBoardTest_2)
# print forceAlgo(gameBoardTest_3)
# print forceAlgo(gameBoardTest_4)
# print forceAlgo(gameBoardTest_5)

# print forceAlgoRe(gameBoardTest_1)
# print forceAlgoRe(gameBoardTest_2)
# print forceAlgoRe(gameBoardTest_3)

# print forceAlgoReRe(gameBoardTest_1)
# print forceAlgoReRe(gameBoardTest_2)
print forceAlgoReRe(gameBoardTest_3)