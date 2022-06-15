#include "test_parser.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../src/parser.h"
#include "../src/operator_types.h"

/* Implementación de dos operadores de prueba
 */
static int sub(int* args) {
	return *args - *(args + 1);
}

static int neg(int* args) {
	return - *args;
}

/* Crea una tabla con los dos operadores
 */
static DArray op_table_init() {
	DArray op_table = d_array_make(sizeof(Operator));
	op_table = d_array_insert(op_table, &(Operator){"-",  2, sub});
	op_table = d_array_insert(op_table, &(Operator){"--", 1, neg});
	return op_table;
}

/* Prueba el caso en el que el comando ingresado sea "   salir  "
 */
static void test_quit() {
	DArray op_table = op_table_init();
	Statement stmt = parse_statement("   salir  ", op_table);
	d_array_free(op_table, NULL);

	assert(stmt.type == Quit);

	statement_free(stmt);
}

/* Prueba el caso en el que el comando ingresado sea "salir expr1"
 * donde se debería devolver un error ya que no se ingresó solo "salir"
 */
static void test_quit_fail() {
	DArray op_table = op_table_init();
	Statement stmt = parse_statement("salir expr1", op_table);
	d_array_free(op_table, NULL);

	assert(stmt.type == Error);

	statement_free(stmt);
}

/* Prueba el caso en el que el comando ingresado sea " evaluar  expr1  "
 */
static void test_eval() {
	DArray op_table = op_table_init();
	Statement stmt = parse_statement(" evaluar  expr1  ", op_table);
	d_array_free(op_table, NULL);

	assert(stmt.type == Eval);
	assert(!strcmp(stmt.identifier, "expr1"));

	statement_free(stmt);
}

/* Prueba el caso en el que el comando ingresado sea "evaluar  expr1 expr2"
 * donde se debería devolver un error ya que después de "evaluar" se ingresaron dos identificadores
 */
static void test_eval_fail() {
	DArray op_table = op_table_init();
	Statement stmt = parse_statement("evaluar  expr1 expr2", op_table);
	d_array_free(op_table, NULL);

	assert(stmt.type == Error);

	statement_free(stmt);
}

/* Prueba el caso en el que el comando ingresado sea "imprimir expr1"
 */
static void test_print() {
	DArray op_table = op_table_init();
	Statement stmt = parse_statement("imprimir expr1", op_table);
	d_array_free(op_table, NULL);

	assert(stmt.type == Print);
	assert(!strcmp(stmt.identifier, "expr1"));

	statement_free(stmt);
}

/* Prueba el caso en el que el comando ingresado sea "imprimir "
 * donde se debería devolver un error ya que no se ingresó ningún identificador
 */
static void test_print_fail() {
	DArray op_table = op_table_init();
	Statement stmt = parse_statement("imprimir ", op_table);
	d_array_free(op_table, NULL);

	assert(stmt.type == Error);

	statement_free(stmt);
}

/* Prueba el caso en el que el comando ingresado sea "expr1=cargar 5 1 - --"
 */
static void test_assign() {
	DArray op_table = op_table_init();
	Statement stmt = parse_statement("expr1=cargar 5 1 - --", op_table);
	d_array_free(op_table, NULL);

	assert(stmt.type == Assign);
	assert(!strcmp(stmt.identifier, "expr1"));
	assert(expression_eval(stmt.expr) == -4);

	statement_free(stmt);
}

/* Prueba el caso en el que el comando ingresado sea "expr1 = cargar"
 * donde se debería devolver un error ya que no hay expresión
 */
static void test_assign_fail() {
	DArray op_table = op_table_init();
	Statement stmt = parse_statement("expr1 = cargar", op_table);
	d_array_free(op_table, NULL);

	assert(stmt.type == Error);

	statement_free(stmt);
}

/* Prueba que los identificadores sean validados correctamente
 */
static void test_identifier_fail() {
	DArray op_table = op_table_init();
	Statement stmt = parse_statement("1expr = cargar 5 --", op_table);
	d_array_free(op_table, NULL);

	assert(stmt.type == Error);

	statement_free(stmt);
}

/* Prueba que la expresión sea validada correctamente,
 * ya sea porque la aridad de algún operador está bien
 * o porque la expresión está incompleta
 */
static void test_expression_fail() {
	DArray op_table = op_table_init();
	Statement stmt1 = parse_statement("expr1 = cargar 5 1 --", op_table);
	Statement stmt2 = parse_statement("expr2 = cargar 5 -", op_table);
	d_array_free(op_table, NULL);

	assert(stmt1.type == Error);
	assert(stmt2.type == Error);

	statement_free(stmt1);
	statement_free(stmt2);
}

void test_parser() {
	test_quit();
	test_quit_fail();
	test_eval();
	test_eval_fail();
	test_print();
	test_print_fail();
	test_assign();
	test_assign_fail();
	test_identifier_fail();
	test_expression_fail();
	printf("parser tests - OK\n");
}
