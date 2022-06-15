#pragma once

#include "../src/contact.h"
#include "../src/contact_array.h"

/*
 * Construye un contacto a partir de valores constantes, de faltar alguno,
 * se rellenará con aquellos que corresponden a un contacto liberado
 */
Contact utils_contact(const char* name, const char* surname, unsigned age, const char* phone);

/*
 * Construye un contacto a partir de un nombre y apellido de tamaño 1,
 * o sea de un solo caracter, y que tiene edad 5 y teléfono 123
 */
Contact utils_shortc(char name, char surname);

/*
 * Construye un array de contactos particular que contiene, en orden
 *
 * {a, b, 5, 123}
 * {a, c, 5, 123}
 * {a, b, 10, 123}
 * {a, b, 5, 123}
 * {c, d, 1, 4321}
 * {c, d, 7, 4321}
 */
ContactArray utils_contact_array();