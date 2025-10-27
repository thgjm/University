'''''
1. Написати програму, яка знаходить максимальне та мінімальне власне значення матриці степеневим методом.

2. Написати програму, яка знаходить всі власні значення та відповідні власні вектори матриці методом обертань (Якобі).
Для тестування взяти вашу матрицю з лабораторної роботи  №2.
'''''

import numpy as np
import math
import sys
sys.stdout.reconfigure(encoding='utf-8')

epsilon = 0.0001

A = np.array([
    [1, 1, 0, 0],
    [1, 3, 1, 0],
    [0, 1, 4, 2],
    [0, 0, 2, 2]
])

x0 = np.array([1, 1, 1, 1])

def leading_principal_minors(A):    #перевірка чи є матриця додатновизначеною
    A = np.array(A, dtype=float)
    n = A.shape[0]
    minors = []
    for k in range(1, n + 1):
        submatrix = A[:k, :k]
        det = np.linalg.det(submatrix)
        minors.append(det)
    return minors

def sufficientConvergenceCondition(A):  #перевірка на збіжність за 2 умовою
    transposed=A.T
    if np.array_equal(A, transposed):
        print("A=A^T")
    else:
        print("A!=A^T")
        return False
    
    minors = leading_principal_minors(A)
    for i, det in enumerate(minors, 1):
        print(f"det(A[{i}x{i}]) = {det:.2f}")
    if all(det > 0 for det in minors):
        print("=> A > 0")
    else:
        print("=> A <=0")
        return False
    return True

# j за замовчування буде 0
def powerMethod(A, x0):
    k = 2
    x_prev = x0
    x_cur = A @ x_prev
    lambda_cur = x_cur[0] / x_prev[0]
    # print("Iteration = ", k)
    # print("x[1] = ", x_cur)
    # print("Lambda[1] = ", round(lambda_cur, 5))
    while True:
        # print("Iteration = ", k)
        norm = np.max(x_cur)
        e_prev = np.zeros(len(x_cur))
        for i in range(len(e_prev)):
            e_prev[i] = x_cur[i]/norm
        # print(f"e[{k-1}] = ", e_cur)
        x_prev = e_prev
        x_cur = A @ x_prev
        # print(f"x[{k}] = ", x_cur)
        lambda_prev = lambda_cur
        lambda_cur = x_cur[0] / x_prev[0]
        # print(f"Lambda[{k}] = ", round(lambda_cur, 5) )
        if abs(lambda_cur - lambda_prev) <= epsilon:
            return lambda_cur
        k += 1


def findMinLambda(A, x0):
    print("\nChecking conditions for finding min lambda:")
    if sufficientConvergenceCondition(A) == False:
        print("Can't find min lambda!")
        return False
    B = np.zeros((len(A), len(A)))
    lambda_maxA = powerMethod(A, x0)
    maxE = np.identity(len(A))
    maxE = lambda_maxA * maxE
    B = maxE - A
    lambda_maxB = powerMethod(B, x0)
    lambda_min = lambda_maxA-lambda_maxB
    return lambda_min


def findMinMax(A, x0):
    lambda_max = powerMethod(A, x0)
    lambda_min = findMinLambda(A, x0)
    print("\nMin lambda = ", round(lambda_min, 5))
    print("Max lambda = ", round(lambda_max, 5))
    return lambda_min, lambda_max


def findPositionOfMax(A):
    max = abs(A[0][1])
    max_i = 0
    max_j = 1
    for i in range(len(A)-1):
        for j in range(i+1, len(A)):
            if abs(A[i][j]) > max:
                max = abs(A[i][j])
                max_i = i
                max_j = j
    return max_i, max_j


def forStopConditions(A):
    sum = 0
    for i in range(len(A)):
        for j in range(len(A)):
            if i != j:
                sum += A[i][j]*A[i][j]
    return sum


def findAllLambdas(A):
    print("==Finding all lambdas==\n")
    A_cur = A
    k = 1
    transposed = A.T
    if np.array_equal(A, transposed):
        print("A=A^T => Can continue.\n")
    else:
        print("A!=A^T => Can't continue finding lambdas.\n")
        return
    U_array = []
    print("A[0]: ")
    print(np.round(A_cur, 3), "\n")
    while True:
        print("Iteration = ", k)
        i_cur, j_cur = findPositionOfMax(A_cur)
        phi_cur = 0
        Atemp = round(A_cur[i_cur][i_cur] - A_cur[j_cur][j_cur], 4)
        if Atemp == 0:
            phi_cur = math.copysign(1, A_cur[i_cur][j_cur])*math.pi/4
        else:
            Atemp = round(2*A_cur[i_cur][j_cur] /
                          (A_cur[i_cur][i_cur] - A_cur[j_cur][j_cur]), 4)
            phi_cur = 1/2 * math.atan(Atemp)
        # print(phi_cur)
        U_cur = np.identity(len(A))
        U_cur[i_cur][i_cur] = math.cos(phi_cur)
        U_cur[j_cur][j_cur] = math.cos(phi_cur)
        U_cur[j_cur][i_cur] = math.sin(phi_cur)
        U_cur[i_cur][j_cur] = -math.sin(phi_cur)
        U_array.append(U_cur)
        # print(np.round(U_cur, 3))
        A_next = (U_cur.T @ A_cur) @ U_cur
        # print(i_cur+1, j_cur+1)
        print(f"A[{k}]:")
        print(np.round(A_next, 3))
        print("\n")
        sum = round(forStopConditions(A_next), 5)
        if sum <= epsilon:
            print("Number of iterations:", k)
            return A_next, U_array
        A_cur = A_next
        k += 1


def getLambdasAndVectors(A):
    matrix, U_array = findAllLambdas(A)
    lambdas = np.zeros(len(matrix))
    vectors = np.identity(len(A))

    for i in range(len(U_array)):
        vectors = vectors @ U_array[i]

    for i in range(len(matrix)):
        lambdas[i] = matrix[i][i]
        print(
            f"λ{i+1} = {lambdas[i]:.4f}. This Eigenvalue corresponds to the eigenvector = {np.round(vectors.T[i], 4)}")

    return lambdas, vectors


lambdas, vectors = getLambdasAndVectors(A)

lambda_min, lambda_max = findMinMax(A, x0)
