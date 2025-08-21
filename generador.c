#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 50
#define MAX_ALPHABET 50
#define MAX_TRANSITIONS 250
#define MAX_LINE_LENGTH 500
#define MAX_STRING_LENGTH 250

// Estructura para representar una transicion
typedef struct {
    char estado_actual[MAX_STATES];
    char simbolo;
    char estado_siguiente[MAX_STATES];
} Transicion;

typedef struct {
    char estados[MAX_STATES][MAX_STATES];
    int num_estados;
    char alfabeto[MAX_ALPHABET];
    int num_simbolos;
    Transicion transiciones[MAX_TRANSITIONS];
    int num_transiciones;
    char estado_inicial[MAX_STATES];
    char estados_finales[MAX_STATES][MAX_STATES];
    int num_estados_finales;
} Automata;

// funciones
void leer_automata(const char* archivo_automata, Automata* af);
void procesar_cadenas(const char* archivo_cadenas, Automata* af);
int aceptar_cadena(const char* cadena, Automata* af);

// Función principal
int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <automata.txt> <cadenas.txt>\n", argv[0]);
        return 1;
    }

    Automata af;
    leer_automata(argv[1], &af);
    procesar_cadenas(argv[2], &af);

    return 0;
}


void leer_estados(FILE* file, Automata* af) {
    char line[MAX_LINE_LENGTH];
    fgets(line, sizeof(line), file);
    line[strcspn(line, "\n")] = 0; // Eliminar el salto de línea

    char* token = strtok(line, ","); // Elimina las comas con la funcion strtok
    af->num_estados = 0;
    while (token != NULL) {
        strcpy(af->estados[af->num_estados++], token);
        token = strtok(NULL, ",");
    }
}

void leer_alfabeto(FILE* file, Automata* af) {
    char line[MAX_LINE_LENGTH];
    fgets(line, sizeof(line), file);
    line[strcspn(line, "\n")] = 0;

    char* token = strtok(line, ",");
    af->num_simbolos = 0;
    while (token != NULL) {
        af->alfabeto[af->num_simbolos++] = token[0];
        token = strtok(NULL, ",");
    }
}

// Lee la línea de transiciones
void leer_transiciones(FILE* file, Automata* af) {
    char line[MAX_LINE_LENGTH];
    fgets(line, sizeof(line), file);
    line[strcspn(line, "\n")] = 0;

    char* token = strtok(line, ",");
    af->num_transiciones = 0;
    while (token != NULL) {
        // Formato: (q0:a)=q1
        char from[MAX_STATES], to[MAX_STATES];
        char symbol;
        sscanf(token, "(%[^:]:%c)=%s", from, &symbol, to);

        strcpy(af->transiciones[af->num_transiciones].estado_actual, from);
        af->transiciones[af->num_transiciones].simbolo = symbol;
        strcpy(af->transiciones[af->num_transiciones].estado_siguiente, to);
        af->num_transiciones++;

        token = strtok(NULL, ",");
    }
}

// Lee la línea del estado inicial
void leer_estado_inicial(FILE* file, Automata* af) {
    char line[MAX_LINE_LENGTH];
    fgets(line, sizeof(line), file);
    line[strcspn(line, "\n")] = 0;
    strcpy(af->estado_inicial, line);
}

// Lee la línea de estados finales
void leer_estados_finales(FILE* file, Automata* af) {
    char line[MAX_LINE_LENGTH];
    fgets(line, sizeof(line), file);
    line[strcspn(line, "\n")] = 0;

    char* token = strtok(line, ",");
    af->num_estados_finales = 0;
    while (token != NULL) {
        strcpy(af->estados_finales[af->num_estados_finales++], token);
        token = strtok(NULL, ",");
    }
}


void leer_automata(const char* archivo_automata, Automata* af) {
    FILE* file = fopen(archivo_automata, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo del automata");
        exit(1);
    }

    leer_estados(file, af);
    leer_alfabeto(file, af);
    leer_transiciones(file, af);
    leer_estado_inicial(file, af);
    leer_estados_finales(file, af);

    fclose(file);
}

// Procesa el archivo de cadenas
void procesar_cadenas(const char* archivo_cadenas, Automata* af) {
    FILE* file = fopen(archivo_cadenas, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo de cadenas");
        exit(1);
    }

    char cadena[MAX_STRING_LENGTH];
    while (fgets(cadena, sizeof(cadena), file) != NULL) {
        cadena[strcspn(cadena, "\n")] = 0; // Eliminar el salto de línea
        if (aceptar_cadena(cadena, af)) {
            printf("La cadena '%s' es ACEPTADA.\n", cadena);
        } else {
            printf("La cadena '%s' es RECHAZADA.\n", cadena);
        }
    }

    fclose(file);
}

int aceptar_cadena(const char* cadena, Automata* af) {
    char estado_actual[MAX_STATES];
    strcpy(estado_actual, af->estado_inicial);

    for (int i = 0; i < strlen(cadena); i++) {
        char simbolo_actual = cadena[i];
        int transicion_encontrada = 0;
        char siguiente_estado[MAX_STATES] = "";

        for (int j = 0; j < af->num_transiciones; j++) {
            if (strcmp(af->transiciones[j].estado_actual, estado_actual) == 0 &&
                af->transiciones[j].simbolo == simbolo_actual) {
                strcpy(siguiente_estado, af->transiciones[j].estado_siguiente);
                transicion_encontrada = 1;
                break;
            }
        }

        if (!transicion_encontrada) {
            return 0;
        }
        strcpy(estado_actual, siguiente_estado);
    }

    // Verificar si el estado final es un estado de aceptación
    for (int i = 0; i < af->num_estados_finales; i++) {
        if (strcmp(estado_actual, af->estados_finales[i]) == 0) {
            return 1; // El estado actual es final, la cadena es aceptada
        }
    }
    return 0; // El estado final no es de aceptación, la cadena es rechazada
}