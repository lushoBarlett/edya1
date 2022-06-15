# Intérprete de Expresiones
---

## Compilación

El Makefile tiene 4 comandos principales

```shell
> make tests # construye los tests
> make expri # construye el intérprete
> make all   # construye los dos programas
> make clean # borra los archivos de compilación ./build y ./bin
```

Dentro de la carpeta `./bin` se encuentran los dos programas, para correrlos simplemente escribimos

```shell
> ./bin/tests
> ./bin/expri
```
respectivamente. Opcionalmente se puede verificar la limpieza de memoria usando el programa `valgrind` con el siguiente comando

```shell
> valgrind --leak-check=full ./bin/tests
```

## Casos de prueba

```
Interfaz de Consola del Evaludor de Expresiones
> 1var = cargar 1 2 %        
Error: El alias no comienza con una letra.
> var1 = cargar 1 2 % ^ 2
Error: No hay suficientes operandos para un operador.
> var1 = cargar 1 2 % 2 ^
> imprimir var1
((1 % 2) ^ 2)
> evaluar var1
1
> salir var1
Error: No se pudo interpretar la acción
> imprimir salir
Error: No se pudo interpretar la acción
> salir
Saliendo del programa
```

```
Interfaz de Consola del Evaludor de Expresiones
> 1expr = cargar 1 1 +
Error: El alias no comienza con una letra.
> exp = cargar 7 3 + 20 2 / -
> imprimir exp
((7 + 3) - (20 / 2))
> evaluar exp
0
> salir
Saliendo del programa
```

## Dificultades

Planear qué estructuras de datos usar, hasta decidirnos por un array dinámico, tuvo como consecuencia la escritura de código que después tuvo que ser borrado. El mal uso de esta estructura introdujo bugs en la interpretación de las expresiones, al contar con una pila de las mismas y tener que pensar qué memoria liberar. En muchos lugares del `Parser` nos hemos olvidado de liberar memoria, pero fue fácil de corregir.