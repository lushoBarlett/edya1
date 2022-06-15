#include "test_contact_array.h"

#include <string.h>
#include <assert.h>

#include "../src/contact_array.h"
#include "utils.h"

/*
 * Prueba las representaciones de strings, tanto la normal como la CSV
 */
static void test_strings() {
	Contact a = utils_contact("a", "a", 5, "123");
	Contact b = utils_contact("b", "b", 3, "321");
	Contact c = utils_contact("c", "c", 10, "456");

	ContactArray array = contact_array_make();
	contact_array_push_back(&array, &a);
	contact_array_push_back(&array, &b);
	contact_array_push_back(&array, &c);

	String normal = contact_array_string(&array);
	assert(!strcmp(normal.c_str,
		"{a, a, 5, 123}\n"
		"{b, b, 3, 321}\n"
		"{c, c, 10, 456}\n"
	));
	string_free(&normal);

	String comma = contact_array_comma_string(&array);
	assert(!strcmp(comma.c_str,
		"a,a,5,123\n"
		"b,b,3,321\n"
		"c,c,10,456\n"
	));
	string_free(&comma);

	contact_array_free(&array);
}

/*
 * Prueba la copia de array de contactos
 */
static void test_copy() {
	Contact a = utils_contact("a", "a", 5, "123");
	Contact b = utils_contact("b", "b", 3, "321");
	Contact c = utils_contact("c", "c", 10, "456");

	ContactArray array = contact_array_make();
	contact_array_push_back(&array, &a);
	contact_array_push_back(&array, &b);
	contact_array_push_back(&array, &c);

	ContactArray copy = contact_array_copy(&array);
	assert(contact_equals(&array.array[0], &copy.array[0]));
	assert(contact_equals(&array.array[1], &copy.array[1]));
	assert(contact_equals(&array.array[2], &copy.array[2]));

	contact_array_free(&copy);
	contact_array_free(&array);
}

void test_contact_array() {
	test_strings();
	test_copy();
}