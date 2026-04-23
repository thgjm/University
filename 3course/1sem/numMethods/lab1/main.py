import matplotlib.pyplot as plt
import numpy as np
import math
import sys
sys.stdout.reconfigure(encoding='utf-8')

epsilon = 0.0001
startA = 0
startB = 0.5

def f(x):
    return np.exp(x) - 2 * ((x - 1)**2) #e^x-2*(x-1)^2=0

def fi(x):
    return 1-np.sqrt(np.exp(x)/2) #e^x-2*(x-1)^2=0 ==> e^x=2*(x-1)^2 ==> x=1+-sqrt(e^x/2) ==> φ(x)=1+-sqrt(e^x/2)

def fi_derivative(x):
    return -np.sqrt(np.exp(x))/(2*math.sqrt(2)) #φ`(x)=+-sqrt(e^x)/(2*sqrt(2))

print("Function we are working with: e^x-2*(x-1)^2=0\n")

#=============graphic====================

x = np.linspace(-2, 4, 200)
y = f(x)
plt.figure(figsize=(8, 6))
plt.axhline(0, color='black', linewidth=0.8) 
plt.axvline(0, color='black', linewidth=0.8)
plt.plot(x, y, label="f(x) = exp(x) - 2(x-1)^2")
plt.legend()
plt.grid(True)
plt.show()

#=============common steps====================

print("Root separation: find the interval that contains a single root.\n"
f"We'll take interval [{startA}; {startB}] and check.")

print(f"f({startA}) = {f(startA)}\n"
      f"f({startB}) = {f(startB)}")

if f(startA)*f(startB)<0:
    print(f"f({startA})*f({startB}) < 0 ===> x*\u2208[{startA}; {startB}].\n")
else:
    print(f"f({startA})*f({startB}) >= 0 ===> interval does not fit here.\n")
    sys.exit()

#=============dichotomy====================

def Dichotomy(a, b, eps):
    print("=========Dichotomy=========\n")
    a_n=a   #a0
    b_n=b   #b0
    x_prev = (a_n + b_n) / 2 #x0
    x_n=x_prev
    iterations=0
    print("Iterations:")
    print(f"i={iterations}, a{iterations}={a_n}, b{iterations}={b_n}, x{iterations}={x_n}")
    #priori estimate
    priori=math.floor(math.log2((startB-startA)/epsilon)) #n>=[log2((b-a)/epsilon)]
    #==start of iteration==
    while True:
        iterations+=1
        if math.copysign(1, f(a_n)) == math.copysign(1, f(x_n)): a_n = x_n
        else: b_n = x_n
        x_n = (a_n + b_n) / 2
        print(f"i={iterations}, a{iterations}={a_n}, b{iterations}={b_n}, x{iterations}={x_n}")
        if abs(x_n - x_prev) <= eps: return x_n, iterations, priori
        x_prev = x_n

root1, posteriori, priori=Dichotomy(startA, startB, epsilon)
print("\nThe answer:")
print(f"x = {root1:.8f}")
print("Posteriori = ", posteriori) #апостеріорна оцінка
print("Priori = ", priori) #апріорна оцінка
if posteriori == priori: print("Posteriori = Priori")

#=============simple iteration====================

def find_max(f, a, b, steps=10000):
    x_vals = np.linspace(a, b, steps)
    y_vals = abs(f(x_vals))
    idx = np.argmax(y_vals)
    return y_vals[idx]

x=(startA+startB)/2 #x0 - initial approximation

def simpleIteration(a, b, eps):
    print("\n=========Simple iteration=========\n")
    iterations=0
    x_prev=x
    delta=abs(startA-x)
    #==convergence check==
    if find_max(fi_derivative, a, b)<1: q=find_max(fi_derivative, a, b) #1st condition
    if abs(fi(x)-x)>(1-q)*delta: return #2nd condition
    if q<1 and abs(fi(x)-x)<=(1-q)*delta: print("Convergence is proven.")
    #priori estimate
    priori=math.floor(math.log(abs(fi(x)-x)/((1-q)*epsilon))/math.log(1/q))+1 #n>=[ln(|fi(x0)-x0|/((1-q)*epsilon))/ln(1/q)]+1
    #==start of iteration==
    while True:
        iterations+=1
        x_n=fi(x_prev)
        print(f"i={iterations}, x{iterations}={x_n:.8f}")
        if q<1/2:
            if abs(x_n-x_prev)<=((1-q)/q)*eps: return x_n, iterations, q, priori
        else:
            if abs(x_n-x_prev)<=eps: return x_n, iterations, q, priori
        x_prev=x_n

root2, posteriori, q, priori=simpleIteration(startA, startB, epsilon)
print("\nThe answer:")
print(f"x = {root2:.8f}")
print("Posteriori = ", posteriori) #апостеріорна оцінка
print("Priori = ", priori) #апріорна оцінка
if posteriori == priori: print("Posteriori = Priori")

#=============conclusion====================

print("\n=========Conclusion=========\n")
print(f"Our start variables:\nInitial interval: [{startA}; {startB}]\nInitial approximation x0: {(startA+startB)/2}\nEpsilon: {epsilon}")
print(f"Dichotomy root answer: {root1:.8f}")
print(f"Simple iteration root answer: {root2:.8f}")
print(f"Difference between roots: {np.abs(root1-root2):.8f}")