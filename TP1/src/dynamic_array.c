#include "dynamic_array.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* Un array dinámico guarda 4 cosas
 * 1. data: La dirección del contenedor de datos
 * 2. size: La cantidad de elementos del array
 * 3. storage_size: La cantidad de elementos que el bloque de memoria * actual (data) puede alojar
 * 4. data_size: El tamaño en bytes que ocupan los elementos del array
 */
struct _DArray {
	void* data;
	size_t size;
	size_t storage_size;
	size_t data_size;
};

DArray d_array_make(size_t data_size) {
	DArray array = malloc(sizeof(struct _DArray));
	assert(array);
	array->data = NULL;
	array->size = 0;
	array->storage_size = 0;
	array->data_size = data_size;
	return array;
}

void* d_array_get(DArray array, size_t pos) {
	return array->data + pos * array->data_size;
}

DArray d_array_insert(DArray array, const void* data) {
	if (array->size == array->storage_size) {
		array->storage_size = array->storage_size ? array->storage_size * 2 : 1;
		void* new_data = realloc(array->data, array->storage_size * array->data_size);
		assert(new_data);
		array->data = new_data;
	}

	memcpy(array->data + array->size * array->data_size, data, array->data_size);
	array->size++;
	return array;
}

DArray d_array_remove(DArray array, DArrayDestructor destructor) {
	if (destructor)
		destructor(d_array_get(array, array->size - 1));
	array->size--;
	return array;
}

DArray d_array_empty(DArray array, DArrayDestructor destructor) {
	if (destructor)
		for (size_t i = 0; i < array->size; i++)
			destructor(d_array_get(array, i));
	array->size = 0;
	return array;
}

const void* d_array_as_array(DArray array) {
	return array->data;
}

size_t d_array_size(DArray array) {
	return array->size;
}

void d_array_free(DArray array, DArrayDestructor destructor) {
	if (destructor)
		array = d_array_empty(array, destructor);
	free(array->data);
	free(array);
}