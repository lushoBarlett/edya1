#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>

#include "contact_book.h"
#include "action_stack.h"
#include "csv.h"
#include "string.h"
#include "algorithms.h"
#include "contact_array.h"

/*
 * Imprime el menú por consola
 */
void print_menu() {
	printf(
	"Menu de acciones:\n"
	"1. Buscar\n"
	"2. Agregar\n"
	"3. Eliminar\n"
	"4. Editar\n"
	"5. Cargar\n"
	"6. Guardar\n"
	"7. Deshacer\n"
	"8. Rehacer\n"
	"9. AND\n"
	"10. OR\n"
	"11. Guardar ordenado\n"
	"12. Buscar por suma de edades\n"
	"13. Salir\n");
}

/*
 * Pide un string no vacío por consola,
 * repite hasta que se proporcione, imprimiendo
 * un mensaje de error cada vez
 */
String prompt_string(const char* message) {
	String string = string_getline();
	while (!string.length) {
		printf(message);
		string_free(&string);
		string = string_getline();
	}
	return string;
}

/*
 * Pide un entero no negativo por consola,
 * MAX_UINT se considera un error
 */
unsigned prompt_uint() {
	String string = string_getline();

	int d = strtol(string.c_str, NULL, 10);

	// Encontramos el primer caracter no separador
	const char* c = string.c_str;
	while(*c && isblank(*c)) c++;

	// strtol nos dió un 0, pero el primer caracter no es un 0?
	// este caso representa un error
	unsigned answer = d < 0 || (d == 0 && *c != '0') ? MAX_UINT : (unsigned)d;
	string_free(&string);
	return answer;
}

/*
 * Pide un código de acción por consola,
 * repite hasta que sea válido
 */
ActionType prompt_action() {
	printf("Seleccione una accion:\n> ");
	unsigned action = prompt_uint();
	while (action < Search || action > Quit) {
		printf("*** La accion es invalida ***\n> ");
		print_menu();
		printf("> ");
		action = prompt_uint();
	}
	return action;
}

/*
 * Guarda en un contacto un nombre y apellido
 * no vacíos pedidos por consola
 */
void prompt_fullname(Contact* contact) {
	printf("Ingrese un nombre:\n> ");
	contact->name = prompt_string("*** El nombre no puede ser vacio ***\n> ");
	printf("Ingrese un apellido:\n> ");
	contact->surname = prompt_string("*** El apellido no puede ser vacio ***\n> ");
}

/*
 * Guarda en un contacto una edad no negativa
 * y un teléfono no vacío pedidos por consola
 */
void prompt_data(Contact* contact) {
	printf("Ingrese una edad:\n> ");
	contact->age = prompt_uint();
	while (contact->age == MAX_UINT) {
		printf("*** La edad debe ser un numero no negativo ***\n> ");
		contact->age = prompt_uint();
	}

	printf("Ingrese un telefono:\n> ");
	contact->phone = prompt_string("*** El telefono no puede ser vacio ***\n> ");
}

/*
 * Pide un nombre no vacío de un archivo por consola,
 * como parámetro se pasa true si debe existir
 * y false si puede no existir
 */
String prompt_filename(bool existing) {
	printf("Ingrese una ruta de archivo:\n> ");
	String filename = prompt_string("*** La ruta no puede ser vacia ***\n> ");
	if (!existing)
		return filename;

	FILE* f;
	while (!(f = fopen(filename.c_str, "r"))) {
		string_free(&filename);
		printf("*** El archivo no existe ***\n> ");
		filename = prompt_string("*** La ruta no puede ser vacia ***\n> ");
	}
	fclose(f);

	return filename;
}

/*
 * Convierte un string a un atributo de contacto.
 * NoField si el atributo no es válido
 */
Field string_to_field(const String* string) {
	Field field;

	if (!strcmp(string->c_str, "nombre"))
		field = Name;
	else if (!strcmp(string->c_str, "apellido"))
		field = Surname;
	else if (!strcmp(string->c_str, "edad"))
		field = Age;
	else if (!strcmp(string->c_str, "telefono"))
		field = Phone;
	else
		field = NoField;

	return field;
}

