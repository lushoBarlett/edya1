#pragma once

#include "contact.h"
#include "contact_array.h"

typedef struct Bucket* HashTable;

/*
 * Un ContactBook es un diccionario de contactos. Que internamente
 * funciona como una tabla hash. Se asume que el conjunto de nombre
 * y apellido de los contactos son siempre únicos. Se identifica
 * a un contacto de forma única por su nombre y apellido.
 * 
 * size: la cantidad de elementos del diccionario
 * inserted: la cantidad de celdas que alguna vez fueron ocupadas
 * storage_size: la cantidad de celdas disponibles
 * table: la memoria de la tabla hash
 */
typedef struct {
	size_t size;
	size_t inserted;
	size_t storage_size;
	HashTable table;
} ContactBook;

/*
 * Crea un diccionario de contactos vacío
 */
ContactBook contacts_make();

/*
 * Inserta un contacto al diccionario. Se asume que este no existe todavía.
 * Marca el contacto argumento como liberado
 */
void contacts_insert(ContactBook* contacts, Contact* contact);

/*
 * Borra un contacto del diccionario. No es necesario que éste exista
 */
void contacts_erase(ContactBook* contacts, const Contact* contact);

/*
 * Reemplaza un contacto del diccionario. No es necesario que éste exista.
 * Marca el contacto argumento como liberado
 */
void contacts_replace(ContactBook* contacts, Contact* contact);

/*
 * Busca un contacto en el diccionario. Si no existe se retorna NULL.
 * El puntero se invalida si se insertan nuevos contactos al diccionario
 */
const Contact* contacts_find(const ContactBook* contacts, const Contact* contact);

/*
 * Crea un array de contactos con todos los contactos presentes
 * en el diccionario, en ningún orden particular
 */
ContactArray contacts_array(const ContactBook* contacts);

/*
 * Libera la memoria de un diccionario de contactos
 */
void contacts_free(ContactBook* contacts);