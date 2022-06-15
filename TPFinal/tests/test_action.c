#include "test_action.h"

#include <string.h>
#include <assert.h>

#include "../src/action.h"
#include "utils.h"

/*
 * Prueba las acciones liberadas, la liberación de acciones,
 * el predicado de acciones liberadas y que se pueda liberar
 * una acción liberadas de forma segura
 */
static void test_free_conditions() {
	Action a = action_freed();
	assert(action_is_freed(&a));
	action_free(&a);
	assert(action_is_freed(&a));

	Contact c = utils_contact("test", "test", 5, "123");
	a = action_insertion(&c);
	assert(!action_is_freed(&a));

	action_free(&a);
	assert(action_is_freed(&a));
}

/*
 * Prueba la creación de acciones de inserción, borrado y reemplazo
 */
static void test_create_from_operations() {
	Contact i, e;
	Action a;

	i = utils_contact("test", "test", 5, "123");
	a = action_insertion(&i);
	assert(contact_is_freed(&i));
	assert(a.type == Insert);
	assert(!contact_is_freed(&a.added));
	assert(contact_is_freed(&a.deleted));
	action_free(&a);

	e = utils_contact("test", "test", 5, "123");
	a = action_erasure(&e);
	assert(contact_is_freed(&e));
	assert(a.type == Erase);
	assert(contact_is_freed(&a.added));
	assert(!contact_is_freed(&a.deleted));
	action_free(&a);

	i = utils_contact("test", "test", 5, "123");
	e = utils_contact("test", "test", 5, "123");
	a = action_replacement(&i, &e);
	assert(contact_is_freed(&i));
	assert(contact_is_freed(&e));
	assert(a.type == Replace);
	assert(!contact_is_freed(&a.added));
	assert(!contact_is_freed(&a.deleted));
	action_free(&a);
}

/*
 * Prueba copiar y mover acciones
 */
static void test_copy_and_move() {
	Contact c = utils_contact("test", "test", 5, "123");
	Action a = action_insertion(&c);

	Action copy = action_copy(&a);
	assert(!action_is_freed(&a));
	assert(!action_is_freed(&copy));

	Action move = action_move(&a);
	assert(action_is_freed(&a));
	assert(!action_is_freed(&move));

	action_free(&copy);
	action_free(&move);
}

void test_action() {
	test_free_conditions();
	test_copy_and_move();
	test_create_from_operations();
}