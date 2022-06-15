#pragma once

#include "contact_array.h"
#include "string.h"

/*
 * Dado el nombre de un archivo CSV, que se asume que existe,
 * carga sus contactos en un array de contactos. Se asume
 * que el archivo tiene el formato correcto
 */
ContactArray csv_load(const String* filename);

/*
 * Guarda en un archivo CSV los contactos proporcionados como
 * segundo argumento en un array de contactos
 */
void csv_save(const String* filename, const ContactArray* contacts);