import matplotlib.pyplot as plt
import numpy as np
import imageio.v3 as iio

# Зчитування зображень і перетворення у float64
X = np.array(iio.imread("C:/Users/User/Desktop/University/3course/systemModeling/lab2/x1.bmp"), dtype=np.float64)
Y = np.array(iio.imread("C:/Users/User/Desktop/University/3course/systemModeling/lab2/y4.bmp"), dtype=np.float64)

# Виведення початкового зображення

plt.figure()
plt.imshow(X.astype(np.uint8), cmap='gray')
plt.title("Зображення X")

# Виведення зображення, отриманого після перетворення X
plt.figure()
plt.imshow(Y.astype(np.uint8), cmap='gray')
plt.title("Зображення Y")

plt.show()


X = np.append(X, np.full((1, X.shape[1]), 1), axis = 0)

m = X.shape[0]
n = X.shape[1]
p = Y.shape[0]

#print(X)
#print(Y)

# Формула Гревіля

def Z(A, A_Pseudoinverse):
    return np.eye(A.shape[0]) - A_Pseudoinverse @ A

def GrevilleAlgorithm(A):
    A = np.asarray(A, dtype=float)
    m, n = A.shape

    # Перший рядок
    a1 = A[0:1].T  # вектор-стовпець
    if np.linalg.norm(a1) != 0:
        A_pinv = a1 / (a1.T @ a1)
    else:
        A_pinv = np.zeros((n, 1))

    # Додаємо рядки один за одним
    for i in range(1, m):
        ai = A[i:i+1].T

        Z = np.eye(n) - A_pinv @ A[:i]  # Проєктор Z(A)
        denominator = ai.T @ Z @ ai

        if np.abs(denominator) > 1e-12:
            # Випадок 1: a^T Z(A) a > 0
            vi = (Z @ ai) / denominator
            A_pinv = np.hstack([A_pinv - vi @ (ai.T @ A_pinv), vi])
        else:
            # Випадок 2: a^T Z(A) a = 0
            R = A[:i].T @ A_pinv
            vi = (R @ ai) / (1 + ai.T @ R @ ai)
            A_pinv = np.hstack([A_pinv - vi @ (ai.T @ A_pinv), vi])

    return A_pinv


X_pseudo = GrevilleAlgorithm(X)
V = np.full((p, m), 0)
Z_X = np.eye(m) - X @ X_pseudo
print(Z_X.shape[0])
print(Z_X.shape[1])
A = Y @ X_pseudo + V @ Z_X

Y_new = A @ X

plt.figure()
plt.imshow(Y_new.astype(np.uint8), cmap='gray')
plt.title("Зображення Y_new")

plt.show()