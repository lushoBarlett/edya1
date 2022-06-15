#include "test_string.h"

#include <string.h>
#include <assert.h>

#include "../src/string.h"

/*
 * Prueba los string liberados, la liberación de strings,
 * el predicado de strings liberados y que se pueda liberar
 * un string liberado de forma segura
 */
static void test_free_conditions() {
	String s = string_freed();
	assert(string_is_freed(&s));
	string_free(&s);
	assert(string_is_freed(&s));

	s = string_empty();
	assert(!string_is_freed(&s));

	string_free(&s);
	assert(string_is_freed(&s));
}

/*
 * Prueba la creación de strings a partir de
 * la inserción de uno o varios caracteres
 */
static void test_create_from_const() {
	String s = string_empty();
	string_push_back(&s, 'a');
	string_push_back(&s, 'b');
	string_append(&s, "cd.");

	assert(!strcmp(s.c_str, "abcd."));
	string_free(&s);
}

/*
 * Prueba copiar y mover strings, además de
 * las operaciones como crear a partir de un entero,
 * dar vuelta y concatenar
 */
static void test_create_from_operations() {
	String s1 = string_from_uint(12345);
	String s2 = string_copy(&s1);
	assert(!string_is_freed(&s1));
	assert(!string_comp(&s1, &s2));

	String s3 = string_move(&s2);
	assert(string_is_freed(&s2));

	string_reverse(&s3);

	String s4 = string_concat(&s1, &s3);
	assert(string_is_freed(&s1));
	assert(string_is_freed(&s3));

	assert(!strcmp(s4.c_str, "1234554321"));
	string_free(&s4);
}

/*
 * Prueba que dar vuelta un string vacío es el string vacío
 */
static void test_reverse_empty() {
	String s = string_empty();
	string_reverse(&s);
	assert(!strcmp(s.c_str, ""));
	string_free(&s);
}

void test_string() {
	test_free_conditions();
	test_create_from_const();
	test_create_from_operations();
	test_reverse_empty();
}