#pragma once

/* Función evaluadora de operadores.
 * Toma una cantidad arbitraria de argumentos y retorna un resultado
 */
typedef int (*OperatorCall)(int* args);

/* Un operador cuenta con un string (symbol) que contiene su
 * representación visual, la cantidad de argumentos que toma (arity)
 * y una función evaluadora (implementation)
 */
typedef struct {
	const char* symbol;
	int arity;
	OperatorCall implementation;
} Operator;