/*
 * Pide un atributro de contacto por consola,
 * repite hasta que sea válido
 */
Field prompt_field() {
	printf("Ingrese un atributo:\n> ");
	String line = string_getline();
	Field field = string_to_field(&line);
	while (field == NoField) {
		printf("*** El atributo no existe ***\n> ");
		string_free(&line);
		line = string_getline();
		field = string_to_field(&line);
	}
	string_free(&line);
	return field;
}

/*
 * Pide todos los atributos para un contacto,
 * repite hasta que al menos uno sea no vacío
 */
void prompt_incomplete_contact(Contact* contact) {
	printf("Ingrese un nombre (opcional):\n> ");
	contact->name = string_getline();
	printf("Ingrese un apellido (opcional):\n> ");
	contact->surname = string_getline();
	printf("Ingrese una edad (opcional):\n> ");
	contact->age = prompt_uint();
	printf("Ingrese un telefono (opcional):\n> ");
	contact->phone = string_getline();

	if (!contact->name.length &&
	    !contact->surname.length &&
	    contact->age == MAX_UINT &&
	    !contact->phone.length) {
		printf("*** Al menos un campo debe ser no vacio ***\n");
		prompt_incomplete_contact(contact);
	}
}

/*
 * Pide un entero no negativo que representa una suma de edades
 */
int prompt_sum() {
	printf("Ingrese una suma de edades:\n> ");
	unsigned sum = prompt_uint();
	while (sum == MAX_UINT) {
		printf("*** La suma de edades debe ser un numero no negativo ***\n> ");
		sum = prompt_uint();
	}
	return sum;
}

/*
 * Realiza una operación de búsqueda de contacto
 */
void search(ContactBook* contacts) {
	Contact contact = contact_freed();
	prompt_fullname(&contact);
	const Contact* found = contacts_find(contacts, &contact);
	if (found) {
		String answer = contact_string(found);
		printf("%s\n", answer.c_str);
		string_free(&answer);
	} else {
		printf("*** Contacto no encontrado ***\n");
	}

	contact_free(&contact);
}

/*
 * Realiza una operación de inserción de contacto
 */
void insert(ContactBook* contacts, ActionStack* actions) {
	Contact contact = contact_freed();
	prompt_fullname(&contact);
	if (!contacts_find(contacts, &contact)) {
		prompt_data(&contact);

		Contact insert_copy = contact_copy(&contact);
		Action insertion = action_insertion(&insert_copy);

		actions_push(actions, &insertion);
		contacts_insert(contacts, &contact);
	} else {
		printf("*** Contacto ya existente ***\n");
	}

	contact_free(&contact);
}

/*
 * Realiza una operación de borrado de contacto
 */
void erase(ContactBook* contacts, ActionStack* actions) {
	Contact contact = contact_freed();
	prompt_fullname(&contact);
	const Contact* erasing = contacts_find(contacts, &contact);
	if (erasing) {
		Contact erase_copy = contact_copy(erasing);
		Action erasure = action_erasure(&erase_copy);

		actions_push(actions, &erasure);
		contacts_erase(contacts, &contact);
	} else {
		printf("*** Contacto no existente ***\n");
	}

	contact_free(&contact);
}

/*
 * Realiza una operación de reemplazo de contacto
 */
void replace(ContactBook* contacts, ActionStack* actions) {
	Contact contact = contact_freed();
	prompt_fullname(&contact);
	const Contact* replacing = contacts_find(contacts, &contact);
	if (replacing) {
		prompt_data(&contact);

		Contact insert_copy = contact_copy(&contact);
		Contact erase_copy = contact_copy(replacing);
		Action replacement = action_replacement(&insert_copy, &erase_copy);

		actions_push(actions, &replacement);
		contacts_replace(contacts, &contact);
	} else {
		printf("*** Contacto no existente ***\n");
	}

	contact_free(&contact);
}

/*
 * Realiza una operación de carga de contactos desde un archivo
 */
void load(ContactBook* contacts) {
	String filename = prompt_filename(true);
	ContactArray array = csv_load(&filename);

	for (size_t i = 0; i < array.size; i++)
		if (!contacts_find(contacts, &array.array[i]))
			contacts_insert(contacts, &array.array[i]);
	
	contact_array_free(&array);
	string_free(&filename);
}

