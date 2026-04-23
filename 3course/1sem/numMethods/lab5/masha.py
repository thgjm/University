import numpy as np
import matplotlib.pyplot as plt
from dataclasses import dataclass

from lab4 import chebyshev_nodes, lagrange_polynomial


# -----------------------------------------------------------------------------
#  Вихідна функція
# -----------------------------------------------------------------------------
def f(x: np.ndarray | float) -> np.ndarray | float:
    """Задана функція з лабораторної роботи."""
    return x**4 - 5.74 * x**3 + 8.18 * x - 3.48 * np.cos(x)


# -----------------------------------------------------------------------------
#  Структура для зберігання коефіцієнтів сплайна
# -----------------------------------------------------------------------------
@dataclass
class NaturalCubicSpline:
    x: np.ndarray  # вузли x_i
    a: np.ndarray  # коефіцієнти a_i = f(x_i)
    b: np.ndarray  # коефіцієнти при (x - x_i)
    c: np.ndarray  # значення другої похідної в вузлах (s''(x_i))
    d: np.ndarray  # коефіцієнти при (x - x_i)^3 / 6


# -----------------------------------------------------------------------------
#  Побудова природного кубічного сплайна
# -----------------------------------------------------------------------------
def build_natural_cubic_spline(x_nodes: np.ndarray,
                               y_nodes: np.ndarray) -> NaturalCubicSpline:
    """
    Будує природний кубічний сплайн для табличної функції (x_i, y_i).

    Теорія (з методички):
        s_i(x) = a_i + b_i (x - x_i) + c_i/2 (x - x_i)^2 + d_i/6 (x - x_i)^3,
        де c_i визначаються з тридіагональної системи:
            h_i * c_{i-1} + 2(h_i + h_{i+1}) c_i + h_{i+1} * c_{i+1} =
                6 * ((f_{i+1} - f_i)/h_{i+1} - (f_i - f_{i-1})/h_i)
        з природними умовами c_0 = c_n = 0.

    Після знаходження c_i обчислюємо:
        a_i = f_i,
        d_i = (c_{i+1} - c_i) / h_i,
        b_i = (f_{i+1} - f_i)/h_i - (2*c_i + c_{i+1}) * h_i / 6.
    """

    x_nodes = np.asarray(x_nodes, dtype=float)
    y_nodes = np.asarray(y_nodes, dtype=float)

    n = len(x_nodes) - 1  # кількість інтервалів
    h = np.diff(x_nodes)

    if np.any(h <= 0):
        raise ValueError("Вузли мають бути впорядкованими за зростанням і різними.")

    # Тридіагональна система для внутрішніх c_i (від 1 до n-1)
    c = np.zeros(n + 1)
    if n > 1:
        A = np.zeros((n - 1, n - 1))
        rhs = np.zeros(n - 1)

        for i in range(1, n):
            hi = h[i - 1]
            hip1 = h[i]
            row = i - 1

            A[row, row] = 2 * (hi + hip1)
            if row - 1 >= 0:
                A[row, row - 1] = hi
            if row + 1 <= n - 2:
                A[row, row + 1] = hip1

            rhs[row] = 6 * ((y_nodes[i + 1] - y_nodes[i]) / hip1 -
                            (y_nodes[i] - y_nodes[i - 1]) / hi)

        c[1:n] = np.linalg.solve(A, rhs)

    a = y_nodes[:-1]
    b = np.zeros(n)
    d = np.zeros(n)

    for i in range(n):
        hi = h[i]
        b[i] = (y_nodes[i + 1] - y_nodes[i]) / hi - (2 * c[i] + c[i + 1]) * hi / 6
        d[i] = (c[i + 1] - c[i]) / hi

    return NaturalCubicSpline(x=x_nodes, a=a, b=b, c=c, d=d)


