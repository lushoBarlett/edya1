#pragma once

#include "contact.h"

/*
 * Un ActionType es un tipo de acción realizable por el usuario.
 * Estas acciones incluyen una acción fallida llamada Error.
 */
typedef enum {
	Error,
	Search,
	Insert,
	Erase,
	Replace,
	Load,
	Save,
	Undo,
	Redo,
	And,
	Or,
	OrderedSave,
	SumSearch,
	Quit,
} ActionType;

/*
 * Un Action representa una de tres acciones que se pueden
 * deshacer y rehacer. Estas son insertar, borrar y reemplazar.
 * Los reemplazos son equivalentes a una inserción seguida de un borrado.
 * 
 * type: el tipo de acción
 * added: el contacto que se insertó, contacto liberado si no se insertó nada
 * deleted: el contacto que se borró, contacto liberado si no se borró nada
 */
typedef struct {
	ActionType type;
	Contact added;
	Contact deleted;
} Action;

/*
 * Crea una acción con memoria liberada,
 * ésta cuenta con un tipo Error y contactos liberados.
 * Las acciones liberadas son seguras de liberar
 */
Action action_freed();

/*
 * Predicado para saber si una acción tiene memoria liberada
 */
bool action_is_freed(const Action* action);

/*
 * Copia una acción
 */
Action action_copy(const Action* action);

/*
 * Retorna la misma acción, marcando el argumento como liberado
 */
Action action_move(Action* action);

/*
 * Crea una nueva acción de inserción con el contacto proporcionado,
 * marcando el mismo como liberado
 */
Action action_insertion(Contact* insertion);

/*
 * Crea una nueva acción de borrado con el contacto proporcionado,
 * marcando el mismo como liberado
 */
Action action_erasure(Contact* erasure);

/*
 * Crea una nueva acción de reemplazo con los contactos proporcionados,
 * marcandolos como liberados. El primer argumento es la inserción y el
 * segundo es el borrado
 */
Action action_replacement(Contact* insertion, Contact* erasure);

/*
 * Libera la memoria de una acción y la marca como liberada
 */
void action_free(Action* action);