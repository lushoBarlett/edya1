#include "action_stack.h"

ActionStack actions_make() {
	ActionStack actions;
	actions.stack_first = 0;
	actions.stack_pointer = 0;
	actions.redo_pointer = 0;

	for (size_t i = 0; i < MAX_UNDO; i++)
		actions.stack[i] = action_freed();

	for (size_t i = 0; i < MAX_UNDO; i++)
		actions.redo[i] = action_freed();

	return actions;
}

/*
 * Dado un índice, retorna el anterior,
 * dando la vuelta al buffer si es necesario
 */
static size_t prev(size_t i) {
	return i == 0 ? MAX_UNDO - 1 : i - 1;
}

/*
 * Dado un índice, retorna el siguiente,
 * dando la vuelta al buffer si es necesario
 */
static size_t next(size_t i) {
	return i == MAX_UNDO - 1 ? 0 : i + 1;
}

/*
 * Libera la memoria de un buffer
 */
static void buffer_free(Action* buffer) {
	for (size_t i = 0; i < MAX_UNDO; i++)
		action_free(&buffer[i]);
}

/*
 * Mueve una acción al tope de la pila de acciones
 * que se pueden rehacer.
 * El comportamiento si la pila está llena no está definido
 */
static void redo_push(ActionStack* actions, Action* action) {
	actions->redo[actions->redo_pointer] = action_move(action);
	actions->redo_pointer++;
}

/*
 * Saca una acción del tope de la pila de acciones
 * que se pueden rehacer.
 * El comportamiento si la pila está vacía no está definido
 */
static Action redo_pop(ActionStack* actions) {
	actions->redo_pointer--;
	return action_move(&actions->redo[actions->redo_pointer]);
}

void actions_push(ActionStack* actions, Action* data) {
	Action* current = &actions->stack[actions->stack_pointer];

	// Estamos por pisar una acción,
	// significa que dimos la vuelta
	if (current->type != Error) {
		action_free(current);
		actions->stack_first = next(actions->stack_first);
	}

	actions->stack[actions->stack_pointer] = action_move(data);
	actions->stack_pointer = next(actions->stack_pointer);

	// Liberamos la pila de acciones para rehacer,
	// esas acciones no se pueden recuperar
	buffer_free(actions->redo);
	actions->redo_pointer = 0;
}

Action actions_undo(ActionStack* actions) {
	size_t p = prev(actions->stack_pointer);
	
	// No hay acción anterior,
	// no podemos retroceder más
	if (actions->stack[p].type == Error)
		return action_freed();

	actions->stack_pointer = p;
	Action undo = action_copy(&actions->stack[p]);
	redo_push(actions, &actions->stack[p]);
	return undo;
}

Action actions_redo(ActionStack* actions) {
	// No hay acciones para rehacer
	if (!actions->redo_pointer)
		return action_freed();
	
	// Como hay acciones para rehacer
	// la pila no está llena
	actions->stack[actions->stack_pointer] = redo_pop(actions);
	Action redo = action_copy(&actions->stack[actions->stack_pointer]);
	actions->stack_pointer = next(actions->stack_pointer);
	return redo;
}

void actions_free(ActionStack* actions) {
	buffer_free(actions->stack);
	buffer_free(actions->redo);
}