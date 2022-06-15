#include "contact.h"

Contact contact_freed() {
	return (Contact){
		string_freed(),
		string_freed(),
		MAX_UINT,
		string_freed()
	};
}

bool contact_is_freed(const Contact* contact) {
	return string_is_freed(&contact->name) &&
	       string_is_freed(&contact->surname) &&
	       string_is_freed(&contact->phone);
}

Contact contact_copy(const Contact* contact) {
	return (Contact){
		string_copy(&contact->name),
		string_copy(&contact->surname),
		contact->age,
		string_copy(&contact->phone),
	};
}

Contact contact_move(Contact* contact) {
	Contact move = (Contact){
		string_move(&contact->name),
		string_move(&contact->surname),
		contact->age,
		string_move(&contact->phone),
	};

	*contact = contact_freed();
	return move;
}

int contact_comp_name(const Contact* a, const Contact* b) {
	return string_comp(&a->name, &b->name);
}

int contact_comp_surname(const Contact* a, const Contact* b) {
	return string_comp(&a->surname, &b->surname);
}

int contact_comp_age(const Contact* a, const Contact* b) {
	return a->age == b->age ? 0 : (a->age < b->age ? -1 : 1);
}

int contact_comp_phone(const Contact* a, const Contact* b) {
	return string_comp(&a->phone, &b->phone);
}

bool contact_equals(const Contact* a, const Contact* b) {
	return
	!contact_comp_name(a, b) &&
	!contact_comp_surname(a, b) &&
	!contact_comp_age(a, b) &&
	!contact_comp_phone(a, b);
}

size_t contact_hash(const Contact* contact) {
	size_t hash = 0;

	const char* p = contact->name.c_str;
	while(*p)
		hash = hash * 31 + *p++;

	p = contact->surname.c_str;
	while(*p)
		hash = hash * 31 + *p++;

	return hash;
}

String contact_string(const Contact* contact) {
	String string = string_empty();
	String copy;

	string_push_back(&string, '{');
	copy = string_copy(&contact->name);
	string = string_concat(&string, &copy);
	string_append(&string, ", ");

	copy = string_copy(&contact->surname);
	string = string_concat(&string, &copy);
	string_append(&string, ", ");

	copy = string_from_uint(contact->age);
	string = string_concat(&string, &copy);
	string_append(&string, ", ");

	copy = string_copy(&contact->phone);
	string = string_concat(&string, &copy);
	string_append(&string, "}");

	return string;
}

String contact_comma_string(const Contact* contact) {
	String string = string_empty();
	String copy;

	copy = string_copy(&contact->name);
	string = string_concat(&string, &copy);
	string_push_back(&string, ',');

	copy = string_copy(&contact->surname);
	string = string_concat(&string, &copy);
	string_push_back(&string, ',');

	copy = string_from_uint(contact->age);
	string = string_concat(&string, &copy);
	string_push_back(&string, ',');

	copy = string_copy(&contact->phone);
	string = string_concat(&string, &copy);

	return string;
}

void contact_free(Contact* contact) {
	contact->age = MAX_UINT;
	string_free(&contact->name);
	string_free(&contact->surname);
	string_free(&contact->phone);
}