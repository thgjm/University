'''
Написати програму, яка розв’язує систему лінійних алгебраїчних рівнянь двома методами:
методом прогонки;
методом Зейделя.

Знайти визначник матриці своїм прямим методом. Знайти число обумовленості.

Необхідні дослідження для методів:
Матрицю і вектор стовпчик правої частини згенерувати самостійно (щоб для розв’язання отриманої системи можна було застосовувати свої методи).
•	Метод Зейделя: достатня умова збіжності (одна з двох, не забудьте вказати в теорії обидві); для першої і останньої ітерації при перевірці умови припинення ітераційного процесу необхідно вивести норму векторів (не забудьте вказати, яку Ви використали);
•	Метод прогонки: достатні умови стійкості; вивести значення а0, b0, c0;
•	Число обумовленості: вивести обернену матрицю, норму матриці початкової, норму оберненої матриці, саме число обумовленості
'''

import numpy as np
import sys
sys.stdout.reconfigure(encoding='utf-8')

epsilon=0.001

matrix = np.array([[1, 1, 0, 0], 
                   [1, 3, 1, 0],
                   [0, 1, 4, 2],
                   [0, 0, 2, 2]])

vector = np.array([1, 2, 5, 3])

#===метод прогонки===
def sufficientStabilityConditions(A): #достатні умови стійкості
    secondCondition = False           #exists i: |ci|>|ai|+|bi|
    n=len(A)-1
    if abs(A[0][0]) < abs(A[0][1]):   #i=0: |c0|>=|b0|
        return False
    if abs(A[0][0])!=abs(A[0][1]): secondCondition = True
    for i in range(1, n):
        if abs(A[i][i]) < abs(A[i][i+1]) + abs(A[i][i-1]):
            return False
        if(abs(A[i][i]) != abs(A[i][i+1]) + abs(A[i][i-1])): secondCondition = True
    if abs(A[n][n]) < abs(A[n][n-1]): #i=n-1: |c(n-1)|>=|a(n-1)|
        return False
    if abs(A[n][n])!=abs(A[n][n-1]): secondCondition = True
    return secondCondition

def tridiagonalMatrixAlgorithm(A, b):
    if sufficientStabilityConditions(A): print("Method is stable.") #перевірка стійкості методу
    else:
        print("Method is not stable.\n")
        return
    b0=A[0][1]
    c0=-A[0][0]
    n=len(A)
    print(f"a0=0; b0={b0}; c0={c0}")
    print("\nFinding racing coefficients: ")  #знаходження прогонкових коефiцiєнтiв
    alpha=[]
    beta=[]
    z=[]
    #прямиц хід
    alpha.append(b0/c0)
    beta.append(-b[0]/c0)
    print(f"alpha{1}={alpha[0]:.2f}")
    print(f"beta{1}={beta[0]:.2f}")
    for i in range(1, n):
        z.append(-A[i][i]-alpha[i-1]*A[i][i-1])
        print(f"z{i}={z[i-1]:.2f}")
        if i==n-1: break
        alpha.append(A[i][i+1]/z[i-1])
        beta.append((-b[i]+A[i][i-1]*beta[i-1])/z[i-1])
        print(f"alpha{i+1}={alpha[i]:.2f}")
        print(f"beta{i+1}={beta[i]:.2f}")
    #зворотній хід
    y=np.zeros(n)
    print("\nReverse")
    y[n-1]=(-b[n-1]+A[n-1][n-2]*beta[n-2])/z[n-2]
    print(f"y{n-1}={y[n-1]:.2f}")
    for i in range(n-2, -1, -1):
        y[i]=alpha[i]*y[i+1]+beta[i]
        print(f"y{i}={y[i]:.2f}")
    #визначник
    detA=-c0
    for i in range(n-1):
        detA*=-z[i]
    return(y, detA)

print("===Tridiagonal Matrix Algorithm===")
y, detMatrix = tridiagonalMatrixAlgorithm(matrix, vector)
print("\nSolution of the system: (" + ", ".join(f"{v:.2f}" for v in y) + ")^T")
print("DetA =", detMatrix)

#===метод Зейделя===
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


def MatrixNorm(A):  #норма для матриця
    max=-sys.maxsize - 1
    for i in range(len(A)):
        sum=0
        for j in range(len(A)):
            sum+=A[i][j]
        if sum>max: max=sum
    return max

def normDifference(x1, x2): #норма різниці для виходу з ітераційного процесу
    n=len(x1)
    x3=np.zeros(n)
    for i in range(n):
        x3[i]=x2[i]-x1[i]
    return np.max(abs(x3)), x3

def SeidelMethod(A, b):
    if sufficientConvergenceCondition(A): print("Seidel's method coincides.")
    else:
        print("Seidel's method does not coincide.")
        return
    det = np.linalg.det(A)
    n=len(A)
    x=np.zeros(n)
    x = x.reshape(1, -1)
    k=0
    x_prev=x[0]
    print(f"x[0]=", np.round(x[0], 4))
    #будуємо ітераційний процес
    while True:
        x_cur=np.zeros(n)
        x = np.vstack((x, x_cur))
        for i in range(n):
            #print("i=", i)
            t1=0
            t2=0
            for j in range(i):
                #print(f"A[{i}][{j}]={A[i][j]}, x[{k+1}][{j}]={x[k+1][j]}")
                t1=t1+A[i][j]*x[k+1][j]
                #print("j=", j, "t1=", t1)
            for j in range(i+1, n):
                #print(f"A[{i}][{j}]={A[i][j]}, x[{k}][{j}]={x[k][j]}")
                t2=t2+A[i][j]*x[k][j]
                #print("j=", j, "t2=", t2)
            #print("b[i]=",b[i])
            #print("A[i][i]=",A[i][i])
            x_cur[i]=(-t1-t2+b[i])/A[i][i]
            #print(f"x_cur[{i}] = ", np.round(x_cur[i], 2))
            x[k+1]=x_cur
        max, x3 = normDifference(x_prev, x_cur)
        print(f"x[{k+1}]=", np.round(x[k+1], 4))
        #print(f"x[{k+1}]=", np.round(x[k+1], 4))
        #print(f"x[{k+1}]-x[{k}] = ", np.round(x3, 4))
        print(f"||x[{k+1}]-x[{k}]|| = {max:.4f}")
        #умова виходу з ітераційного процесу
        if max < epsilon or max == epsilon:
            return x_cur, det
        x_prev=x_cur
        k+=1

print("\n===Seidel Method===")
x, det = SeidelMethod(matrix, vector)
print("\nSolution of the system with precision Ɛ=0.001: (" + ", ".join(f"{v:.2f}" for v in x) + ")^T")
print("DetA =", np.round(det, 2))

#===число обумовленості
def numberOfConditionality(A):
    inversed=np.linalg.inv(A)
    print("A:\n", A)
    print("A^-1:\n", np.round(inversed, 2))
    print("The norm of matrix A: ||A|| = ", MatrixNorm(A))
    print("The norm of matrix A^-1: ||A^-1|| = ", np.round(MatrixNorm(inversed), 2))
    print("Number of Conditionality: ||A||*||A^-1|| = ", np.round(MatrixNorm(A)*MatrixNorm(inversed), 2))

print("\n===Conditionality number calculations===")
numberOfConditionality(matrix)