# -----------------------------------------------------------------------------
#  Обчислення значення сплайна
# -----------------------------------------------------------------------------
def evaluate_spline(spline: NaturalCubicSpline, x_values: np.ndarray) -> np.ndarray:
    x_values = np.asarray(x_values)
    result = np.zeros_like(x_values, dtype=float)

    for idx, xv in np.ndenumerate(x_values):
        # Знаходимо інтервал [x_i, x_{i+1}]
        if xv <= spline.x[0]:
            i = 0
        elif xv >= spline.x[-1]:
            i = len(spline.x) - 2
        else:
            i = np.searchsorted(spline.x, xv) - 1

        dx = xv - spline.x[i]
        result[idx] = (spline.a[i] +
                       spline.b[i] * dx +
                       spline.c[i] / 2 * dx**2 +
                       spline.d[i] / 6 * dx**3)
    return result


# -----------------------------------------------------------------------------
#  Допоміжні функції
# -----------------------------------------------------------------------------
def print_coefficients(spline: NaturalCubicSpline):
    print("\nКоефіцієнти природного кубічного сплайна:")
    header = "i".ljust(4) + "x_i".ljust(15) + "a_i".ljust(15) + \
        "b_i".ljust(15) + "c_i".ljust(15) + "d_i".ljust(15)
    print(header)
    print("-" * len(header))
    for i in range(len(spline.a)):
        print(f"{i:<4}{spline.x[i]:<15.8f}{spline.a[i]:<15.8f}"
              f"{spline.b[i]:<15.8f}{spline.c[i]:<15.8f}{spline.d[i]:<15.8f}")
    print(f"{len(spline.c)-1:<4}{spline.x[-1]:<15.8f}{f(spline.x[-1]):<15.8f}"
          f"{'-':<15}{spline.c[-1]:<15.8f}{'-':<15}")
    print(f"c0 = {spline.c[0]:.3e},  c_n = {spline.c[-1]:.3e} (повинні дорівнювати нулю для природного сплайна)")


def print_spline_functions(spline: NaturalCubicSpline):
    """
    Виводить формули кубічного сплайна для кожного інтервалу.
    Формула: s_i(x) = a_i + b_i(x - x_i) + (c_i/2)(x - x_i)^2 + (d_i/6)(x - x_i)^3
    """
    print("\n" + "=" * 80)
    print("ФУНКЦІЇ ПРИРОДНОГО КУБІЧНОГО СПЛАЙНА")
    print("=" * 80)
    print("\nФормула для кожного інтервалу [x_i, x_{i+1}]:")
    print("s_i(x) = a_i + b_i(x - x_i) + (c_i/2)(x - x_i)^2 + (d_i/6)(x - x_i)^3\n")
    
    n = len(spline.a)
    for i in range(n):
        x_i = spline.x[i]
        x_i1 = spline.x[i + 1]
        a_i = spline.a[i]
        b_i = spline.b[i]
        c_i = spline.c[i]
        d_i = spline.d[i]
        
        print(f"Інтервал [{i}]: x ∈ [{x_i:.8f}, {x_i1:.8f}]")
        print(f"s_{i}(x) = ", end="")
        
        # Виводимо формулу з коефіцієнтами
        terms = []
        
        # a_i
        if abs(a_i) > 1e-10:
            terms.append(f"{a_i:.8f}")
        
        # b_i(x - x_i)
        if abs(b_i) > 1e-10:
            if abs(x_i) < 1e-10:
                term = f"{b_i:.8f} * x"
            elif x_i > 0:
                term = f"{b_i:.8f} * (x - {x_i:.8f})"
            else:
                term = f"{b_i:.8f} * (x - ({x_i:.8f}))"
            terms.append(term)
        
        # (c_i/2)(x - x_i)^2
        if abs(c_i) > 1e-10:
            coeff = c_i / 2.0
            if abs(x_i) < 1e-10:
                term = f"{coeff:.8f} * x^2"
            elif x_i > 0:
                term = f"{coeff:.8f} * (x - {x_i:.8f})^2"
            else:
                term = f"{coeff:.8f} * (x - ({x_i:.8f}))^2"
            terms.append(term)
        
        # (d_i/6)(x - x_i)^3
        if abs(d_i) > 1e-10:
            coeff = d_i / 6.0
            if abs(x_i) < 1e-10:
                term = f"{coeff:.8f} * x^3"
            elif x_i > 0:
                term = f"{coeff:.8f} * (x - {x_i:.8f})^3"
            else:
                term = f"{coeff:.8f} * (x - ({x_i:.8f}))^3"
            terms.append(term)
        
        if terms:
            print(" + ".join(terms))
        else:
            print("0")
        
        print()
    
    print("=" * 80 + "\n")


