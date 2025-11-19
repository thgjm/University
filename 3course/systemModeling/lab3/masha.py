import numpy as np
from scipy.linalg import inv
import pandas as pd
import matplotlib.pyplot as plt

plt.style.use('seaborn-v0_8-whitegrid')

# -------------------- Параметри --------------------
P_SIZE = 7
C2_KNOWN, C3_KNOWN, C4_KNOWN, M3_KNOWN = 0.3, 0.2, 0.12, 18.0
BETA_INDICES = [3, 0, 1]  # [c1, m1, m2]
BETA_0_GUESS = np.array([0.1, 11.0, 23.0])
M = len(BETA_INDICES)
TRUE_BETA_VALS = np.array([0.08, 10.0, 20.0])

T0, T, DT = 0.0, 50.0, 0.2
t_points = np.arange(T0, T + DT, DT)
X0 = np.zeros(6)


# -------------------- Зовнішні сили --------------------
def external_forces(t):
    return np.array([np.sin(t), 0.0, 0.0])


# -------------------- Параметри --------------------
def create_full_params(beta_vals):
    full_params = np.zeros(P_SIZE)
    full_params[2] = M3_KNOWN
    full_params[4] = C2_KNOWN
    full_params[5] = C3_KNOWN
    full_params[6] = C4_KNOWN
    for i, idx in enumerate(BETA_INDICES):
        full_params[idx] = beta_vals[i]
    return full_params


# -------------------- Система ODE --------------------
def system_ode(t, x, params):
    m1, m2, m3, c1, c2, c3, c4 = params
    f1, f2, f3 = external_forces(t)

    dx1, dx2, dx3 = x[3], x[4], x[5]
    dx4 = -(c1 + c2)/m1 * x[0] + c2/m1 * x[1] + f1
    dx5 = c2/m2 * x[0] - (c2 + c3)/m2 * x[1] + c3/m2 * x[2] + f2
    dx6 = c3/m3 * x[1] - (c3 + c4)/m3 * x[2] + f3

    return np.array([dx1, dx2, dx3, dx4, dx5, dx6])


# -------------------- Компоненти чутливості --------------------
def sensitivity_ode_components(x, params):
    m1, m2, m3, c1, c2, c3, c4 = params

    A = np.zeros((6, 6))
    A[0, 3], A[1, 4], A[2, 5] = 1.0, 1.0, 1.0
    A[3, 0] = -(c1 + c2)/m1; A[3, 1] = c2/m1
    A[4, 0] = c2/m2; A[4, 1] = -(c2 + c3)/m2; A[4, 2] = c3/m2
    A[5, 1] = c3/m3; A[5, 2] = -(c3 + c4)/m3

    dF_dBeta = np.zeros((6, M))
    dF_dBeta[3, 0] = -x[0]/m1
    dF_dBeta[3, 1] = (c1 + c2)/(m1**2)*x[0] - c2/(m1**2)*x[1]
    dF_dBeta[4, 2] = -c2/(m2**2)*x[0] + (c2 + c3)/(m2**2)*x[1] - c3/(m2**2)*x[2]

    return A, dF_dBeta


# -------------------- RK4 --------------------
def rk4_step(f, t, x, dt, params):
    k1 = f(t, x, params)
    k2 = f(t + dt/2, x + dt/2 * k1, params)
    k3 = f(t + dt/2, x + dt/2 * k2, params)
    k4 = f(t + dt, x + dt * k3, params)
    return x + dt/6 * (k1 + 2*k2 + 2*k3 + k4)


def rk4_integrate(f, x0, t_points, params):
    X_sol = np.zeros((len(t_points), len(x0)))
    X_sol[0] = x0
    for i in range(1, len(t_points)):
        X_sol[i] = rk4_step(f, t_points[i-1], X_sol[i-1], t_points[i]-t_points[i-1], params)
    return X_sol


# -------------------- Генерація спостережень --------------------
TRUE_PARAMS = create_full_params(TRUE_BETA_VALS)
Y_BAR_OBSERVATION = rk4_integrate(system_ode, X0, t_points, TRUE_PARAMS)


