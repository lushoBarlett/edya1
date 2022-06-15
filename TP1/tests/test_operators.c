#include "test_operators.h"

#include <assert.h>
#include <stdio.h>

#include "../src/operators.h"

/* Prueba las implementaciones de todos los operadores
 */
static void test_all_operators() {
	int args[2] = {1, 3};

	assert(sum_impl(args) == 4);
	assert(sub_impl(args) == -2);
	assert(neg_impl(args) == -1);
	assert(mul_impl(args) == 3);
	assert(div_impl(args) == 0);
	assert(mod_impl(args) == 1);
	assert(pow_impl(args) == 1);
}

void test_operators() {
	test_all_operators();
	printf("operators tests - OK\n");
}