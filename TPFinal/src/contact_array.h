#pragma once

#include "contact.h"
#include "string.h"

/*
 * Un ContactArray representa un array de contactos cuyo
 * manejo de la memoria está automatizado.
 * 
 * size: la cantidad de elementos
 * storage_size: la cantidad de elememtos que soporta actualmente
 * array: el buffer que guarda los contactos
 */
typedef struct {
	size_t size;
	size_t storage_size;
	Contact* array;
} ContactArray;

/*
 * Crea un array de contactos de tamaño 0
 */
ContactArray contact_array_make();

/*
 * Copia un array de contactos, copiando todos los contactos que contiene
 */
ContactArray contact_array_copy(const ContactArray* contacts);

/*
 * Inserta un contacto al final del array, marcando como libre
 * la memoria del contacto proporcionado
 */
void contact_array_push_back(ContactArray* contacts, Contact* contact);

/*
 * Retorna la representación como string de todos los contactos del array
 */
String contact_array_string(const ContactArray* contacts);

/*
 * Retorna la representación de archivo CSV de todos los contactos del array
 */
String contact_array_comma_string(const ContactArray* contacts);

/*
 * Libera toda la memoria del array
 */
void contact_array_free(ContactArray* contacts);