# -------------------- Алгоритм ідентифікації --------------------
def identification_algorithm(x0, y_bar_obs, t_points, initial_beta_guess, epsilon=1e-6, max_iter=10):
    dt = t_points[1] - t_points[0]
    beta_k = initial_beta_guess.copy()
    results = []

    for k in range(max_iter):
        full_params_k = create_full_params(beta_k)

        # Об’єднана система y + U
        def coupled_ode(t, X_coupled, full_params):
            x = X_coupled[:6]
            U_flat = X_coupled[6:]
            dx_dt = system_ode(t, x, full_params)
            A, dF_dBeta = sensitivity_ode_components(x, full_params)
            U = U_flat.reshape(6, M)
            dU_dt = A @ U + dF_dBeta
            return np.concatenate([dx_dt, dU_dt.flatten()])

        # RK4
        X_c0 = np.concatenate([x0, np.zeros(6*M)])
        X_c_sol = np.zeros((len(t_points), len(X_c0)))
        X_c_sol[0] = X_c0
        for i in range(1, len(t_points)):
            X_c_sol[i] = rk4_step(coupled_ode, t_points[i-1], X_c_sol[i-1], dt, full_params_k)

        y_t_beta_k = X_c_sol[:, :6]
        U_flat_k_sol = X_c_sol[:, 6:]

        E_t = y_bar_obs - y_t_beta_k
        Integral_UT_U = np.zeros((M, M))
        Integral_UT_E = np.zeros(M)

        for i in range(len(t_points)):
            U_t = U_flat_k_sol[i].reshape(6, M)
            Integral_UT_U += U_t.T @ U_t * dt
            Integral_UT_E += U_t.T @ E_t[i] * dt

        Delta_beta_k = inv(Integral_UT_U) @ Integral_UT_E
        beta_next = beta_k + Delta_beta_k
        I_beta_k = np.sum(E_t**2) * dt

        results.append({'k': k+1, 'c1_k': beta_k[0], 'm1_k': beta_k[1], 'm2_k': beta_k[2],
                        '||Delta_beta||': np.linalg.norm(Delta_beta_k), 'I(beta)': I_beta_k})

        if np.linalg.norm(Delta_beta_k) < epsilon:
            # Оновлюємо beta_k до beta_next перед виходом
            beta_k = beta_next
            # Обчислюємо фінальне рішення з оновленими параметрами
            full_params_final = create_full_params(beta_k)
            X_c0_final = np.concatenate([x0, np.zeros(6*M)])
            X_c_sol_final = np.zeros((len(t_points), len(X_c0_final)))
            X_c_sol_final[0] = X_c0_final
            for i in range(1, len(t_points)):
                X_c_sol_final[i] = rk4_step(coupled_ode, t_points[i-1], X_c_sol_final[i-1],
                                            dt, full_params_final)
            y_t_beta_k = X_c_sol_final[:, :6]
            E_t_final = y_bar_obs - y_t_beta_k
            I_beta_k = np.sum(E_t_final**2) * dt
            break
        beta_k = beta_next

    results_df = pd.DataFrame(results)
    
    print("\nТаблиця всіх ітерацій:")
    print(results_df.to_string(index=False))
    
    print(f"\n{'='*60}")
    print("ОТРИМАНІ ЗНАЧЕННЯ ПАРАМЕТРІВ:")
    print(f"{'='*60}")
    print(f"c1 = {beta_k[0]:.10f}")
    print(f"m1 = {beta_k[1]:.10f}")
    print(f"m2 = {beta_k[2]:.10f}")
    print(f"\nПоказник якості ідентифікації параметрів:")
    print(f"I(β) = {I_beta_k:.10e}")
    print(f"{'='*60}")
    
    return beta_k, I_beta_k, y_t_beta_k, results_df


# -------------------- Візуалізація --------------------
def plot_results(t_points, Y_BAR_OBSERVATION, final_y_sol, results_df):
    labels = ['$y_1(t)$', '$y_2(t)$', '$y_3(t)$']

    fig, axes = plt.subplots(3, 1, figsize=(10, 8), sharex=True)
    for i in range(3):
        axes[i].plot(t_points, Y_BAR_OBSERVATION[:, i], 'r--', linewidth=2, label='Істинне')
        axes[i].plot(t_points, final_y_sol[:, i], 'b-', linewidth=1, label='Модель')
        axes[i].set_ylabel(labels[i])
        axes[i].legend()
    axes[-1].set_xlabel('Час t, c')
    plt.tight_layout()
    plt.show()


# -------------------- Запуск --------------------
identified_beta, final_I_beta, final_y_sol, results_df = identification_algorithm(
    X0, Y_BAR_OBSERVATION, t_points, BETA_0_GUESS, epsilon=1e-6, max_iter=10
)

plot_results(t_points, Y_BAR_OBSERVATION, final_y_sol, results_df)