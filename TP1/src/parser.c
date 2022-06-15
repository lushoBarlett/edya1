#include "parser.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "declarations.h"
#include "dynamic_array.h"
#include "expression.h"

#define COMMON_ERROR "No se pudo interpretar la acción"

/* Un resultado de un parser ParseResult es lo que devuelven las funciones
 * intermedias, todas excepto la última (parse_statement), y su trabajo
 * es pasar información sobre el resultado de dichas operaciones intermedias.
 * 
 * - new_pointer: el puntero de lo que queda del input, si la interpretación
 * tuvo éxito. Si no tuvo éxito su contenido no está definido.
 * - message: mensaje de error si la operación no tuvo éxito.
 * Si tuvo éxito, su valor es NULL.
 * - data: dirección del valor interpretado por el parser, si tuvo éxito.
 * Si no tuvo éxito su valor no está definido.
 */
typedef struct {
	const char* new_pointer;
	const char* message;
	void* data;
} ParseResult;

/* Parsea todos los espacios o caracters 'newline' que se encuentren
 * desde el puntero input.
 * Usamos el bool para decir si al menos un espacio es requerido
 * y en ese caso si no hay espacios devuelve un mensaje con el error
 */
ParseResult parse_white_spaces(const char* input, bool required) {
	bool seen = false;

	while (*input == ' ' || *input == '\n') {
		input++;
		seen = true;
	}

	if (required && !seen)
		return (ParseResult){input, COMMON_ERROR, NULL};

	return (ParseResult){input, NULL, NULL};
}

/* Parsea un string fijo que le pasamos en str, y en el caso de no encontrarse
 * devuleve un mensaje con el error
 */
ParseResult parse_string(const char* str, const char* input) {
	while (*str) {
		if (*input == *str)
			input++;
		else
			return (ParseResult){input, COMMON_ERROR, NULL};

		str++;
	}

	return (ParseResult){input, NULL, NULL};
}

/* Parsea un string (el identificador) que cumpla con las siguientes condiciones:
 * solo puede contener caracteres alfanuméricos,
 * no puede comenzar con un número
 * y no puede ser igual a ninguna de las palabras reservadas: cargar, imprimir, evaluar, y salir.
 * En el caso de que no se cumpla alguna condición devuelve un mensaje con el error
 */
ParseResult parse_identifier(const char* input) {
	const char* beggining = input;

	if (!isalpha(*input))
		return (ParseResult){input, "El alias no comienza con una letra.", NULL};

	while (isalnum(*input))
		input++;

	size_t length = input - beggining + 1;
	char* identifier = malloc(sizeof(char) * length);

	for (size_t j = 0; j < length; j++)
		identifier[j] = beggining[j];
	identifier[length - 1] = '\0';

	if (!strcmp(identifier, "cargar") || !strcmp(identifier, "imprimir") ||
	    !strcmp(identifier, "evaluar") || !strcmp(identifier, "salir")) {
		free(identifier);
		return (ParseResult){input, COMMON_ERROR, NULL};
	}

	return (ParseResult){input, NULL, identifier};
}

/* Verifica que estamos al final del string que nos pasaron,
 * en el caso de que siga habiendo caracteres devuelve un mensaje con el error
 */
ParseResult parse_end(const char* input) {
	if (*input)
		return (ParseResult){input, COMMON_ERROR, NULL};

	return (ParseResult){input, NULL, NULL};
}

/* Parsea un entero positivo, y en el caso de no contener devuelve un mensaje con el error
 */
ParseResult parse_int(const char* input) {
	if (isdigit(*input)) {
		char* ptr;
		int* integer = malloc(sizeof(int));
		*integer = strtol(input, &ptr, 10);
		return (ParseResult){ptr, NULL, integer};
	}

	return (ParseResult){input, "Se esperaba un entero.", NULL};
}

/* Parsea un operador y en caso de no contener ninguno,
 * devueve un mensaje indicándolo
 */
ParseResult parse_operator(const char* input, DArray op_table) {
	ParseResult result;
	Operator* operator;

	for (size_t i = 0; i < d_array_size(op_table); i++) {
		operator = (Operator*)d_array_get(op_table, i);

		result = parse_string(operator->symbol, input);
		if (result.message)
			continue;

		// NOTA: Luego de un operador debe estar el final del input
		// o debe haber al menos un espacio
		ParseResult result_end = parse_end(result.new_pointer);
		ParseResult result_space = parse_white_spaces(result.new_pointer, true);
		result = result_end.message ? result_space : result_end;
		if (result.message)
			continue;

		result.data = (void*)operator;
		return result;
	}

	return (ParseResult){input, "Operador no reconocido.", NULL};
}

/* Parsea una expresión postfija y retorna su árbol
 */
