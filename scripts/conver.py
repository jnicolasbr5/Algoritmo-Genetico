import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("../resultados/convergencia.csv")

plt.figure(figsize=(8, 5))
plt.plot(df["evaluaciones"], df["makespan"])
plt.ylim(5000, df["makespan"].max() * 1.02)                      # <-- aquí: el eje Y empieza en 0
plt.axhline(6441, color="green", linestyle=":", label="UB")
plt.xlabel("Evaluaciones")
plt.ylabel("Mejor makespan")
plt.title("Progresión del makespan")
plt.legend()
plt.grid(alpha=0.3)
plt.savefig("convergencia.png", dpi=150)
plt.show()