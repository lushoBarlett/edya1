#pragma once

#include "expression.h"
#include "dynamic_array.h"

/* Tipos de acciones posibles
 * - Error: acción inválida
 * - Assign: asignar una expresión a un identificador
 * - Print: imprimir la expresión asociada a un identificador
 * - Eval: evaluar la expresión asociada a un identificador
 * - Quit: salir del programa
 */
typedef enum { Error, Assign, Print, Eval, Quit } StmtType;

/* Un Statement (acción) es de alguno de los tipos detallados en StmtType (type).
 * Si la acción contiene información sobre un identificador se guarda en (identifier)
 * y si la acción contiene información sobre una expresión se guarda en (expr)
 */
typedef struct {
	StmtType type;
	char* identifier;
	Expression expr;
} Statement;

/* Interpreta el input, dado un array de los operadores cargados (op_table)
 * y retorna qué acción se desea realizar, o si esa acción no es válida
 */
Statement parse_statement(const char* input, DArray op_table);

/* Libera la memoria de una acción, o sea el identificador y la expresión
 * (si los hubiese)
 */
void statement_free(Statement stmt);
