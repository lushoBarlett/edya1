#include "test_expression.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "../src/expression.h"

static int sum(int* args) {
	return *args + *(args + 1);
}

static int neg(int* args) {
	return - *args;
}

/* Prueba la funcionalidad de la expresión vacía
 * y que el predicado expression_is_empty concuerde
 */
static void test_empty_expression() {
	Expression expr = expression_make_empty();

	assert(expression_is_empty(expr));

	expression_free(expr);
}

/* Prueba la funcionalidad de la expresión con un entero
 * y que el predicado expression_is_empty concuerde
 */
static void test_make_number() {
	Expression expr = expression_from_number(1);

	assert(!expression_is_empty(expr));

	expression_free(expr);
}

/* Prueba la evaluación e impresión de una expresión compleja
 * con joins usando operadores unarios y binarios
 */
static void test_operations() {
	Expression n1 = expression_from_number(1);
	Expression n2 = expression_from_number(2);

	// NOTA: operador unario lleva la subexpresión a la derecha
	Expression expr1 = expression_join((Operator){"--", 1, neg}, NULL, n1);
	Expression expr2 = expression_join((Operator){"+", 2, sum}, expr1, n2);

	assert(expression_eval(expr2) == 1);

	expression_print_infix(expr2);
	printf("\n");

	expression_free(expr2);
}

void test_expression() {
	test_empty_expression();
	test_make_number();
	test_operations();
	printf("expression tests - OK\n");
}