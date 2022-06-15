#include "csv.h"

#include <stdio.h>

/*
 * Lee un string de un archivo CSV, hasta una coma,
 * una nueva línea o el final del archivo. String
 * vacío se considera un error
 */
static String parse_string(FILE* f) {
	String string = string_empty();

	int c;
	while ((c = fgetc(f)) != EOF && c != ',' && c != '\n')
		string_push_back(&string, c);

	return string;	
}

/*
 * Lee un entero no negativo de un archivo CSV, hasta una coma,
 * una nueva línea o el final del archivo. MAX_UINT se
 * considera un error
 */
static unsigned parse_uint(FILE* f) {
	String string = parse_string(f);

	int d = strtol(string.c_str, NULL, 10);

	// strtol nos dió un 0, pero el primer caracter no es un 0?
	// este caso representa un error
	unsigned answer = d < 0 || (d == 0 && *string.c_str != '0') ? MAX_UINT : (unsigned)d;
	string_free(&string);
	return answer;
}

ContactArray csv_load(const String* filename) {
	FILE* f = fopen(filename->c_str, "r");

	// Leemos la primer linea con los 4 atributos
	for (size_t i = 0; i < 4; i++) {
		String field = parse_string(f);
		string_free(&field);
	}

	Contact contact;
	ContactArray contacts = contact_array_make();
	while(!feof(f)) {
		contact.name = parse_string(f);
		contact.surname = parse_string(f);
		contact.age = parse_uint(f);
		contact.phone = parse_string(f);

		// Cargamos si es un contacto
		// que no se leyó con errores
		if (contact.name.length &&
		    contact.surname.length &&
		    contact.age != MAX_UINT &&
		    contact.phone.length)
			contact_array_push_back(&contacts, &contact);
		else
			contact_free(&contact);
	}

	fclose(f);
	return contacts;
}

void csv_save(const String* filename, const ContactArray* contacts) {
	FILE* f = fopen(filename->c_str, "w");

	// Ponemos los 4 atributos al principio del archivo 
	String start = string_empty();
	string_append(&start, "nombre,apellido,edad,telefono\n");

	String string = contact_array_comma_string(contacts);
	string = string_concat(&start, &string);

	fputs(string.c_str, f);
	string_free(&string);

	fclose(f);
}