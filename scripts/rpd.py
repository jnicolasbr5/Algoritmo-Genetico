import pandas as pd
 
# Leer el CSV
datos = pd.read_csv("../resultados/resultados.csv")
 
# Crear la columna del grupo: 20x5, 20x10, ...
datos["grupo"] = datos["n"].astype(str) + "x" + datos["m"].astype(str)
 
# Media, desviación, mínimo y máximo del RPD por grupo
tabla = datos.groupby(["n", "m", "grupo"])["RPD"].agg(["mean", "std", "min", "max"])
 
# Quitar n y m del índice (solo servían para ordenar bien los grupos)
tabla = tabla.reset_index().drop(columns=["n", "m"]).set_index("grupo")
tabla.columns = ["RPD_medio", "desviacion", "RPD_min", "RPD_max"]
 
# Fila final con la media de todos los grupos
tabla.loc["Media"] = tabla.mean()
 
# Mostrar y guardar
print(tabla.round(2))
tabla.round(2).to_csv("../resultados/tabla_rpd.csv")
print("Guardada ../resultados/tabla_rpd.csv")