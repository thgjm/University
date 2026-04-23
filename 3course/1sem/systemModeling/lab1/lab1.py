import numpy as np
import matplotlib.pyplot as plt

#параметри
deltat = 0.01 #крок
T = 5 #кінець інтервалу спостереження
t = np.arange(0, T + deltat, deltat) #інтервал спостереження, t ∈ [0, 0.01, ..., 4.99, 5]
y = np.loadtxt(r"C:\Users\User\Desktop\uni\3course\systemModeling\lab1\f1.txt") #зчитування значень з файлу
N = len(y) #кількість елементів в інтервалі

#побудова графіку спостережуваної дискретної функції 
plt.figure()
plt.plot(t, y)
plt.grid()
plt.title("ŷ(t)")

#власна реалізаія дискретного перетворення Фур'є
dft = np.zeros(N, dtype=complex)
for k in range(N):
    for m in range(N):
        dft[k] += ( y[m] * np.exp(-1j * 2 * np.pi * k * m / N) ) / N

#побудова графіку дискретного перетворення Фур'є
plt.figure()
plt.plot(t, np.abs(dft))
plt.grid()
plt.title("DFT")

#робота з частотами
deltaf = 1 / T
yf = np.abs(dft[:N // 2])  #візьмемо першу половину спектру
f=np.arange(0, N//2*deltaf, deltaf) #частоти

plt.figure()
plt.plot(f, yf)
plt.grid()
plt.title("Амплітудний спектр")

#пошук локальних максимумів
maximum = []
for i in range(1, len(yf) - 1):
    if yf[i] > yf[i - 1] and yf[i] > yf[i + 1] and abs(yf[i] - yf[i - 1]) > 1:
        maximum.append(i*deltaf)
print("Local maximum:", maximum)

#побудова апроксимації (метод найменших квадратів)
sinf = np.sin(2 * np.pi * maximum[0] * t)
A = np.array([
    [np.sum(t**6),     np.sum(t**5),     np.sum(t**4),     np.sum(sinf*t**3), np.sum(t**3)],
    [np.sum(t**5),     np.sum(t**4),     np.sum(t**3),     np.sum(sinf*t**2), np.sum(t**2)],
    [np.sum(t**4),     np.sum(t**3),     np.sum(t**2),     np.sum(sinf*t),    np.sum(t)],
    [np.sum(sinf*t**3),np.sum(sinf*t**2),np.sum(sinf*t),   np.sum(sinf**2),   np.sum(sinf)],
    [np.sum(t**3),     np.sum(t**2),     np.sum(t),        np.sum(sinf),      N]
])

#вектор a
a = np.array([
    np.sum(y*t**3),
    np.sum(y*t**2),
    np.sum(y*t),
    np.sum(y*sinf),
    np.sum(y)
])

#розв’язуємо систему a=Ay
coeffs = np.linalg.inv(A) @ a

print("Coefficients:", np.round(coeffs, 0))

#будуємо апроксимацію
approx = coeffs[0]*t**3 + coeffs[1]*t**2 + coeffs[2]*t + coeffs[3]*sinf + coeffs[4]

plt.figure()
plt.plot(t, y, label="ŷ(t)")
plt.plot(t, approx, label="y(t)")
plt.legend()
plt.grid()
plt.title("Апроксимація сигналу")

#похибка
error = np.sum((y - approx) ** 2)
print("Error:", error)

plt.show()