# Generador y comprobador de automatas mediante txt
---
## Ejemplo de prueba
Ingresamos la siguiente configuracion para el automata:
```bash
q0,q1,qdead
a,b
(q0:a)=q1,(q0:b)=qdead,(q1:a)=q1,(q1:b)=qdead,(qdead:a)=qdead,(qdead:b)=qdead
q0
q0,q1
```

Para efectos de organizacion, decidi establecer que la **primera linea** del archivo sean los estados que hay en el automata, la **segunda linea** es el alfabeto que acepta (caracteres), la **tercera linea** contiene las transiciones entre estados, la **cuarta linea** es el estado inicial y la ultima linea son los estados finales.

## Integracion del automata con caracteres

```bash
a
aa
aaa
b
ab
ba
aab
aaaba
```
## Ejecucion del prorgrama

```bash
python generador.py try.txt cadenas.txt
```
## Imagen de pruebas
![alt text](image.png)