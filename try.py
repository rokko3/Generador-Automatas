import sys

class Automata:
    def __init__(self, estados, alfabeto, transiciones, estado_inicial, estados_finales):
        self.estados = estados
        self.alfabeto = alfabeto
        
        # Validar origen de transiciones
        if not all(origen in estados for (origen, letra) in transiciones.keys()):
            print("Las transiciones deben estar con estados válidos")
            sys.exit()

        # Validar letras de transiciones
        if not all(letra in alfabeto for (_, letra) in transiciones.keys()):
            print("Las transiciones deben estar con letras válidas")
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

    caracteres = str.maketrans("", "", "{}\n")
    nombre_archivo = sys.argv[1]

    try:
        with open(nombre_archivo, "r") as f:
            lineas = f.readlines() 
       
            if len(lineas) != 5:
                print("El archivo debe tener 5 líneas")
                return

            estados = lineas[0].translate(caracteres).split(",")
            alfabeto = lineas[1].translate(caracteres).split(",")

            # Procesar transiciones - FORMA CORREGIDA
            transiciones_raw = lineas[2].strip()
            transiciones = {}
            
            # Dividir por comas que separan las transiciones, pero teniendo cuidado
            # con las comas dentro de los paréntesis
            transiciones_lista = []
            temp = ""
            dentro_parentesis = False
            
            for char in transiciones_raw:
                if char == '(':
                    dentro_parentesis = True
                    temp += char
                elif char == ')':
                    dentro_parentesis = False
                    temp += char
                elif char == ',' and not dentro_parentesis:
                    transiciones_lista.append(temp.strip())
                    temp = ""
                else:
                    temp += char
            
            # Agregar la última transición
            if temp.strip():
                transiciones_lista.append(temp.strip())
            
            # Procesar cada transición individual
            for t in transiciones_lista:
                if not t.strip():
                    continue
                
                # Limpiar y procesar la transición
                t_limpia = t.strip()
                if t_limpia.startswith('(') and t_limpia.endswith(')'):
                    t_limpia = t_limpia[1:-1]  # Quitar paréntesis
                
                # Dividir en parte izquierda (origen,símbolo) y destino
                if '=' in t_limpia:
                    partes = t_limpia.split('=', 1)
                    izquierda = partes[0].strip()
                    destino = partes[1].strip()
                    
                    # Dividir la parte izquierda en origen y símbolo
                    if ',' in izquierda:
                        origen, simbolo = izquierda.split(',', 1)
                        origen = origen.strip()
                        simbolo = simbolo.strip()
                        
                        # Agregar al diccionario de transiciones
                        transiciones[(origen, simbolo)] = destino

            estado_inicial = lineas[3].translate(caracteres).strip()
            estados_finales = lineas[4].translate(caracteres).split(",")

            automata = Automata(estados, alfabeto, transiciones, estado_inicial, estados_finales)
            print("Automata creado correctamente")
            print("Estados:", automata.estados)
            print("Alfabeto:", automata.alfabeto)
            print("Estado inicial:", automata.estado_inicial)
            print("Estados finales:", automata.estados_finales)
            print("Transiciones:")
            for clave, valor in automata.transiciones.items():
                print(f"  {clave} -> {valor}")

    except FileNotFoundError:
        print("Archivo no encontrado.")
    except Exception as e:
        print(f"Ocurrió un error: {e}")
        import traceback
        traceback.print_exc()


if __name__ == "__main__":
    main()