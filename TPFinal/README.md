# Agenda de Contactos
---

## Compilación

El Makefile tiene 4 comandos principales

```shell
> make tests # construye los tests
> make cbook # construye la agenda
> make all   # construye los dos programas
> make clean # borra los archivos de compilación ./build y ./bin
```

Dentro de la carpeta `./bin` se encuentran los dos programas, para correrlos simplemente escribimos

```shell
> ./bin/tests
> ./bin/cbook
```
respectivamente. Opcionalmente se puede verificar la limpieza de memoria usando el programa `valgrind` con el siguiente comando

```shell
> valgrind --leak-check=full ./bin/tests
```

## Casos de Prueba
Los casos de prueba se encuentran en la carpeta `input` del proyecto

## Módulos
Los módulos que se encuentran en el proyecto son

- **String**: manejo dinámico de strings
- **Action**: datos relacionados a las acciones que se pueden deshacer y rehacer
- **Contact**: datos de contacto
- **ActionStack**: pila circular con pila auxiliar para manejar deshacer y rehacer acciones
- **ContactArray**: array dinámico de contactos
- **ContactBook**: tabla hash de contactos
- **Algorithms**: algoritmos varios
- **CSV**: manejo de archivos con extensión `.csv`

## Estructuras de Datos

Como se mencionó previamente, se utilizaron 3 tipos de estructuras en el proyecto. 

La primera y la más sencilla de explicar es la de array dinámico, que se utiliza tanto para strings como para array de contactos. Usar arrays dinámicos permite la abstracción del manejo de memoria, con inserción al final en `O(1)` (amortizado) y acceso por índice. Además podemos acceder al tamaño en la estructura, lo que facilita la iteración.

La segunda estructura utilizada es una "pila circular", a lo que me refiero con esto es que se comporta como una pila de tamaño fijo, que pisa los valores más viejos cuando no hay más lugar. Tenemos una operación de inserción que inserta "arriba", tenemos una operación que vuelve para atrás, quitando acciones de "arriba". La salvedad es que nos apoyamos en una segunda pila, que guarda estas vueltas hacia atrás, para permitir la reinserción en el mismo orden al volver hacia adelante. Esta pila también tiene tamaño fijo, y es el mismo que el de la otra pila. Las operaciones de inserción y borrado de las pilas son `O(1)`, y nuestra memoria es constante y es `O(max_a)` que es el máximo de acciones que se pueden retroceder.

La tercer y más importante estructura de datos utilizada es una tabla hash dinámica con resolución de colisiones usando linear probing, que marca elementos borrados como `Tombstone` y con carga máxima del 75% (como en Java). Para las operaciones que requiere el programa, ésta estructura es óptima, pues nos proporciona búsqueda, inserción y borrado en prácticamente `O(1)`, además nos aprovechamos del uso de la caché con linear probing. Para muchas operaciones se requiere iteración en `O(N)` sin importar la estructura, en ninguna se ganaba velocidad eligiendo algo que no sea una tabla hash.

## Algoritmos

Para deshacer/rehacer, como se ha dicho, se implementaron un par de pilas de tamaño fijo e igual a la cantidad máxima de veces que se puede deshacer y rehacer, una de las cuales pisa el valor más viejo se inserta una acción que supera el máximo. Al deshacer, se quita la última acción de la pila principal y se inserta en la segunda pila. Al rehacer, se quita la acción arriba de la segunda pila y se reinserta en la pila principal. Si se realiza una nueva acción cuando hay acciones en la segunda pila, insertamos normalmente pero vaciando la segunda pila, pues estas acciones ya no son recuperables.

Para guardar ordenado se utilizó la función de C `qsort` que implementa el algoritmo de ordenamiento *quick sort*, que ordena en `O(N * log(N))` un array, en este caso de contactos.

Para buscar por suma de edades, se utiliza la solución clásica del problema NP-completo *subset sum* en tiempo `O(N * suma)` usando *Divide and Conquer* con *memoización*. En competencias esto **también** se conoce como *Programación Dinámica*, pero la definición formal difiere, y carece de posibles optimizaciones del espacio de memoria, que en mí implementación, es igual a la complejidad de tiempo.

## Desarrollo y Dificultades

Al comenzar el trabajo me empecé a plantear los datos que iba a necesitas, más que las estructuras. Además me puse a pensar qué utilidades me iba a hacer falta.

Hice primero el `main`, opté por pensar *top-down* para planear correctamente mis interfaces. Quería precisar cómo las quería usar.

Así me planteé la interfaz de las estructuras de datos del diccionario, de la pila circular, y del manejo de archivos. También consolidé mayormente los datos que mis estructuras contendrían, o sea, strings, contactos y acciones.

En este punto no estaban separados mis algoritmos, mi array dinámico de contactos, que terminé usando mayormente como utilidad y estructura intermedia. Antes de esto pasaba directamente el diccionario para todo. Los algoritmos volvían muy grandes sus archivos de origen, por lo que era mejor separarlos. Además extraje el ordenamiento, que estaba en `main` por ser la implementación principal parte del lenguaje.

Lo más destacable del desarrollo, fue que inicialmente mi diccionario era un AVL y no una tabla hash. Esta estructura lograba lo mismo que la tabla hash, pero de forma más lenta, en `O(Log N)`. La razón de mi primer elección fue que tenía una hecha hacía poco en C++ y la adapté, además de que pensaba utilizarla para ordenar automáticamente a medida que inserto, y así cumplir los requerimientos de "guardar ordenado". Tendría 4 o 5 modos de ordenamiento en mi AVL y se ordenaría mientras insertaba. En este punto el array de contactos no existía, entonces tenía sentido. Cuando cambié a tabla hash, la necesidad de un array de contactos se hizo evidente, y lo agregué, para luego usarlo en muchos lugares que antes requerían tener el diccionario.

## Bibliografía

- [Subset sum](https://en.wikipedia.org/wiki/Subset_sum_problem)
- [Hash Table](https://en.wikipedia.org/wiki/Hash_table)
- [C/C++ docs](https://www.cplusplus.com/)
- [AVL C++](https://gist.github.com/lushoBarlett/bec28f15c201b4d9cf71a41a13f3db26)
- Teoría proporcionada por la cátedra