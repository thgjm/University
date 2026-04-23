import numpy as np
import matplotlib.pyplot as plt
from sympy import symbols, expand, simplify


def func(x):
    return np.sinh(x) - 12 * np.tanh(x) - 0.311

a_global = 3
b_global = 4
n = 10  # з минулої лабораторної

# ===з минулої лабораторної===

def chebyshev_nodes(a, b, n):
    k = np.arange(0, n)
    nodes = 0.5*(a + b) + 0.5*(b - a) * np.cos((2*k + 1) * np.pi / (2*n))
    return nodes

# Розділені різниці (коеф. Ньютона)
def divided_diff(x, y):
    n = len(x)
    coef = np.copy(y)
    for j in range(1, n):
        coef[j:] = (coef[j:] - coef[j-1:-1]) / (x[j:] - x[:n-j])
    return coef

# Поліном Ньютона
def newton_poly(x_data, coef, x):
    n = len(x_data)
    p = coef[-1]
    for k in range(1, n):
        p = coef[n-k-1] + (x - x_data[n-k-1]) * p
    return p

def pretty_newton_poly(x_data, coef):
    s = "P(x) =\n"
    for i in range(len(coef)):
        s += f"  {coef[i]: .6f}"
        for j in range(i):
            s += f"*(x - {x_data[j]: .6f})"
        if i < len(coef)-1:
            s += " +\n"
    return s

x_nodes = chebyshev_nodes(a_global, b_global, n)
x_nodes=np.sort(x_nodes)
y_nodes = func(x_nodes)
# ===                   ===


# ===знаходження усіх коефіцієнтів===

def natural_cubic_spline_M(x, y):
    n = len(x) - 1
    h = np.diff(x) # x[i]-x[i-1]

    rhs = np.zeros(n-1) # права частина системи
    for i in range(1, n):
        rhs[i-1] = 6.0 * ((y[i+1] - y[i]) / h[i] - (y[i] - y[i-1]) / h[i-1])

    # коефіцієнти тридіагональної матриці
    a = h[1:-1]           # піддіагональ: h2..h_{n-1}
    c = 2.0 * (h[:-1] + h[1:])  # головна діагональ (len n-1)
    b = h[1:-1]          # наддіагональ: h1..h_{n-2}

    m = len(c)
    A = np.zeros((m, m))
    for i in range(m):
        A[i][i] = c[i]
    for i in range(1, m):
        A[i][i-1] = a[i-1]
    for i in range(m-1):
        A[i][i+1] = b[i]

    new_c = np.linalg.solve(A, rhs)

    c_coef = np.zeros(n+1)
    c_coef[1:n] = new_c

    a_coef = y[1:]
    b_coef = np.zeros(len(c_coef)-1)
    d_coef = np.zeros(len(c_coef)-1)
    d_coef = (c_coef[1:]-c_coef[:-1]) / h
    b_coef = (h/2)*c_coef[1:] - ((h**2)/6)*d_coef+(y[1:]-y[:-1])/h
    return a_coef, b_coef, c_coef, d_coef

# ===вивід таблиці коефіцієнтів===
def print_table(x, y, a, b, c, d):
    print("===Coefficients===\n")
    print("|\t\t x \t\t |\t\t y \t\t |\t\t a \t\t |\t\t b \t\t |\t\t c \t\t |\t\t d \t\t |")
    print(
        f"|\t {x[0]:.6f} \t |\t {y[0]:.6f} \t |\t\t  \t\t |\t\t  \t\t |\t\t  \t\t |\t\t  \t\t |")
    for i in range(len(c)-1):
        print(f"|\t {x[i+1]:.6f} \t |\t {y[i+1]:.6f} \t |\t {a[i]:.6f} \t |\t {b[i]:.6f} \t |\t {c[i+1]:.6f} \t |\t {d[i]:.6f} \t |")

# ===допоміжні функції для виводу сплайнів===
def remove_small_terms(expr, eps=1e-10):
    return expr.xreplace({
        n: 0 for n in expr.atoms() if n.is_Number and abs(float(n)) < eps
    })

def format_coef(value):
    s = f"{value:.6f}".rstrip('0').rstrip('.')
    return s

def format_term(coef, power, is_first=False):
    coef_f = float(coef)

    if abs(coef_f) < 1e-12:
        return ""

    if coef_f < 0:
        sign = "-" if is_first else "- "
    else:
        sign = "" if is_first else "+ "

    coef_abs = abs(coef_f)

    coef_str = format_coef(coef_abs)

    if coef_str == "1" and power != 0:
        coef_str = ""

    if power == 0:
        term = coef_str
    elif power == 1:
        term = f"{coef_str}x"
    else:
        term = f"{coef_str}x^{power}"

    return sign + term

