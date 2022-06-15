#include "test_csv.h"

#include <assert.h>
#include <stdio.h>

#include "../src/csv.h"
#include "utils.h"

/*
 * Prueba el guardado y la carga de contactos a archivos
 */
void test_load_and_save() {
	ContactArray contacts = utils_contact_array();
	String filename = string_empty();
	string_append(&filename, "./test.csv");

	csv_save(&filename, &contacts);
	ContactArray loaded = csv_load(&filename);

	assert(contacts.size == loaded.size);
	for (size_t i = 0; i < contacts.size; i++)
		assert(contact_equals(&contacts.array[i], &loaded.array[i]));

	remove(filename.c_str);
	contact_array_free(&loaded);
	contact_array_free(&contacts);
	string_free(&filename);
}

void test_csv() {
	test_load_and_save();
}