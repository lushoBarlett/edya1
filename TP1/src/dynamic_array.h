#pragma once

#include <stddef.h>

/* Función destructora de los elementos del array,
 * toma su ubicación y libera su memoria
 */
typedef void (*DArrayDestructor)(void* data);

typedef struct _DArray* DArray;

/* Crea un array dinámico con elementos
 * cuyo tamaño en bytes es data_size
 */
DArray d_array_make(size_t data_size);

/* Obtiene un puntero al elemento en la posición pos del array.
 * Pedir una posición inexistente resulta en comportamiento
 * no definido
 */
void* d_array_get(DArray array, size_t pos);

/* Inserta una copia del elemento alojado
 * en la dirección data al final del array
 */
DArray d_array_insert(DArray array, const void* data);

/* Borra el último elemento del array, y usa el argumento
 * opcional destructor si es necesario liberar memoria de los elementos.
 * Borrar cuando no hay elementos resulta en comportamiento no definido
 */
DArray d_array_remove(DArray array, DArrayDestructor destructor);

/* Borra todos los elementos del array, y usa el argumento
 * opcional destructor si es necesario liberar memoria de los elementos
 */
DArray d_array_empty(DArray array, DArrayDestructor destructor);

/* Retorna la dirección del contenedor de datos del array,
 * se garantiza que todos los elementos están contiguos en memoria
 */
const void* d_array_as_array(DArray array);

/* Retorna la cantidad de elementos en el array
 */
size_t d_array_size(DArray array);

/* Libera la memoria del array si se ha dejado de usar, y usa el
 * argumento opcional destructor si es necesario liberar memoria
 * de los elementos del array
 */
void d_array_free(DArray array, DArrayDestructor destructor);