import numpy as np
import matplotlib.pyplot as plt

def f(x):
    return np.sinh(x) - 12 * np.tanh(x) - 0.311

a = 3
b = 4
n = 4 # з минулої лабораторної

def chebyshev_nodes(a, b, n):
    k = np.arange(0, n)                              
    nodes = 0.5*(a + b) + 0.5*(b - a) * np.cos((2*k + 1) * np.pi / (2*n))
    return nodes

#x_nodes = chebyshev_nodes(a, b, n)
#y_nodes = f(x_nodes)
X = np.linspace(a, b, 500)
x_nodes = np.array([0, 1, 2, 3])
y_nodes = np.array([0, 0.5, 2, 1.5])


# ---------------------------
# 2) Побудова коефіцієнтів природного кубічного сплайну
# ---------------------------
def natural_cubic_spline_M(x, y):
    n = len(x) - 1 # 10 - 1 = 9
    """
    Розв'язує тридіагональну систему для отримання других похідних M_i
    природного кубічного сплайну (M0 = Mn = 0).
    Повертає масив M довжини n+1.
    Формула згідно з підручником: h_i c_{i-1} + 2(hi-1+hi) c_i + hi+1 c_{i+1} = RHS.
    (див. Посібник, розділ «Інтерполяційний природний кубічний сплайн», стор.100). :contentReference[oaicite:1]{index=1}
    """

    h = np.diff(x)  # h_i = x_{i+1} - x_i, len = n
    if np.any(h == 0):
        raise ValueError("Вузли містять дублікати або не зростають.")
    if n == 0:
        return np.array([0.0])

    # побудова RHS (вектор правих частин) для внутрішніх вузлів i=1..n-1
    rhs = np.zeros(n-1) # 9 - 1 = 8
    for i in range(1, n):
        rhs[i-1] = 6.0 * ( (y[i+1] - y[i]) / h[i] - (y[i] - y[i-1]) / h[i-1] )

    # коефіцієнти тридіагональної матриці
    a = h[1:-1]           # піддіагональ: h2..h_{n-1}
    c = 2.0 * (h[:-1] + h[1:])  # головна діагональ (len n-1)
    b = h[1:-1]          # наддіагональ: h1..h_{n-2}

    # Thomas algorithm (метод прогонки)
    m = len(c) # =8

    # forward sweep
    alpha = np.zeros(m-1)
    beta = np.zeros(m)
    alpha[0] = b[0] / c[0] if m > 1 else 0.0
    beta[0] = rhs[0] / c[0]
    for i in range(1, m-1):
        z = -c[i] - a[i-1] * alpha[i-1]
        alpha[i] = b[i] / z
        beta[i] = (-rhs[i] + a[i-1] * beta[i-1]) / z
    if m > 1:
        z = -c[m-1] - a[m-2] * alpha[m-2]
        beta[m-1] = (-rhs[m-1] + a[m-2] * beta[m-2]) / z

    # back substitution
    xvec = np.zeros(m)
    xvec[-1] = beta[-1]
    for i in range(m-2, -1, -1):
        xvec[i] = beta[i] + alpha[i] * xvec[i+1]

    # повний c_i: c_0 = c_n = 0
    c_coef = np.zeros(n+1)
    c_coef[1:n] = xvec


    a_coef=y[1:]
    b_coef=np.zeros(len(c_coef)-1)
    d_coef=np.zeros(len(c_coef)-1)
    d_coef = (c_coef[1:]-c_coef[:-1]) / h
    b_coef = (h/2)*c_coef[1:] - ((h**2)/6)*d_coef+(y[1:]-y[:-1])/h
    return a_coef, b_coef, c_coef, d_coef


def print_table(x, y, a, b, c, d):
    print("|\t\t x \t\t |\t\t y \t\t |\t\t a \t\t |\t\t b \t\t |\t\t c \t\t |\t\t d \t\t |")
    print(f"|\t {x[0]:.4f} \t |\t {y[0]:.4f} \t |\t\t  \t\t |\t\t  \t\t |\t\t  \t\t |\t\t  \t\t |")
    for i in range(len(c)-1):
        print(f"|\t {x[i+1]:.4f} \t |\t {y[i+1]:.4f} \t |\t {a[i]:.4f} \t |\t {b[i]:.4f} \t |\t {c[i+1]:.4f} \t |\t {d[i]:.4f} \t |")


a, b, c, d = natural_cubic_spline_M(x_nodes, y_nodes)

#print(a)

print_table(x_nodes, y_nodes, a, b, c, d)


coef = divided_diff(x_nodes, y_nodes)
print("\nПоліном Ньютона:")
print(pretty_newton_poly(x_nodes, coef))

# --------------------------
# 7. Табличне порівняння значень у вузлах
# --------------------------
print(" x          S(x)        P(x)      |S-P|")
for x, y in zip(x_nodes, y_nodes):
    s_val = y        # сплайн у вузлі = y_nodes
    p_val = newton_poly(x_nodes, coef, x)
    print(f"{x: .6f}   {s_val: .6f}   {p_val: .6f}   {abs(s_val-p_val): .6e}")

# --------------------------
# 8. Графік
# --------------------------
xs = np.linspace(x_nodes[0], x_nodes[-1], 400)
ys_spline = [evaluate_spline(x, x_nodes, a, b, c, d) for x in xs]
ys_newton = [newton_poly(x_nodes, coef, x) for x in xs]

plt.figure(figsize=(8,5))
plt.plot(xs, ys_spline, label="Кубічний сплайн", linewidth=2)
plt.plot(xs, ys_newton, label="Поліном Ньютона", linestyle='--')
plt.scatter(x_nodes, y_nodes, color='black', zorder=5)
plt.xlabel("x")
plt.ylabel("y")
plt.title("Порівняння кубічного сплайну та полінома Ньютона")
plt.legend()
plt.grid(True)
plt.show()