#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "expression.h"
#include "operator_types.h"
#include "operators.h"
#include "declarations.h"
#include "dynamic_array.h"

#define MAX_LINE 100

/* Modelamos una tabla de operadores como un array dinámico
 * que contiene Operator, una estructura que cuenta con 3 campos:
 * - la representación como string del operador
 * - la aridad (cantidad de argumentos)
 * - la implementación del operador
 */

/* Imprime el mensaje de inicio de la interfaz de usuario
 */
void welcome() {
	printf("Interfaz de Consola del Evaludor de Expresiones\n");
}

/* Pide input del usuario y lo guarda en el buffer
 * que se le provee como argumento
 */
void prompt(char* const buffer) {
	printf("> ");
	fgets(buffer, MAX_LINE, stdin);
	fflush(stdin);
}

/* Dado un string que representa el nombre de una variable
 * de la interfaz de usuario, imprime el mensaje de que la variable no existe
 */
void undeclared(const char* identifier) {
	printf("Error: La variable %s no existe\n", identifier);
}

/* Función principal de la interfaz de usuario.
 * Toma una array de operadores como argumento
 */
void interpret(DArray op_table) {
	char input[MAX_LINE];
	DArray decls = d_array_make(sizeof(Declaration));

	welcome();

	while(1) {
		prompt(input);

		Statement stmt = parse_statement(input, op_table);
		Expression expr;

 		switch (stmt.type) {
		case Quit:
			printf("Saliendo del programa\n");
			statement_free(stmt);
			d_array_free(decls, declaration_free);
			return;
		case Eval:
			expr = declaration_find(decls, stmt.identifier);
			if (!expression_is_empty(expr))
				printf("%d\n", expression_eval(expr));
			else
				undeclared(stmt.identifier);
			statement_free(stmt);
			break;
		case Print:
			expr = declaration_find(decls, stmt.identifier);
			if (!expression_is_empty(expr)) {
				expression_print_infix(expr);
				printf("\n");
			} else {
				undeclared(stmt.identifier);
			}
			statement_free(stmt);
			break;
		case Assign:
			// NOTA: no liberamos la memoria del Statement en esta parte
			// ya que las declaraciones se adueñan de esa memoria
			decls = declarations_assign(decls, stmt.identifier, stmt.expr);
			break;
		case Error:
			break;
		default:
			// NOTA: el programa nunca debería llegar a esta parte
			printf("Error: operación no definida\n");
		}
	}
}

/* Inserta un operador (operator) en el array de operadores (op_table)
 */
DArray operator_insert(DArray op_table, Operator operator) {
	return d_array_insert(op_table, &operator);
}

/* Punto de entrada del programa
 */
int main() {
	DArray op_table = d_array_make(sizeof(Operator));

	op_table = operator_insert(op_table, (Operator){"+",  2, sum_impl});
	op_table = operator_insert(op_table, (Operator){"-",  2, sub_impl});
	op_table = operator_insert(op_table, (Operator){"--", 1, neg_impl});
	op_table = operator_insert(op_table, (Operator){"*",  2, mul_impl});
	op_table = operator_insert(op_table, (Operator){"/",  2, div_impl});
	op_table = operator_insert(op_table, (Operator){"%",  2, mod_impl});
	op_table = operator_insert(op_table, (Operator){"^",  2, pow_impl});

	interpret(op_table);

	d_array_free(op_table, NULL);

	return 0;
}