#include "test_algorithms.h"

#include <assert.h>

#include "../src/algorithms.h"
#include "utils.h"

#include <stdio.h>

/*
 * Prueba el algoritmo de consulta, tanto la consulta
 * que matchea con todos los campos, como la que matchea
 * con alguno de los campos
 */
void test_query() {
	ContactArray contacts = utils_contact_array();
	ContactArray answer;
	// Copias para hacer comparaciones
	ContactArray copies = contact_array_copy(&contacts);
	
	Contact query = utils_contact("", "b", 5, "");

	answer = algorithms_query(&contacts, &query, true);
	assert(answer.size == 2);
	assert(contact_equals(&answer.array[0], &contacts.array[0]));
	assert(contact_equals(&answer.array[1], &contacts.array[3]));
	contact_array_free(&answer);

	answer = algorithms_query(&contacts, &query, false);
	assert(answer.size == 4);
	assert(contact_equals(&answer.array[0], &contacts.array[0]));
	assert(contact_equals(&answer.array[1], &contacts.array[1]));
	assert(contact_equals(&answer.array[2], &contacts.array[2]));
	assert(contact_equals(&answer.array[3], &contacts.array[3]));
	contact_array_free(&answer);

	contact_free(&query);
	query = utils_contact("c", "", MAX_UINT, "123");

	answer = algorithms_query(&contacts, &query, true);
	assert(answer.size == 0);
	contact_array_free(&answer);

	answer = algorithms_query(&contacts, &query, false);
	assert(answer.size == 5);
	assert(contact_equals(&answer.array[0], &contacts.array[0]));
	assert(contact_equals(&answer.array[1], &contacts.array[1]));
	assert(contact_equals(&answer.array[2], &contacts.array[2]));
	assert(contact_equals(&answer.array[3], &contacts.array[4]));
	assert(contact_equals(&answer.array[4], &contacts.array[5]));
	contact_array_free(&answer);

	contact_free(&query);

	contact_array_free(&contacts);
	contact_array_free(&copies);
}

/*
 * Prueba el algoritmo de ordenamiento de arrays de contactos
 * para los cuatro tipos de ordenamientos posibles
 */
static void test_sort() {
	ContactArray contacts = utils_contact_array();

	algorithms_sort_by(&contacts, Name);
	for (size_t i = 1; i < contacts.size; i++)
		assert(string_comp(&contacts.array[i - 1].name, &contacts.array[i].name) <= 0);

	algorithms_sort_by(&contacts, Surname);
	for (size_t i = 1; i < contacts.size; i++)
		assert(string_comp(&contacts.array[i - 1].surname, &contacts.array[i].surname) <= 0);

	algorithms_sort_by(&contacts, Age);
	for (size_t i = 1; i < contacts.size; i++)
		assert(contacts.array[i - 1].age <= contacts.array[i].age);

	algorithms_sort_by(&contacts, Phone);
	for (size_t i = 1; i < contacts.size; i++)
		assert(string_comp(&contacts.array[i - 1].phone, &contacts.array[i].phone) <= 0);

	contact_array_free(&contacts);
}

/*
 * Prueba el algoritmo de subconjunto de contactos
 * de acuerdo a una suma total de edades
 */
static void test_subset_sum() {
	ContactArray contacts = utils_contact_array();
	ContactArray answer;
	
	// El algoritmo inserta los elementos
	// al revés, y chequea en el orden del array.
	// La primer solución válida es la que se retorna.
	answer = algorithms_subset_sum(&contacts, 5);
	assert(answer.size == 1);
	assert(contact_equals(&answer.array[0], &contacts.array[0]));
	contact_array_free(&answer);

	answer = algorithms_subset_sum(&contacts, 10);
	assert(answer.size == 2);
	assert(contact_equals(&answer.array[0], &contacts.array[1]));
	assert(contact_equals(&answer.array[1], &contacts.array[0]));
	contact_array_free(&answer);

	answer = algorithms_subset_sum(&contacts, 13);
	assert(answer.size == 3);
	assert(contact_equals(&answer.array[0], &contacts.array[5]));
	assert(contact_equals(&answer.array[1], &contacts.array[4]));
	assert(contact_equals(&answer.array[2], &contacts.array[0]));
	contact_array_free(&answer);

	answer = algorithms_subset_sum(&contacts, 33);
	assert(answer.size == 6);
	assert(contact_equals(&answer.array[0], &contacts.array[5]));
	assert(contact_equals(&answer.array[1], &contacts.array[4]));
	assert(contact_equals(&answer.array[2], &contacts.array[3]));
	assert(contact_equals(&answer.array[3], &contacts.array[2]));
	assert(contact_equals(&answer.array[4], &contacts.array[1]));
	assert(contact_equals(&answer.array[5], &contacts.array[0]));
	contact_array_free(&answer);

	answer = algorithms_subset_sum(&contacts, 0);
	assert(answer.size == 0);
	contact_array_free(&answer);

	answer = algorithms_subset_sum(&contacts, 34);
	assert(answer.size == 0);
	contact_array_free(&answer);

	contact_array_free(&contacts);
}

void test_algorithms() {
	test_query();
	test_sort();
	test_subset_sum();
}