def simplified_spline_formulas(x, a, b, c, d):
    formulas = []
    x_sym = symbols('x')

    for i in range(len(a)):
        xi = x[i+1]

        Ai = a[i]
        Bi = b[i]
        Ci = c[i+1]
        Di = d[i]

        expr = (
            Ai
            + Bi * (x_sym - xi)
            + (Ci / 2) * (x_sym - xi)**2
            + (Di / 6) * (x_sym - xi)**3
        )

        expr_simple = simplify(expand(expr))

        expr_clean = remove_small_terms(expr_simple)

        expr_poly = expr_clean.as_poly(x_sym)
        coeffs = expr_poly.all_coeffs()[::-1]

        terms = []
        for power, coef in enumerate(coeffs):
            is_first = len(terms) == 0
            term = format_term(coef, power, is_first)
            if term:
                terms.append(term)

        expr_sorted_str = " ".join(terms)
        if expr_sorted_str.startswith("+"):
            expr_sorted_str = expr_sorted_str[1:].strip()

        interval = f"{x[i]:.6f} < x <= {x[i+1]:.6f}"
        formulas.append((expr_sorted_str, interval))

    return formulas

# ===                   ===


# обчислення полінома S
def evaluate_spline(x, x_nodes, a, b, c, d):
    if x <= x_nodes[0]:
        i = 0
    elif x >= x_nodes[-1]:
        i = len(x_nodes) - 2
    else:
        for i in range(len(x_nodes)-1):
            if x_nodes[i] <= x <= x_nodes[i+1]:
                break
    dx = x - x_nodes[i+1]
    return a[i] + b[i]*dx + c[i+1]*dx**2 / 2 + d[i]*dx**3 / 6

# main
def main():
    a, b, c, d = natural_cubic_spline_M(x_nodes, y_nodes)

    print_table(x_nodes, y_nodes, a, b, c, d)

    forms = simplified_spline_formulas(x_nodes, a, b, c, d)
    i=1
    # вивід сплайнів
    print("\n===Splains===\n")
    for f, interval in forms:
        print(f"s{i}(x) = {f},   {interval}")
        i=i+1

    # для поліному Ньютона
    X = np.linspace(a_global, b_global, 500)
    Y = func(X)
    coef = divided_diff(x_nodes, y_nodes)
    P = np.array([newton_poly(x_nodes, coef, xi) for xi in X])
    print("\n===Newton's polynome===\n")
    print(pretty_newton_poly(x_nodes, coef))

    S=np.array([evaluate_spline(xi, x_nodes, a, b, c, d) for xi in X])

    max_diff_SP = np.max(np.abs(S - P))
    max_diff_SF = np.max(np.abs(S - Y))
    max_diff_PF = np.max(np.abs(P - Y))

    print("\nMax |S(x) - P_newton(x)| = {:.6e}".format(max_diff_SP))
    print("Max |S(x) - f(x)|        = {:.6e}".format(max_diff_SF))
    print("Max |P_newton(x) - f(x)| = {:.6e}".format(max_diff_PF))

    # побудова графіків
    fig, axs = plt.subplots(1, 2, figsize=(14,6))

    # ===1 графік===
    axs[0].plot(X, Y, 'k-', label='f(x)')
    axs[0].plot(X, P, 'r--', label='Інтерполяція за поліномом Ньютона')
    axs[0].plot(X, S, 'b-', label='Сплайни', alpha=0.6, linewidth=1)
    axs[0].axhline(0, color='gray', linewidth=1)
    axs[0].set_title("Повний графік")
    axs[0].set_xlabel("x")
    axs[0].set_ylabel("y")
    axs[0].grid(True)
    axs[0].legend()

    # ===2 графік===
    axs[1].plot(X, Y, 'k-', label='f(x)')
    axs[1].plot(X, P, 'r--', label='Інтерполяція за поліномом Ньютона')
    axs[1].plot(X, S, 'b-', label='Сплайни', alpha=0.6, linewidth=1)
    axs[1].set_xlim(3.434970877, 3.434974111)
    axs[1].set_ylim(3.21306985, 3.21307810)
    axs[1].axhline(0, color='gray', linewidth=1)
    axs[1].set_title("Приближений графік")
    axs[1].set_xlabel("x")
    axs[1].set_ylabel("y")
    axs[1].grid(True)
    axs[1].legend()

    plt.suptitle("Інтерполяція Ньютона, сплайни та збільшення", fontsize=14)
    plt.show()
    
main()