/*
 * Realiza una operación de guardado de contactos hacia un archivo
 */
void save(ContactBook* contacts) {
	String filename = prompt_filename(false);
	ContactArray array = contacts_array(contacts);
	csv_save(&filename, &array);
	contact_array_free(&array);
	string_free(&filename);
}

/*
 * Deshace la última acción realizada, si se puede
 */
void undo(ContactBook* contacts, ActionStack* actions) {
	Action undo = actions_undo(actions);
	switch (undo.type) {
	case Insert:
		contacts_erase(contacts, &undo.added);
		break;
	case Erase:
		contacts_insert(contacts, &undo.deleted);
		break;
	case Replace:
		contacts_erase(contacts, &undo.added);
		contacts_insert(contacts, &undo.deleted);
		break;
	case Error:
		printf("*** No se puede deshacer ninguna accion ***\n");
		break;
	default:
		assert(0);
		break;
	}
}

/*
 * Rehace la última acción deshecha, si se puede
 */
void redo(ContactBook* contacts, ActionStack* actions) {
	Action redo = actions_redo(actions);
	switch (redo.type) {
	case Insert:
		contacts_insert(contacts, &redo.added);
		break;
	case Erase:
		contacts_erase(contacts, &redo.deleted);
		break;
	case Replace:
		contacts_erase(contacts, &redo.deleted);
		contacts_insert(contacts, &redo.added);
		break;
	case Error:
		printf("*** No se puede rehacer ninguna accion ***\n");
		break;
	default:
		assert(0);
		break;
	}
}

/*
 * Realiza una operación de búsqueda de contactos por coincidencias,
 * ya sea comparando con todos o con alguno de los atributos, determinado
 * por el tipo de acción en el parámetro
 */
void query_with(ContactBook* contacts, ActionType action) {
	Contact contact;
	prompt_incomplete_contact(&contact);

	ContactArray array = contacts_array(contacts);
	ContactArray answer = algorithms_query(&array, &contact, action == And);

	if (!answer.size) {
		printf("*** No hay entradas ***\n");
	} else {
		String string = contact_array_string(&answer);
		printf("%s", string.c_str);
		string_free(&string);
	}

	contact_array_free(&array);
	contact_array_free(&answer);
}

/*
 * Realiza una operación de guardado de contactos hacia un archivo,
 * ordenados según algún atributo
 */
void ordered_save(ContactBook* contacts) {
	String filename = prompt_filename(false);
	Field field = prompt_field();

	ContactArray array = contacts_array(contacts);
	algorithms_sort_by(&array, field);
	csv_save(&filename, &array);

	contact_array_free(&array);
	string_free(&filename);
}

/*
 * Realiza una operación de búsqueda de subconjunto de contactos
 * cuyas edades suman algún total
 */
void sum_search(ContactBook* contacts) {
	ContactArray array = contacts_array(contacts);
	ContactArray answer = algorithms_subset_sum(&array, prompt_sum());
	contact_array_free(&array);

	if (!answer.size) {
		printf("*** No hay solucion ***\n");
	} else {
		String string = contact_array_string(&answer);
		printf("%s", string.c_str);
		string_free(&string);
	}

	contact_array_free(&answer);
}

/*
 * Punto de entrada del programa
 */
int main() {
	print_menu();
	
	ContactBook contacts = contacts_make();
	ActionStack actions = actions_make();
	ActionType action;
	while((action = prompt_action()) != Quit) {
		switch (action) {
		case Search: search(&contacts); break;
		case Insert: insert(&contacts, &actions); break;
		case Erase: erase(&contacts, &actions); break;
		case Replace: replace(&contacts, &actions); break;
		case Load: load(&contacts); break;
		case Save: save(&contacts); break;
		case Undo: undo(&contacts, &actions); break;
		case Redo: redo(&contacts, &actions); break;
		case And: query_with(&contacts, And); break;
		case Or: query_with(&contacts, Or); break;
		case OrderedSave: ordered_save(&contacts); break;
		case SumSearch: sum_search(&contacts); break;
		case Quit: assert(0);
		case Error: assert(0);
		}
	}

	return 0;
}