ParseResult parse_expression(const char* input, DArray op_table) {
	Expression tree = expression_make_empty();
	DArray stack = d_array_make(sizeof(Expression));

	ParseResult result;
	// Nos movemos usando (result.new_pointer)
	// para concatenar las funciones con facilidad
	result.new_pointer = input;

	while (*result.new_pointer) {
		result = parse_white_spaces(result.new_pointer, false);
		
		if (!parse_end(result.new_pointer).message)
			break;

		// Nueva variable para no pisar (result),
		// ya que luego es usada por (parse_operator).
		// No es prudente usar (result.new_pointer) de algo que dió error
		ParseResult result_int = parse_int(result.new_pointer);
		if (!result_int.message) {
			result = result_int;

			int* number = (int*)result_int.data;
			Expression new_tree = expression_from_number(*number);
			stack = d_array_insert(stack, &new_tree);
			free(number);

			continue;
		}

		// NOTA: parsea el/los espacios obligatorios después del operador
		result = parse_operator(result.new_pointer, op_table);
		if (result.message) {
			d_array_free(stack, expression_pointer_free);
			return result;
		}
		
		Operator* operator = (Operator*)result.data;

		size_t arity = operator->arity;
		if (d_array_size(stack) < arity) {
			d_array_free(stack, expression_pointer_free);
			return (ParseResult){
				input,
				"No hay suficientes operandos para un operador.",
				NULL
			};
		}

		// La subexpresión derecha siempre está,
		// mientras que la izquierda está solo cuando el operador es binario
		Expression data1 = expression_make_empty();
		Expression data2 = *(Expression*)d_array_get(stack, d_array_size(stack) - 1);
		stack = d_array_remove(stack, NULL);

		if (arity == 2) {
			data1 = *(Expression*)d_array_get(stack, d_array_size(stack) - 1);
			stack = d_array_remove(stack, NULL);
		}

		Expression new_tree = expression_join(*operator, data1, data2);
		stack = d_array_insert(stack, &new_tree);
	}

	if (d_array_size(stack) != 1) {
		d_array_free(stack, expression_pointer_free);
		return (ParseResult){result.new_pointer, "La expresión quedó incompleta.", NULL};
	}

	tree = *(Expression*)d_array_get(stack, 0);
	d_array_free(stack, NULL);

	return (ParseResult){result.new_pointer, NULL, tree};
}

/* Verifica si el comando es 'salir' con posibles espacios al final
 */
ParseResult parse_quit(const char* input) {
	ParseResult result;

	result = parse_white_spaces(input, false);
	result = parse_string("salir", result.new_pointer);
	if (result.message)
		return result;

	result = parse_white_spaces(result.new_pointer, false);
	result = parse_end(result.new_pointer);

	return result;
}

/* Verifica si el comando es 'evaluar identificador'
 */
ParseResult parse_eval(const char* input) {
	ParseResult result;

	result = parse_white_spaces(input, false);
	result = parse_string("evaluar", result.new_pointer);
	if (result.message)
		return result;

	result = parse_white_spaces(result.new_pointer, true);
	if (result.message)
		return result;

	result = parse_identifier(result.new_pointer);
	if (result.message)
		return result;

	char* identifier = (char*)result.data;

	result = parse_white_spaces(result.new_pointer, false);
	result = parse_end(result.new_pointer);
	if (result.message) {
		free(identifier);
		return result;
	}

	result.data = identifier;
	return result;
}

/* Verifica si el comando es 'imprimir identificador'
 */
ParseResult parse_print(const char* input) {
	ParseResult result;

	result = parse_white_spaces(input, false);
	result = parse_string("imprimir", result.new_pointer);
	if (result.message)
		return result;

	result = parse_white_spaces(result.new_pointer, true);
	if (result.message)
		return result;

	result = parse_identifier(result.new_pointer);
	if (result.message)
		return result;

	char* identifier = (char*)result.data;

	result = parse_white_spaces(result.new_pointer, false);
	result = parse_end(result.new_pointer);
	if (result.message) {
		free(identifier);
		return result;
	}

	result.data = identifier;
	return result;
}

/* Verifica si el comando es 'identificador = cargar expresion'
 */
ParseResult parse_assign(const char* input, DArray op_table) {
	ParseResult result;

	result = parse_white_spaces(input, false);
	result = parse_identifier(result.new_pointer);
	if (result.message)
		return result;

	char* identifier = (char*)result.data;

	result = parse_white_spaces(result.new_pointer, false);
	result = parse_string("=", result.new_pointer);
	if (result.message) {
		free(identifier);
		return result;
	}

	result = parse_white_spaces(result.new_pointer, false);
	result = parse_string("cargar", result.new_pointer);
	if (result.message) {
		free(identifier);
		return result;
	}

	result = parse_white_spaces(result.new_pointer, true);
	if (result.message) {
		free(identifier);
		return result;
	}

	result = parse_expression(result.new_pointer, op_table);
	if (result.message) {
		free(identifier);
		return result;
	}

	Expression expr = (Expression)result.data;

	result = parse_white_spaces(result.new_pointer, false);
	result = parse_end(result.new_pointer);
	if (result.message) {
		free(identifier);
		expression_free(expr);
		return result;
	}

	result.data = malloc(sizeof(Declaration));
	((Declaration*)result.data)->identifier = identifier;
	((Declaration*)result.data)->expr = expr;
	return result;
}

Statement parse_statement(const char* input, DArray op_table) {
	ParseResult result;

	result = parse_quit(input);
	if (!result.message)
		return (Statement){Quit, NULL, NULL};

	result = parse_eval(input);
	if (!result.message)
		return (Statement){Eval, (char*)result.data, NULL};

	result = parse_print(input);
	if (!result.message)
		return (Statement){Print, (char*)result.data, NULL};

	result = parse_assign(input, op_table);
	if (!result.message) {
		Declaration* data = (Declaration*)result.data;
		Statement stmt = (Statement){Assign, data->identifier, data->expr};
		// Liberar la declaración pero no su contenido
		free(data);
		return stmt;
	}

	printf("Error: %s\n", result.message);
	return (Statement){Error, NULL, NULL};
}

void statement_free(Statement stmt) {
	if (stmt.identifier)
		free(stmt.identifier);

	if (!expression_is_empty(stmt.expr))
		expression_free(stmt.expr);
}
