#pragma once

#include <stdbool.h>

#include "string.h"

/*
 * Vale lo que el número más grande que entra en un entero sin signo,
 * lo usamos para representar una edad vacía
 */
#define MAX_UINT (~(unsigned)0)

/*
 * Un Contact contiene toda la información de un contacto
 *
 * name: el nombre, como una cadena de caracteres
 * surname: el apellido, como otra cadena de caracteres
 * age: la edad, como un entero positivo
 * phone: el número de teléfono, también como cadena de caracteres
 */
typedef struct {
	String name;
	String surname;
	unsigned age;
	String phone;
} Contact;

/*
 * Un Field representa uno de los cuatro atributos de un contacto,
 * con un extra NoField denotando campo inválido
 */
typedef enum {
	NoField,
	Name,
	Surname,
	Age,
	Phone,
} Field;

/*
 * Crea un contacto con memoria liberada,
 * éste cuenta con strings liberados y edad MAX_UINT
 */
Contact contact_freed();

/*
 * Predicado para saber si un contacto tiene memoria liberada
 */
bool contact_is_freed(const Contact* contact);

/*
 * Copia un contacto
 */
Contact contact_copy(const Contact* contact);

/*
 * Retorna el mismo contacto, marcando el argumento como liberado
 */
Contact contact_move(Contact*);

/*
 * Compara los nombres de los contactos.
 * -1 si el primero es anterior al segundo
 * 1 si el segundo es anterior al primero
 * 0 si son iguales
 */
int contact_comp_name(const Contact* a, const Contact* b);

/*
 * Compara los apellidos de los contactos.
 * -1 si el primero es anterior al segundo
 * 1 si el segundo es anterior al primero
 * 0 si son iguales
 */
int contact_comp_surname(const Contact* a, const Contact* b);

/*
 * Compara las edades de los contactos.
 * -1 si el primero es anterior al segundo
 * 1 si el segundo es anterior al primero
 * 0 si son iguales
 */
int contact_comp_age(const Contact* a, const Contact* b);

/*
 * Compara los teléfonos de los contactos.
 * -1 si el primero es anterior al segundo
 * 1 si el segundo es anterior al primero
 * 0 si son iguales
 */
int contact_comp_phone(const Contact* a, const Contact* b);

/*
 * Predicado para saber si los contactos son completamente iguales
 */
bool contact_equals(const Contact* a, const Contact* b);

/*
 * Función de hash para un contacto, toma en cuenta su nombre y apellido
 */
size_t contact_hash(const Contact* contact);

/*
 * Retorna la representación como string del contacto
 */
String contact_string(const Contact* contact);

/*
 * Retorna la representación de archivo CSV del contacto
 */
String contact_comma_string(const Contact* contact);

/*
 * Libera la memoria de un contacto y lo marca como liberado
 */
void contact_free(Contact* contact);