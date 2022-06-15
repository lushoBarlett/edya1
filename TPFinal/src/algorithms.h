#pragma once

#include "contact_book.h"

/*
 * Busca contactos coincidentes con uno de consulte, de acuerdo a un modo de consulta,
 * sobre un array de contactos, y retora un nuevo array con los resultados.
 * 
 * El contacto de consulta no puede tener todos sus campos vacíos simultáneamente.
 * Se considera vacío un campo String si es el string vacío, no puede ser liberado.
 * Se considera vacía la edad si es MAX_UINT.
 * 
 * Cuando el modo de consulta es true, todos los campos no vacíos deben coincidir
 * para formar parte del resultado, y cuando es false solo alguno debe coincidir.
 */
ContactArray algorithms_query(const ContactArray* contacts, const Contact* contact, bool strict);

/*
 * Retorna algún subconjunto de un array de contactos cuyas edades suman en total
 * un número no negativo proporcionado como argumento.
 */
ContactArray algorithms_subset_sum(const ContactArray* contacts, unsigned sum);

/*
 * Ordena un array de contactos según alguno de los cuatro atributos
 * de los contactos.
 */
void algorithms_sort_by(ContactArray* contacts, Field order);