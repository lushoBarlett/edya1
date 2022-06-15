#pragma once

#include "expression.h"
#include "dynamic_array.h"

/* Una Declaración es un nombre ligado a una expresión aritmética,
 * guardamos el nombre en un string en (identifier) y la expresión
 * en un Expression en (expr)
 */
typedef struct {
	char* identifier;
	Expression expr;
} Declaration;

/* Dada una lista de declaraciones, una variable y una expresión,
 * si el identificador existe en la lista, reemplaza la expresión
 * y libera la memoria del identificador provisto.
 * Sino, crea un nuevo nodo con el identificador, ligado a esa expresión
 */
DArray declarations_assign(DArray decls, char* identifier, Expression expr);

/* Retorna la expresión asignada a un nombre
 * Si el nombre no es encontrado, retorna una expresión vacía
 */
Expression declaration_find(DArray decls, const char* identifier);

/* Libera la memoria de una declaración, o sea
 * el identificador y la expresión
 */
void declaration_free(void* decl);