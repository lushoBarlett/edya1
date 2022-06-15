#include "test_declarations.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/declarations.h"
#include "../src/dynamic_array.h"
#include "../src/expression.h"

/* Prueba la re-asignación de variables
 */
static void test_redeclaration() {
	Expression expr1 = expression_from_number(1);
	Expression expr2 = expression_from_number(2);

	DArray decls = d_array_make(sizeof(Declaration));

	char* identifier1 = malloc(sizeof(char) * 5);
	strcpy(identifier1, "test");
	decls = declarations_assign(decls, identifier1, expr1);

	char* identifier2 = malloc(sizeof(char) * 5);
	strcpy(identifier2, "test");
	decls = declarations_assign(decls, identifier2, expr2);

	assert(declaration_find(decls, "test") == expr2);

	d_array_free(decls, declaration_free);
}

/* Prueba la asignación de nombres a expresiones,
 * la búsqueda de expresiones usando nombres y
 * la liberación de memoria
 */
static void test_operations() {
	Expression expr = expression_from_number(1);

	DArray decls = d_array_make(sizeof(Declaration));

	char* identifier = malloc(sizeof(char) * 5);
	strcpy(identifier, "test");
	decls = declarations_assign(decls, identifier, expr);

	assert(declaration_find(decls, "test") == expr);
	assert(expression_is_empty(declaration_find(decls, "test1")));

	d_array_free(decls, declaration_free);
}

void test_declarations() {
	test_operations();
	test_redeclaration();
	printf("declarations tests - OK\n");
}