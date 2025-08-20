import sys

class Automata:
    def __init__(self, estados, alfabeto, transiciones, estado_inicial, estados_finales):
        self.estados = estados
        self.alfabeto = alfabeto
        
        # Validar origen de transiciones
        if not all(origen in estados for (origen, letra) in transiciones.keys()):
            print("Las transiciones deben estar con estados válidos")
            sys.exit()

        self.transiciones = transiciones

        if estado_inicial not in estados:
            print("El estado inicial no está en el conjunto")
            sys.exit()

        self.estado_inicial = estado_inicial

        if not all(estado in estados for estado in estados_finales):
            print("Los estados finales no están en el conjunto")
            sys.exit()

        self.estados_finales = estados_finales
        

def main():
    if len(sys.argv) != 2:
        print("Agregue un archivo")
        return

    caracteres = str.maketrans("", "", "{}\n()")
    nombre_archivo = sys.argv[1]

    try:
        with open(nombre_archivo, "r") as f:
            lineas = f.readlines() 
       
            if len(lineas) != 5:
                print("El archivo debe tener 5 líneas")
                return

            estados = lineas[0].translate(caracteres).split(",")
            alfabeto = lineas[1].translate(caracteres).split(",")
            estados = [e.strip() for e in lineas[0].translate(caracteres).split(",")]
            alfabeto = [a.strip() for a in lineas[1].translate(caracteres).split(",")]

            transiciones_raw = lineas[2].translate(caracteres).split(",")
            transiciones = {}
            print(transiciones_raw)
            for t in transiciones_raw:
                if not t.strip():
                    continue

                izquierda, destino = t.split("=", 1)

                izquierda = izquierda.split(":")

                transiciones[(izquierda[0], izquierda[1])] = destino

            estado_inicial = lineas[3].translate(caracteres).strip()
            estados_finales = lineas[4].translate(caracteres).split(",")
            automata = Automata(estados, alfabeto, transiciones, estado_inicial, estados_finales)
            print("Automata creado correctamente")
            print("Estados:", automata.estados)
            print("Transiciones:", automata.transiciones)

    except FileNotFoundError:
        print("Archivo no encontrado.")
    except Exception as e:
        print(f"Ocurrio un error: {e}")


if __name__ == "__main__":
    main()
