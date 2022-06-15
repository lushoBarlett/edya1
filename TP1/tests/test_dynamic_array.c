#include "test_dynamic_array.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "../src/dynamic_array.h"

/* Datos de prueba, contiene con string
 */
typedef struct {
	char* string;
} Data;

/* Crea un Data con nueva memoria reservada
 * para una copia del string argumento
 */
static Data test_data(const char* string) {
	Data d;
	d.string = malloc(sizeof(char) * (strlen(string) + 1));
	assert(d.string);
	strcpy(d.string, string);
	return d;
}

/* Función destrutora de Data
 * NOTA: aseguramos que los Data no contienen los mismos
 * punteros para no liberar la memoria dos veces
 */
static void data_destructor(void* data) {
	Data* casted = (Data*)data;
	free(casted->string);
}

/* Prueba todas las operaciones básicas en conjunto
 * - d_array_make: crea un array
 * - d_array_insert: inserta un elemento
 * - d_array_get: obtiene un elemento
 * - d_array_size: obtiene la cantidad de elementos
 * - d_array_free: libera la memoria del array
 */
static void test_basic_operations() {
	DArray array = d_array_make(sizeof(Data));
	
	Data element = test_data("test");
	array = d_array_insert(array, &element);

	Data* retrieved = d_array_get(array, 0);

	assert(!strcmp(element.string, retrieved->string));
	assert(d_array_size(array) == 1);

	d_array_free(array, data_destructor);
}


/* Prueba la liberación de memoria y achicamiento del tamaño
 * del array al llamar a la función d_array_empty
 */
static void test_empty() {
	DArray array = d_array_make(sizeof(Data));
	
	Data element;
	element = test_data("test1");
	array = d_array_insert(array, &element);
	element = test_data("test2");
	array = d_array_insert(array, &element);

	assert(d_array_size(array) == 2);

	array = d_array_empty(array, data_destructor);

	assert(d_array_size(array) == 0);

	// no hay elementos y el destructor no es necesario
	d_array_free(array, NULL);
}

/* Prueba que el bloque de memoria retornado por la función
 * d_array_as_array tenga los elementos previamente insertados
 */
static void test_as_array() {
	DArray array = d_array_make(sizeof(Data));
	
	Data element;
	element = test_data("test1");
	array = d_array_insert(array, &element);
	element = test_data("test2");
	array = d_array_insert(array, &element);

	const Data* raw_data = d_array_as_array(array);
	assert(!strcmp(raw_data[0].string, "test1"));
	assert(!strcmp(raw_data[1].string, "test2"));

	d_array_free(array, data_destructor);
}

void test_dynamic_array() {
	test_basic_operations();
	test_empty();
	test_as_array();
	printf("dynamic_array tests - OK\n");
}