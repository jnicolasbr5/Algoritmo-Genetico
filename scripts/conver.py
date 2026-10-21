import pandas as pd
import matplotlib.pyplot as plt

# Instancias que quieres dibujar
INSTANCIAS = ["tai20_20_0", "tai100_10_0"]

# Leer el CSV
datos = pd.read_csv("../resultados/convergencia.csv")

# Quitar la ruta y la extensión: ../inst/tai100_10_0.fsp -> tai100_10_0
datos["instancia"] = datos["instancia"].str.replace("../inst/", "").str.replace(".fsp", "")

for inst in INSTANCIAS:
    plt.figure()

    # Una línea por cada semilla
    filas = datos[datos["instancia"] == inst]
    for semilla in range(1, 11):
        f = filas[filas["semilla"] == semilla]
        plt.step(f["evaluaciones"], f["mejor_cmax"], where="post", label=f"semilla {semilla}")

    plt.xlabel("Evaluaciones")
    plt.ylabel("Mejor makespan")
    plt.title("Convergencia en " + inst)
    plt.legend()
    plt.savefig("convergencia_" + inst + ".png")
    print("Guardada convergencia_" + inst + ".png")