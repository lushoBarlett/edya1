#include "utils.h"

#include "../src/string.h"

Contact utils_contact(const char* name, const char* surname, unsigned age, const char* phone) {
	Contact answer = contact_freed();
	String aux;

	if (name) {
		aux = string_empty();
		string_append(&aux, name);
		answer.name = string_move(&aux);
	}

	if (surname) {
		aux = string_empty();
		string_append(&aux, surname);
		answer.surname = string_move(&aux);
	}

	answer.age = age;

	if (phone) {
		aux = string_empty();
		string_append(&aux, phone);
		answer.phone = string_move(&aux);
	}

	return answer;
}

Contact utils_shortc(char name, char surname) {
	Contact answer = contact_freed();
	String aux;

	aux = string_empty();
	string_push_back(&aux, name);
	answer.name = string_move(&aux);

	aux = string_empty();
	string_push_back(&aux, surname);
	answer.surname = string_move(&aux);

	answer.age = 5;

	aux = string_empty();
	string_append(&aux, "123");
	answer.phone = string_move(&aux);

	return answer;
}

ContactArray utils_contact_array() {
	ContactArray contacts = contact_array_make();

	Contact contact;
	contact = utils_contact("a", "b", 5, "123");
	contact_array_push_back(&contacts, &contact);
	contact = utils_contact("a", "c", 5, "123");
	contact_array_push_back(&contacts, &contact);
	contact = utils_contact("a", "b", 10, "123");
	contact_array_push_back(&contacts, &contact);
	contact = utils_contact("a", "b", 5, "1234");
	contact_array_push_back(&contacts, &contact);
	contact = utils_contact("c", "d", 1, "4321");
	contact_array_push_back(&contacts, &contact);
	contact = utils_contact("c", "d", 7, "4321");
	contact_array_push_back(&contacts, &contact);

	return contacts;
}