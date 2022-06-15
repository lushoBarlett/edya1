#include "test_contact.h"

#include <string.h>
#include <assert.h>

#include "../src/contact.h"
#include "utils.h"

/*
 * Prueba los contactos liberados, la liberación de contactos,
 * el predicado de contactos liberados y que se pueda liberar
 * un contacto liberado de forma segura
 */
static void test_free_conditions() {
	Contact c = contact_freed();
	assert(contact_is_freed(&c));
	contact_free(&c);
	assert(contact_is_freed(&c));

	c = utils_contact("test", "test", 5, "123");
	assert(!contact_is_freed(&c));

	contact_free(&c);
	assert(contact_is_freed(&c));
}

/*
 * Prueba todas las funciones de comparación de contactos
 */
static void test_comparisons() {
	Contact a = utils_contact("test", "test", 5, "123");
	Contact b = utils_contact("test", "vest", 2, "3");

	assert(!contact_equals(&a, &b));
	assert(contact_comp_name(&a, &b) == 0);
	assert(contact_comp_surname(&a, &b) == -1);
	assert(contact_comp_age(&a, &b) == 1);
	assert(contact_comp_phone(&a, &b) == -1);

	Contact equals = contact_copy(&a);
	assert(contact_equals(&a, &equals));

	contact_free(&a);
	contact_free(&b);
	contact_free(&equals);
}

/*
 * Prueba las representaciones de strings, tanto la normal como la CSV
 */
static void test_strings() {
	Contact a = utils_contact("test", "test", 5, "123");

	String normal = contact_string(&a);
	assert(!strcmp(normal.c_str, "{test, test, 5, 123}"));
	string_free(&normal);

	String comma = contact_comma_string(&a);
	assert(!strcmp(comma.c_str, "test,test,5,123"));
	string_free(&comma);

	contact_free(&a);
}

/*
 * Prueba copiar y mover contactos
 */
static void test_copy_and_move() {
	Contact c = utils_contact("test", "test", 5, "123");

	Contact copy = contact_copy(&c);
	assert(!contact_is_freed(&c));
	assert(!contact_is_freed(&copy));

	Contact move = contact_move(&c);
	assert(contact_is_freed(&c));
	assert(!contact_is_freed(&move));

	contact_free(&copy);
	contact_free(&move);
}

void test_contact() {
	test_free_conditions();
	test_comparisons();
	test_strings();
	test_copy_and_move();
}