#include "test_action_stack.h"

#include <assert.h>

#include "../src/action_stack.h"
#include "utils.h"

/*
 * Prueba la creación y liberación de la pila de acciones.
 * Prueba que se puedan insertar más del máximo, que se pueda
 * retroceder y avanzar exitósamente hasta el máximo permitido,
 * y que no se puedan rehacer acciones si se realizó una nueva
 */
static void test_operations() {
	ActionStack actions = actions_make();
	Contact c = utils_contact("test", "test", 5, "123");
	Action a = action_insertion(&c);
	
	// Inserta 5 más que el límite
	for (size_t i = 0; i < MAX_UNDO + 5; i++) {
		Action copy = action_copy(&a);
		actions_push(&actions, &copy);
	}

	// Vuelve el límite de veces
	for (size_t i = 0; i < MAX_UNDO; i++) {
		Action undo = actions_undo(&actions);
		assert(undo.type == Insert);
		action_free(&undo);
	}

	assert(actions_undo(&actions).type == Error);

	// Avanza el límite de veces
	for (size_t i = 0; i < MAX_UNDO; i++) {
		Action redo = actions_redo(&actions);
		assert(redo.type == Insert);
		action_free(&redo);
	}

	assert(actions_redo(&actions).type == Error);

	// Insertar reinicia las acciones
	for (size_t i = 0; i < MAX_UNDO; i++) {
		Action undo = actions_undo(&actions);
		action_free(&undo);
	}

	Action copy = action_copy(&a);
	actions_push(&actions, &copy);
	assert(actions_redo(&actions).type == Error);

	Action undo = actions_undo(&actions);
	assert(undo.type == Insert);
	action_free(&undo);

	action_free(&a);
	actions_free(&actions);
}

void test_action_stack() {
	test_operations();
}