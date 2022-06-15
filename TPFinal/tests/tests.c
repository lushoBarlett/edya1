#include "test_string.h"
#include "test_contact_book.h"
#include "test_contact_array.h"
#include "test_action_stack.h"
#include "test_contact.h"
#include "test_action.h"
#include "test_algorithms.h"
#include "test_csv.h"

/*
 * Realiza todas las pruebas del programa
 */
int main() {
	test_string();
	test_contact();
	test_action();
	test_action_stack();
	test_contact_array();
	test_contact_book();
	test_algorithms();
	test_csv();

	return 0;
}