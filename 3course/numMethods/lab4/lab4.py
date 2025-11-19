'''
Знайти найбiльший корiнь нелiнiйного рiвняння sh x − 12 th x − 0.311 = 0 за допомогою
iнтерполяцiї (використати iнтерполяцiйний полiном Ньютона, побудованого за 1) 10 рiвновiддаленими вузлами 2) 10 вузлами, якi є нулями
полiному Чебишова).
'''

import numpy as np
import matplotlib.pyplot as plt

# -----Визначення функції-----
def f(x):
    return np.sinh(x) - 12 * np.tanh(x) - 0.311

def find_largest_root_interval(f, a_start=-1000, b_start=1000, dx=1):
    x = a_start
    largest_interval = None
    
    while x < b_start:
        a = x
        b = x + dx
        if f(a) * f(b) < 0:  # знайшли інтервал зі зміною знаку
            largest_interval = (a, b)  # оновлюємо — останній знайдений інтервал буде з найбільшим коренем
        x += dx
    
    return largest_interval

# Розділені різниці (коеф. Ньютона)
def divided_diff(x, y):
    n = len(x)
    coef = np.copy(y)
    for j in range(1, n):
        coef[j:] = (coef[j:] - coef[j-1:-1]) / (x[j:] - x[:n-j])
    return coef

def printInterpolationNodes(x, y, n):
    print("===Interpolation Nodes===")
    for i in range(n):
        print(f"k = {i}, x = {x[i]:.4f}, y = {y[i]:.4f}")

# Поліном Ньютона
def newton_poly(x_data, coef, x):
    n = len(x_data)
    p = coef[-1]
    for k in range(1, n):
        p = coef[n-k-1] + (x - x_data[n-k-1]) * p
    return p

def chebyshev_nodes(a, b, n):
    k = np.arange(0, n)                              
    nodes = 0.5*(a + b) + 0.5*(b - a) * np.cos((2*k + 1) * np.pi / (2*n))
    return nodes

# Метод бісекції
def bisection(func, a, b, eps=1e-15, max_iter=1000):
    fa, fb = func(a), func(b)
    if fa * fb > 0:
        return None  # немає зміни знаку
    for _ in range(max_iter):
        c = 0.5 * (a + b)
        fc = func(c)
        if abs(fc) < eps or (b - a) / 2 < eps:
            return c
        if fa * fc < 0:
            b = c
            fb = fc
        else:
            a = c
            fa = fc
    return 0.5 * (a + b)

def pretty_newton_poly(x_data, coef):
    s = "P(x) =\n"
    for i in range(len(coef)):
        s += f"  {coef[i]: .6f}"
        for j in range(i):
            s += f"*(x - {x_data[j]: .6f})"
        if i < len(coef)-1:
            s += " +\n"
    return s

a, b = find_largest_root_interval(f, a_start=-10, b_start=10, dx=1)

X = np.linspace(a, b, 500)
Y = f(X)

if f(a) * f(b) >= 0:
    print(
        f"There is no solution on the interval [{a}, {b}] => f(a) * f(b) >= 0.")
else:
    print(f"There is the biggest solution on the interval [{a}, {b}] => f(a) * f(b) < 0.")



n = 10
# рівновіддалені вузли
x_eq = np.linspace(a, b, n)
y_eq = f(x_eq)

# чебишовські вузли
x_ch = chebyshev_nodes(a, b, n)

x_ch=np.sort(x_ch)

y_ch = f(x_ch)

print("For eq nodes: ")
printInterpolationNodes(x_eq, y_eq, n)

coef_eq = divided_diff(x_eq, y_eq)
coef_ch = divided_diff(x_ch, y_ch)

P_eq = [newton_poly(x_eq, coef_eq, xi) for xi in X]
P_ch = [newton_poly(x_ch, coef_ch, xi) for xi in X]

root_exact = bisection(f, a, b)
root_eq = bisection(lambda x: newton_poly(x_eq, coef_eq, x), a, b)
root_ch = bisection(lambda x: newton_poly(x_ch, coef_ch, x), a, b)

print(pretty_newton_poly(x_eq, coef_eq))

print("\nFor ch nodes: ")
printInterpolationNodes(x_ch, y_ch, n)
print(pretty_newton_poly(x_ch, coef_ch))


print(f"\nExact root: {root_exact}")
print(f"Approximate (equidistant nodes): {root_eq}")
print(f"Approximate (chebishov): {root_ch}")

# ------------------- 2 графіки -------------------
fig, axs = plt.subplots(1, 2, figsize=(14,6))

# ---- 1. Повний графік ----
axs[0].plot(X, Y, 'k-', label='f(x)')
axs[0].plot(X, P_eq, 'r--', label='Інтерп. (рівновіддал.)')
axs[0].plot(X, P_ch, 'b-.', label='Інтерп. (Чебишев)')
axs[0].axhline(0, color='gray', linewidth=1)
axs[0].scatter(root_exact, f(root_exact), color='black', s=70, marker='x', label='Точний корінь')
axs[0].scatter(root_eq, newton_poly(x_eq, coef_eq, root_eq), color='red', s=70, marker='o', label='Рівновіддал.')
axs[0].scatter(root_ch, newton_poly(x_ch, coef_ch, root_ch), color='blue', s=70, marker='^', label='Чебишев')
axs[0].set_title("Повний графік")
axs[0].set_xlabel("x")
axs[0].set_ylabel("y")
axs[0].grid(True)
axs[0].legend()

# ---- 2. Приближений графік ----
axs[1].plot(X, Y, 'k-', label='f(x)')
axs[1].plot(X, P_eq, 'r--', label='Інтерп. (рівновіддал.)')
axs[1].plot(X, P_ch, 'b-.', label='Інтерп. (Чебишев)')
axs[1].set_xlim(3.2020706202417784, 3.2020706202418898)
axs[1].set_ylim(1.682176e-06, 1.682285e-06)
axs[1].axhline(0, color='gray', linewidth=1)
axs[1].scatter(root_exact, f(root_exact), color='black', s=70, marker='x', label='Точний корінь')
axs[1].scatter(root_eq, newton_poly(x_eq, coef_eq, root_eq), color='red', s=70, marker='o', label='Рівновіддал.')
axs[1].scatter(root_ch, newton_poly(x_ch, coef_ch, root_ch), color='blue', s=70, marker='^', label='Чебишев')
axs[1].set_title("Повний графік")
axs[1].set_xlabel("x")
axs[1].set_ylabel("y")
axs[1].grid(True)
axs[1].legend()

plt.suptitle("Інтерполяція Ньютона та збільшення біля кореня", fontsize=14)
plt.show()