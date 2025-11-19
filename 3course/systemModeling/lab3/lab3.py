import numpy as np
import sympy as sp
import matplotlib.pyplot as plt

epsilon = 1e-5

# ініціалізація матриці А
def initializeMatrixA():
    c1, c2, c3, c4, m1, m2, m3 = sp.symbols('c1 c2 c3 c4 m1 m2 m3')
    matrix = [
        [0, 1, 0, 0, 0, 0],
        [-(c2 + c1) / m1, 0, c2 / m1, 0, 0, 0],
        [0, 0, 0, 1, 0, 0],
        [c2 / m2, 0, -(c2 + c3) / m2, 0, c3 / m2, 0],
        [0, 0, 0, 0, 0, 1],
        [0, 0, c3 / m3, 0, -(c4 + c3) / m3, 0]
    ]

    return sp.Matrix(matrix)

# виведення результату
def print_result(final_result, I_B):
    print("\n===Results===")
    for param, value in final_result.items():
        print(f"{param}: {value:.6f}")
    print("I(Beta) = ", I_B)


# ∂Ay/∂beta
def calculate_derivatives(Ay, beta, values):
    derivatives = []
    for y in Ay:
        for b in beta:
            derivative = sp.diff(y, b)
            derivative = derivative.subs(values)
            derivatives.append(derivative)

    n = len(beta)
    derivative_matrix = [derivatives[i:i + n]
                         for i in range(0, len(derivatives), n)]
    return sp.Matrix(derivative_matrix)


def calculate_sensitive_U(A, Ay_dbeta, U, delta_t):
    Ay_dbeta = np.array(Ay_dbeta.tolist())
    k1 = delta_t * (np.dot(A, U) + Ay_dbeta)
    k2 = delta_t * (np.dot(A, U + k1 / 2) + Ay_dbeta)
    k3 = delta_t * (np.dot(A, U + k2 / 2) + Ay_dbeta)
    k4 = delta_t * (np.dot(A, U + k3) + Ay_dbeta)
    return U + (k1 + 2 * k2 + 2 * k3 + k4) / 6


def calculate_y(A, y, delta_t):
    k1 = delta_t * np.dot(A, y)
    k2 = delta_t * np.dot(A, y + k1 / 2)
    k3 = delta_t * np.dot(A, y + k2 / 2)
    k4 = delta_t * np.dot(A, y + k3)
    return y + (k1 + 2 * k2 + 2 * k3 + k4) / 6


# функція для основного розрахунку
def Main_Calculation(y_observed, known_params, beta_symbols, beta_values, epsilon, delta_t=0.2):

    A_with_params = initializeMatrixA().subs(known_params) # ініціалізація та підстановка початкових даних

    beta_vector = np.array([beta_values[beta_symbols[0]],
                           beta_values[beta_symbols[1]], beta_values[beta_symbols[2]]])

    # виведення початкового наближення
    print(f"Beta0:")
    for i, value in enumerate(beta_vector):
        print(f"Beta {beta_symbols[i]} = {value}")

    iter = 1

    while True:
        A_with_betas = np.array(A_with_params.subs(beta_values).tolist())

        Matrix_U = np.zeros((6, 3))
        I_B = 0
        integral_inverse = np.zeros((3, 3))
        intergral_multi = np.zeros((1, 3))
        y_t = y_observed[0]

        for i in range(len(y_observed)):
            
            # розрахунок матриці # ∂Ay/∂beta для U
            derivative_b = calculate_derivatives(A_with_params * sp.Matrix(y_t), beta_symbols, beta_values)
            # розрахунок лівого мнонжина для Δbeta
            integral_inverse = (integral_inverse + np.dot(Matrix_U.T, Matrix_U)).astype('float64')
            # розрахунок правого мнонжина для Δbeta
            intergral_multi = (intergral_multi + np.dot(Matrix_U.T, y_observed[i] - y_t)).astype('float64')
            # розрахунок показника якості I(beta)
            I_B = I_B + np.dot((y_observed[i] - y_t).T, y_observed[i] - y_t)

            # знаходження матриці чутливості U
            Matrix_U = calculate_sensitive_U(A_with_betas, derivative_b, Matrix_U, delta_t)
            # знаходження y
            y_t = calculate_y(A_with_betas, y_t, delta_t)

        integral_inverse = integral_inverse * delta_t
        intergral_multi = intergral_multi * delta_t
        I_B = I_B * delta_t

        # знаходження Δbeta та нове beta
        delta_beta = np.dot(np.linalg.inv(integral_inverse), intergral_multi.flatten())
        beta_vector = beta_vector + delta_beta

        beta_values = {beta_symbols[0]: beta_vector[0], beta_symbols[1]: beta_vector[1], beta_symbols[2]: beta_vector[2]}

        # виведення значень поточних параметрів beta показника якості та 
        print(f"\n===Iteration {iter}===")

        print("Current beta values:")

        for i, value in enumerate(beta_vector):
            print(f"Beta {beta_symbols[i]} = {value:}")

        print(f"\nQuality indicator I(Beta) = {I_B:}")

        # перевірка умов виходу
        if I_B < epsilon:
            print("\nThe algorithm is complete: I(Beta) < ", epsilon)
            return beta_values, I_B
        else:
            print("\nThe algorithm is not complete: I(Beta) >= ", epsilon)

        iter = iter + 1

def main():
    # зчитування даних із файлу
    y_observed  = np.loadtxt("C:/Users/User/Desktop/University/3course/systemModeling/lab3/y4.txt")

    # ініціалізація відомих значень та тих, що потрібно знайти
    c1, c2, c3, c4, m1, m2, m3 = sp.symbols('c1 c2 c3 c4 m1 m2 m3')
    
    beta0 = {c3: 0.15, m2: 21, m3: 11}
    known_values = {c1: 0.14, c2: 0.3, c4: 0.12, m1: 12}
    to_find = [c3, m2, m3]

    # запуск основного алгоритму та вивід результату на екран
    final_result, I_B = Main_Calculation(y_observed.T, known_values, to_find, beta0, epsilon)
    print_result(final_result, I_B)

    # побудова графіку

    A_final = initializeMatrixA().subs(known_values).subs(final_result)
    A_final = np.array(A_final.tolist(), dtype='float64')

    delta_t = 0.2
    steps = y_observed.shape[1]
    y_sim = np.zeros((steps, 6))

    y_sim[0, :] = y_observed[:, 0]

    t_vec = np.arange(steps) * delta_t

    for i in range(steps - 1):
        y_sim[i+1] = calculate_y(A_final, y_sim[i], delta_t)

    plt.figure(figsize=(10, 9))

    coord_indices = [0, 2, 4]
    labels = ["y1", "y2", "y3"]

    for k in range(3):
        idx = coord_indices[k]
        plt.subplot(3, 1, k + 1)
        plt.plot(t_vec, y_observed[idx], 'b', label=f"Observed {labels[k]}")
        plt.plot(t_vec, y_sim[:, idx], 'r--', label=f"Simulated {labels[k]}")
        plt.xlabel("t")
        plt.ylabel(labels[k])
        plt.legend()
        plt.grid(True)

    plt.tight_layout()
    plt.suptitle("Observed vs Simulated Model Response (Identified Parameters)")
    plt.subplots_adjust(top=0.93)
    plt.show()

main()