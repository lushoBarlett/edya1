#pragma once

#include <stdlib.h>
#include <stdbool.h>

/*
 * Un String representa una secuencia de caracteres
 * delimitadas por un caracter terminador, cuya memoria
 * se maneja automáticamente
 * 
 * length: el largo de la secuencia de caracteres, sin contar el terminador
 * storage_size: la cantidad de caracteres que soporta en total actualmente
 * c_str: el buffer que guarda los caracteres
 */
typedef struct {
	size_t length;
	size_t storage_size;
	char* c_str;
} String;

/*
 * Crea un string de tamaño 0, el string vacío
 */
String string_empty();

/*
 * Crea un string con memoria liberada,
 * éste cuenta con tamaño -1 y buffer apuntando NULL
 */
String string_freed();

/*
 * Predicado para saber si un string tiene memoria liberada
 */
bool string_is_freed(const String* string);

/*
 * Retorna un string leído por consola, sin el caracter de nueva linea
 */
String string_getline();

/*
 * Dá vuelta un string
 */
void string_reverse(String* string);

/*
 * Inserta un caracter al final del string, reacomodando el terminador
 */
void string_push_back(String* string, char c);

/*
 * Inserta un grupo de caracteres al final del string,
 * reacomodando el terminador
 */
void string_append(String* string, const char* cs);

/*
 * Convierte un entero no negativo a un string
 */
String string_from_uint(unsigned d);

/*
 * Copia un string
 */
String string_copy(const String* string);

/*
 * Retorna el mismo string, marcando el argumento como liberado
 */
String string_move(String* string);

/*
 * Retorna el primer string, seguido del segundo, marcando ambos
 * argumentos como liberados
 */
String string_concat(String* a, String* b);

/*
 * Compara los strings lexicográficamente.
 * -1 si el primero es anterior al segundo
 * 1 si el segundo es anterior al primero
 * 0 si son iguales
 */
int string_comp(const String* a, const String* b);

/*
 * Libera la memoria de un contacto y lo marca como liberado
 */
void string_free(String* string);