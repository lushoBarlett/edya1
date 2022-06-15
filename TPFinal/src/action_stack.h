#pragma once

#include "action.h"

/*
 * Cantidad máxima de veces acciones de
 * inserción, borrado y reemplazo, que podemos retrodecer
 */
#define MAX_UNDO 10

/*
 * Una ActionStack es una pila de acciones, que podemos deshacer
 * y rehacer hasta un máximo de MAX_UNDO.
 * Tiene dos buffers, una pila circular principal, que pisa las acciones
 * viejas para colocar acciones nuevas, y mantiene un tamaño fijo,
 * y un stack de acciones que todavía se pueden rehacer.
 * 
 * stack_first: índice de la acción más vieja
 * stack_pointer: índice que le sigue a la última acción realizada
 * redo_pointer: índice que le sigue a la última acción deshecha
 * (y que puede ser rehecha)
 * stack: stack principal
 * redo: stack de acciones deshechas
 */
typedef struct {
	size_t stack_first;
	size_t stack_pointer;
	size_t redo_pointer;
	Action stack[MAX_UNDO];
	Action redo[MAX_UNDO];
} ActionStack;

/*
 * Crea una pila de acciones vacía
 */
ActionStack actions_make();

/*
 * Inserta una nueva acción en la pila.
 * Si había acciones deshechas, estas ya no se pueden rehacer
 */
void actions_push(ActionStack* actions, Action* action);

/*
 * Deshace un acción de la pila y la retorna.
 * Si no existe acción anterior, se retorna una acción de tipo Error
 */
Action actions_undo(ActionStack* actions);

/*
 * Rehace un acción de la pila y la retorna.
 * Si no existe acción siguiente, se retorna una acción de tipo Error
 */
Action actions_redo(ActionStack* actions);

/*
 * Libera la memoria de una pila de acciones
 */
void actions_free(ActionStack* actions);