# -----------------------------------------------------------------------------
#  Основний сценарій
# -----------------------------------------------------------------------------
def main():
    a, b = -2.0, -1.0     # проміжок інтерполяції
    n_nodes = 10          # використовуємо ті ж 10 вузлів, що і в lab4.py
    x_nodes = chebyshev_nodes(a, b, n_nodes)
    y_nodes = f(x_nodes)

    spline = build_natural_cubic_spline(x_nodes, y_nodes)
    print_coefficients(spline)
    print_spline_functions(spline)

    # Перевірка природних умов
    print(f"\nКонтроль умов природності: s''(a)= {spline.c[0]:.4e},"
          f" s''(b)= {spline.c[-1]:.4e}")

    # Оцінка точності на щільній сітці
    x_dense = np.linspace(a, b, 500)
    spline_vals = evaluate_spline(spline, x_dense)
    true_vals = f(x_dense)
    max_error = np.max(np.abs(spline_vals - true_vals))

    # Побудова графіків
    plt.figure(figsize=(12, 6))
    plt.plot(x_dense, true_vals, label='f(x)', linewidth=2)
    plt.plot(x_dense, spline_vals, '--', label='Природний кубічний сплайн', linewidth=2)
    plt.scatter(x_nodes, y_nodes, color='red', label='Вузли', zorder=5)
    plt.title('Інтерполяція природним кубічним сплайном')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.tight_layout()
    plt.show()

    # Порівняння сплайна з поліномом Лагранжа
    print("\nПорівняння методів інтерполяції...")
    lagrange_vals = lagrange_polynomial(x_dense, x_nodes, y_nodes)
    spline_error = np.abs(spline_vals - true_vals)
    lagrange_error = np.abs(lagrange_vals - true_vals)
    
    max_spline_error = np.max(spline_error)
    max_lagrange_error = np.max(lagrange_error)
    
    print(f"Максимальна похибка сплайна: {max_spline_error:.4e}")
    print(f"Максимальна похибка полінома Лагранжа: {max_lagrange_error:.4e}")
    
    # Окремий графік порівняння
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))
    
    # Графік 1: Порівняння інтерполяцій
    ax1.plot(x_dense, true_vals, label='f(x)', linewidth=2, color='blue')
    ax1.plot(x_dense, spline_vals, '--', label='Природний кубічний сплайн', 
             linewidth=2, color='green')
    ax1.plot(x_dense, lagrange_vals, ':', label='Поліном Лагранжа', 
             linewidth=2, color='orange')
    ax1.scatter(x_nodes, y_nodes, color='red', s=50, label='Вузли', zorder=5)
    ax1.set_xlabel('x', fontsize=12)
    ax1.set_ylabel('y', fontsize=12)
    ax1.set_title('Порівняння методів інтерполяції', fontsize=14)
    ax1.grid(True, alpha=0.3)
    ax1.legend(fontsize=10)
    
    # Графік 2: Похибки
    ax2.plot(x_dense, spline_error, label='Похибка сплайна', linewidth=2, color='green')
    ax2.plot(x_dense, lagrange_error, label='Похибка полінома Лагранжа', 
             linewidth=2, color='orange')
    ax2.set_xlabel('x', fontsize=12)
    ax2.set_ylabel('|Помилка|', fontsize=12)
    ax2.set_title('Похибки інтерполяції', fontsize=14)
    ax2.set_yscale('log')
    ax2.grid(True, alpha=0.3)
    ax2.legend(fontsize=10)
    
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()