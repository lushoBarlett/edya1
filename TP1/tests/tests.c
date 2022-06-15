#include "test_dynamic_array.h"
#include "test_expression.h"
#include "test_operators.h"
#include "test_declarations.h"
#include "test_parser.h"

int main() {
	test_dynamic_array();
	test_expression();
	test_operators();
	test_declarations();
	test_parser();

	return 0;
}