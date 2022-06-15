#include "test_contact_book.h"

#include <string.h>
#include <assert.h>

#include "../src/contact_book.h"
#include "utils.h"

/*
 * Inserta en un array de contactos INSERTION_COUNT contactos,
 * estos contactos tienen nombre y apellido de una letra, y cambia la letra
 */
#define INSERTION_COUNT 26
static void insertions(ContactArray* array) {
	for (size_t i = 0; i < INSERTION_COUNT; i++) {
		Contact c = utils_shortc('a' + i, 'a' + i);
		contact_array_push_back(array, &c);
	}
}

/*
 * Prueba las operaciones de inserción, búsqueda, borrado, reemplazo
 */
static void test_basic_operations() {
	Contact insert = utils_contact("a", "b", 1, "1");
	ContactBook contacts = contacts_make();

	assert(!contacts_find(&contacts, &insert));

	contacts_insert(&contacts, &insert);
	assert(contact_is_freed(&insert));

	Contact find = utils_contact("a", "b", MAX_UINT, NULL);
	assert(contacts_find(&contacts, &find));

	Contact replace = utils_contact("a", "b", 2, "2");
	contacts_replace(&contacts, &replace);
	const Contact* found = contacts_find(&contacts, &find);
	assert(found);
	assert(found->age == 2);

	contacts_erase(&contacts, &find);
	assert(!contacts_find(&contacts, &find));

	contact_free(&find);
	contacts_free(&contacts);
}

/*
 * Prueba la creación de un array de contactos que tiene
 * todos los elementos del diccionario
 */
static void test_array() {
	ContactArray insert = contact_array_make();
	ContactArray copies = contact_array_make();
	ContactBook contacts = contacts_make();

	insertions(&insert);
	insertions(&copies);

	// Insertar
	for (size_t i = 0; i < INSERTION_COUNT; i++)
		contacts_insert(&contacts, &insert.array[i]);

	ContactArray elements = contacts_array(&contacts);

	for (size_t i = 0; i < INSERTION_COUNT; i++)
		assert(contacts_find(&contacts, &elements.array[i]));

	contact_array_free(&elements);
	contact_array_free(&insert);
	contact_array_free(&copies);
	contacts_free(&contacts);
}

/*
 * Prueba las operaciones básicas, pero insertando y reinsertando muchos elementos
 */
static void test_intense_use() {
	ContactArray insert = contact_array_make();
	ContactArray copies = contact_array_make();
	ContactBook contacts = contacts_make();

	insertions(&insert);
	insertions(&copies);

	// Insertar
	for (size_t i = 0; i < INSERTION_COUNT; i++)
		contacts_insert(&contacts, &insert.array[i]);

	// Buscar
	for (size_t i = 0; i < INSERTION_COUNT; i++)
		assert(contact_equals(contacts_find(&contacts, &copies.array[i]), &copies.array[i]));

	// Borrar
	for (size_t i = 0; i < INSERTION_COUNT; i++)
		contacts_erase(&contacts, &copies.array[i]);

	// Buscar falla
	for (size_t i = 0; i < INSERTION_COUNT; i++)
		assert(!contacts_find(&contacts, &copies.array[i]));

	// Reinsertar
	for (size_t i = 0; i < INSERTION_COUNT; i++)
		contacts_insert(&contacts, &copies.array[i]);

	assert(contacts.size == INSERTION_COUNT);
	contacts_free(&contacts);
	contact_array_free(&insert);
	contact_array_free(&copies);
}

void test_contact_book() {
	test_basic_operations();
	test_array();
	test_intense_use();
}