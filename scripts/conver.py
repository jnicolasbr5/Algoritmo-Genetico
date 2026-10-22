import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("../resultados/convergencia.csv")

plt.figure(figsize=(8, 5))
plt.plot(df["evaluaciones"], df["makespan"])
plt.ylim(1000, df["makespan"].max() * 1.1)
plt.xlabel("Evaluaciones")
plt.ylabel("Mejor makespan")
plt.title("Progresión del makespan")
plt.grid(alpha=0.3)
plt.savefig("convergencia.png", dpi=150)
plt.show()