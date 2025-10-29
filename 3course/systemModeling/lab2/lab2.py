import matplotlib.pyplot as plt
import numpy as np
import imageio.v3 as iio

# Зчитування зображень
X = np.array(iio.imread("C:/Users/User/Desktop/University/3course/systemModeling/lab2/x1.bmp"), dtype=np.float64)
Y = np.array(iio.imread("C:/Users/User/Desktop/University/3course/systemModeling/lab2/y4.bmp"), dtype=np.float64)

# Виведення вхідного зображення
plt.figure()
plt.imshow(X.astype(np.uint8), cmap='gray')
plt.title("Зображення X")

# Виведення вихідного зображення
plt.figure()
plt.imshow(Y.astype(np.uint8), cmap='gray')
plt.title("Зображення Y")

# Додавання рядка одинись до матриці Х
X = np.append(X, np.full((1, X.shape[1]), 1), axis=0)

# Знаходження розмірів матриць (X - m x n; Y - p x n)
m = X.shape[0]
n = X.shape[1]
p = Y.shape[0]

# Функція для перевірки правильності знаходження псевдооберненої матриці за теоремою
def characteristicPropertyOfPsinv(A, A_psinv):
    m1, n1 = A.shape
    m2, n2 = A_psinv.shape
    # A - m x n and A+ - n x m
    if m1 != n2 and n1 != m2:
        return False
    # A*A+*A = A
    if not np.allclose(A @ A_psinv @ A, A):
        return False
    # A+*A*A+ = A+
    if not np.allclose(A_psinv @ A @ A_psinv, A_psinv):
        return False
    # A*A+ - симетрична і має розмір m x m
    A_Apsinv = A @ A_psinv
    if not np.allclose(A_Apsinv, A_Apsinv.T) and A_Apsinv.shape[0] != m1 and A_Apsinv.shape[1] != m1:
        return False
    # A+*A - симетрична і має розмір n x n
    Apsinv_A = A_psinv @ A
    if not np.allclose(Apsinv_A, Apsinv_A.T) and A_Apsinv.shape[0] != n1 and A_Apsinv.shape[1] != n1:
        return False
    return True

# Формула Гревіля
def GrevilleAlgorithm(A):
    # Перший рядок
    a1 = A[0:1].T  # вектор-стовпець

    d = a1.T @ a1
    if d != 0:
        A_pinv = a1 / d
    else:
        A_pinv = np.zeros((n, 1))

    A_i = a1.T
    # Додаємо рядки один за одним
    for i in range(1, m):
        ai = A[i:i+1].T
        Z = np.identity(n) - A_pinv @ A_i  # Проєктор Z(A)
        A_i = np.vstack([A_i, ai.T])
        denominator = ai.T @ Z @ ai
        if denominator > 0:
            # Випадок 1: a^T*Z(A)*a > 0
            vi = (Z @ ai) / denominator
            A_pinv = np.hstack(
                [A_pinv - (Z @ ai @ ai.T @ A_pinv) / denominator, vi])
        else:
            # Випадок 2: a^T*Z(A)*a = 0
            R = A_pinv.T @ A_pinv
            denominator = 1 + ai.T @ R @ ai
            vi = (R @ ai) / denominator
            A_pinv = np.hstack(
                [A_pinv - (R @ ai @ ai.T @ A_pinv) / denominator, vi])
    return A_pinv


X_Gr_psinv = GrevilleAlgorithm(X)

if characteristicPropertyOfPsinv(X, X_Gr_psinv):
    print("X_Gr_psinv is correct.")

V = np.zeros((p, m))
Z_X = np.eye(m) - X @ X_Gr_psinv
A = Y @ X_Gr_psinv + V @ Z_X.T

Y_GrNew = A @ X
Y_GrNew = np.clip(Y_GrNew, 0, 255)
plt.figure()
plt.imshow(Y_GrNew.astype(np.uint8), cmap='gray')
plt.title("Зображення Y за формулою Грівеля.")

# Алгоритм, заснований на означеннi Мура-Пенроуза.
def MoorePenroseInverse(A):
    m, n = A.shape
    epsilon = 0.00000001
    delta = 10
    # Нульова ітерація
    Apinv_prev = A.T @ np.linalg.inv(A @ A.T +
                                     (delta * delta) * np.identity(m))
    while True:
        # Початок ітераційного процесу
        delta /= 2
        Apinv_cur = A.T @ np.linalg.inv(A @
                                        A.T + (delta * delta) * np.identity(m))
        # Умови виходу
        if np.linalg.matrix_norm(Apinv_cur - Apinv_prev) < epsilon:
            return Apinv_cur
        Apinv_prev = Apinv_cur

X_MP_psinv = MoorePenroseInverse(X)

if characteristicPropertyOfPsinv(X, X_MP_psinv):
    print("X_MP_psinv is correct.")

V = np.zeros((p, m))
Z_X = np.eye(m) - X @ X_MP_psinv
A = Y @ X_MP_psinv + V @ Z_X.T

Y_MPnew = A @ X
Y_MPnew = np.clip(Y_MPnew, 0, 255)
plt.figure()
plt.imshow(Y_MPnew.astype(np.uint8), cmap='gray')
plt.title("Зображення Y за Мура-Пенроуза.")

plt.show()