
!pip install memory_profiler matplotlib
import time
from memory_profiler import memory_usage
import matplotlib.pyplot as plt

# ---------------- Funciones factorial ----------------
def facto_r(n):
    temp = [0] * 100000  # Forzamos consumo de memoria para que se note
    if n == 0 or n == 1:
        return 1
    else:
        return n * facto_r(n - 1)

def facto_i(n):
    temp = [0] * 100000
    resultado = 1
    for i in range(1, n + 1):
        resultado *= i
    return resultado

# ---------------- Listas para guardar resultados ----------------
valores_n = [10, 110, 400, 600, 900]  # Valores
tiempo_r, tiempo_i = [], []
memoria_r, memoria_i = [], []

# ---------------- Medición de datos ----------------
print("n\tTiempo Recursivo (s)\tTiempo Iterativo (s)\tMemoria Recursivo \tMemoria Iterativo ")
print("-"*80)

for n in valores_n:
    # Recursivo
    start_time = time.time()
    facto_r(n)
    end_time = time.time()
    tr = end_time - start_time
    tiempo_r.append(tr)

    mem_r = memory_usage((facto_r, (n,)), max_iterations=1)
    mr = max(mem_r) - min(mem_r)
    memoria_r.append(mr)

    # Iterativo
    start_time = time.time()
    facto_i(n)
    end_time = time.time()
    ti = end_time - start_time
    tiempo_i.append(ti)

    mem_i = memory_usage((facto_i, (n,)), max_iterations=1)
    mi = max(mem_i) - min(mem_i)
    memoria_i.append(mi)

    # Mostrar datos en consola
    print(f"{n}\t{tr:.6f}\t\t\t{ti:.6f}\t\t\t{mr:.6f}\t\t\t{mi:.6f}")

# ---------------- Gráfica de tiempo ----------------
plt.figure(figsize=(10, 4))
plt.plot(valores_n, tiempo_r, 'd-m', label="Recursivo")
plt.plot(valores_n, tiempo_i, 'x-c', label="Iterativo")
plt.xlabel("Valor de n")
plt.ylabel("Tiempo (segundos)")
plt.title("Comparación de tiempo de ejecución")
plt.legend()
plt.grid(True)
plt.show()

# ---------------- Gráfica de memoria ----------------
plt.figure(figsize=(10, 4))
plt.plot(valores_n, memoria_r, 'd-m', label="Recursivo")
plt.plot(valores_n, memoria_i, 'x-c', label="Iterativo")
plt.xlabel("Valor de n")
plt.ylabel("Memoria usada ")
plt.title("Comparación de uso de memoria")
plt.legend()
plt.grid(True)
plt.show()
