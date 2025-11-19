import sympy as sp
import numpy as np
import sys
sys.stdout.reconfigure(encoding='utf-8')

BORDER = '-' * 22
SPACE =  ' ' *4

def load_data(file_name):
    with open(file_name, 'r') as file:
        lines = file.readlines()
    data = []
    for line in lines:
        values = line.strip().split()
        row = [float(value) for value in values]
        data.append(row)
    return np.array(data).T


def print_iteration(iteration, beta_symbols, beta_vector, beta_values, total_quality, tolerance=1e-6):
    print(BORDER)
    print(SPACE + f"Iteration {iteration}")

    print("Current Beta Values:")

    for idx, beta_val in enumerate(beta_vector):
        print(f"  β{beta_symbols[idx]} = {beta_val:}")

    print(f"\nTotal Quality (Δ) = {total_quality:}")

    if total_quality < tolerance:
        print("\nConvergence Status: Achieved (Δ < 1e-10)")
        return True
    else:
        print(f"\nConvergence Status: Not Achieved (Δ > 1e-10)")
        return False


def print_result(final_result):
    # Final result display with clear formatting
    print("\n--- Final Approximation ---")
    for param, value in final_result.items():
        print(f"{param}: {value:.6f}")

def compute_derivatives(y_vec, b_vec, b_values):
    derivatives = []
    for y in y_vec:
        for b in b_vec:
            derivative = sp.diff(y, b)
            derivative = derivative.subs(b_values)
            derivatives.append(derivative)

    num_cols = len(b_vec)
    derivative_matrix = [derivatives[i:i + num_cols] for i in range(0, len(derivatives), num_cols)]
    return sp.Matrix(derivative_matrix)

def calculate_u_matrix(a_matrix, b_matrix, u_matrix, h):
    b_array = np.array(b_matrix.tolist())
    k1 = h * (np.dot(a_matrix, u_matrix) + b_array)
    k2 = h * (np.dot(a_matrix, u_matrix + k1 / 2) + b_array)
    k3 = h * (np.dot(a_matrix, u_matrix + k2 / 2) + b_array)
    k4 = h * (np.dot(a_matrix, u_matrix + k3) + b_array)
    return u_matrix + (k1 + 2 * k2 + 2 * k3 + k4) / 6

def calculate_y(a_matrix, y_current, h):
    k1 = h * np.dot(a_matrix, y_current)
    k2 = h * np.dot(a_matrix, y_current + k1 / 2)
    k3 = h * np.dot(a_matrix, y_current + k2 / 2)
    k4 = h * np.dot(a_matrix, y_current + k3)
    return y_current + (k1 + 2 * k2 + 2 * k3 + k4) / 6

def initialize_matrix():
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

def optimize_approximation(y_matrix, params, beta_symbols, beta_values, tolerance, h=0.2):
    a_matrix = initialize_matrix().subs(params)



    beta_vector = np.array([beta_values[beta_symbols[0]], beta_values[beta_symbols[1]], beta_values[beta_symbols[2]]])
    print(f"Initial beta values:")

    for idx, beta_val in enumerate(beta_vector):
        print(f"  β{beta_symbols[idx]} = {beta_val:.6f}")
    print()

    iteration = 0  # To track iterations


    while True:
        iteration += 1
        a_full = np.array(a_matrix.subs(beta_values).tolist())
        u_matrix = np.zeros((6, 3))
        total_quality = 0
        inverse_integral_part = np.zeros((3, 3))
        multiplication_integral_part = np.zeros((1, 3))
        y_approx = y_matrix[0]

        for i in range(len(y_matrix)):
            b_derivative_matrix = compute_derivatives(a_matrix * sp.Matrix(y_approx), beta_symbols, beta_values)

            inverse_integral_part = (inverse_integral_part + np.dot(u_matrix.T, u_matrix)).astype('float64')
            multiplication_integral_part = (multiplication_integral_part + np.dot(u_matrix.T, y_matrix[i] - y_approx)).astype('float64')
            total_quality = total_quality + np.dot((y_matrix[i] - y_approx).T, y_matrix[i] - y_approx)

            u_matrix = calculate_u_matrix(a_full, b_derivative_matrix, u_matrix, h)
            y_approx = calculate_y(a_full, y_approx, h)

        inverse_integral_part = inverse_integral_part * h
        multiplication_integral_part = multiplication_integral_part * h
        total_quality = total_quality * h

        delta_beta = np.dot(np.linalg.inv(inverse_integral_part), multiplication_integral_part.flatten())
        beta_vector = beta_vector + delta_beta

        beta_values = {beta_symbols[0]: beta_vector[0], beta_symbols[1]: beta_vector[1], beta_symbols[2]: beta_vector[2]}

        if print_iteration(iteration,beta_symbols, beta_vector, beta_values, total_quality,tolerance=1e-6):
          return beta_values




def main():
  data = load_data('C:/Users/User/Desktop/University/3course/systemModeling/lab3/y4.txt')

  c1, c2, c3, c4, m1, m2, m3 = sp.symbols('c1 c2 c3 c4 m1 m2 m3')

  initial_values = {m2:21, c3:0.15, m3:11}
  known_params = {c1: 0.14, c2: 0.3, c4: 0.12, m1: 12}
  need_find = [c3, m2, m3]
  epsilon = 1e-10

  final_result = optimize_approximation(data, known_params, need_find, initial_values, epsilon)
  print_result(final_result)

if __name__ == "__main__":
    main()
