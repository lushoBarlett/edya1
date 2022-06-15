#include "algorithms.h"

#include <assert.h>

ContactArray algorithms_query(const ContactArray* contacts, const Contact* query, bool strict) {
	ContactArray answer = contact_array_make();

	for (size_t i = 0; i < contacts->size; i++) {
		const Contact* c = &contacts->array[i];

		bool accepted;
		if (strict)
			// Campos vacíos evalúan a true (elemento neutro de &&)
			accepted =
			(!query->name.length    || !string_comp(&query->name, &c->name)) &&
			(!query->surname.length || !string_comp(&query->surname, &c->surname)) &&
			(query->age == MAX_UINT || query->age == c->age) &&
			(!query->phone.length   || !string_comp(&query->phone, &c->phone));
		else
			// Campos vacíos evalúan a false (elemento neutro de ||)
			accepted =
			(query->name.length     && !string_comp(&query->name, &c->name)) ||
			(query->surname.length  && !string_comp(&query->surname, &c->surname)) ||
			(query->age != MAX_UINT && query->age == c->age) ||
			(query->phone.length    && !string_comp(&query->phone, &c->phone));

		if (accepted) {
			Contact copy = contact_copy(c);
			contact_array_push_back(&answer, &copy);
		}
	}

	return answer;
}

/*
 * Rellena la tabla de soluciones con:
 * - 1, si existe algún subconjunto que empiece en (index) y que sume (sum)
 * - 0, en caso contrario
 */
static bool subset(signed char** DP, const ContactArray* contacts, size_t index, int sum) {
	// Llegamos a una solución
	if (sum == 0)
		return true;

	// Fuera de rango o solución inválida
	if (index >= contacts->size || sum < 0)
		return false;

	// Ya fue calculado
	if (DP[index][sum] != -1)
		return DP[index][sum];

	// Elegimos tomar el contacto, si no produce solución válida
	// elegimos no tomar el contacto, y esa será la respuesta
	return (DP[index][sum] =
	subset(DP, contacts, index + 1, sum - contacts->array[index].age) ||
	subset(DP, contacts, index + 1, sum));
}

/*
 * Recorre el camino marcado con 1s en la tabla de soluciones
 * y construye el subconjunto solución que determina.
 */
static ContactArray* reconstruct(signed char** DP, const ContactArray* contacts, size_t index, int sum) {
	// Llegamos a una solución
	if (sum == 0) {
		ContactArray* answer = malloc(sizeof(*contacts));
		assert(answer);
		*answer = contact_array_make();
		return answer;
	}

	// Fuera de rango o solución inválida
	if (index >= contacts->size || sum < 0 || !DP[index][sum])
		return NULL;

	// Elegimos tomar el contacto, si no produce solución válida
	// elegimos no tomar el contacto, y esa será la respuesta.
	// Como recorremos en el mismo orden que en la función subset, está
	// garantizado que recorreremos exáctamente las mismas opciones en el mismo orden
	ContactArray* answer = reconstruct(DP, contacts, index + 1, sum - contacts->array[index].age);
	if (answer) {
		Contact copy = contact_copy(&contacts->array[index]);
		contact_array_push_back(answer, &copy);
	} else {
		answer = reconstruct(DP, contacts, index + 1, sum);
	}

	return answer;
}

ContactArray algorithms_subset_sum(const ContactArray* contacts, unsigned sum) {
	// DP creado en el heap para evitar stack overflow
	signed char** DP = malloc(sizeof(*DP) * contacts->size);
	assert(DP);

	for (size_t i = 0; i < contacts->size; i++) {
		DP[i] = malloc(sizeof(**DP) * (sum + 1));
		assert(DP[i]);
		for (size_t j = 0; j <= sum; j++)
			DP[i][j] = -1;
	}

	subset(DP, contacts, 0, (int)sum);
	ContactArray* array = reconstruct(DP, contacts, 0, (int)sum);

	// Obtenemos el valor para liberar el puntero
	ContactArray answer = array ? *array : contact_array_make();
	free(array);

	for (size_t i = 0; i < contacts->size; i++)
		free(DP[i]);
	free(DP);

	return answer;
}

/*
 * Tipo de la función de ordenamiento que acepta qsort
 */
typedef int (*QSortOrder)(const void*, const void*);

void algorithms_sort_by(ContactArray* contacts, Field order) {
	QSortOrder comparator;

	switch (order) {
	case Name:    comparator = (QSortOrder)contact_comp_name; break;
	case Surname: comparator = (QSortOrder)contact_comp_surname; break;
	case Age:     comparator = (QSortOrder)contact_comp_age; break;
	case Phone:   comparator = (QSortOrder)contact_comp_phone; break;
	case NoField: assert(0);
	}

	qsort(contacts->array, contacts->size, sizeof(*contacts->array), comparator);
}