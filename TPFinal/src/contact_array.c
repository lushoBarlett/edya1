#include "contact_array.h"

#include <assert.h>

ContactArray contact_array_make() {
	return (ContactArray){0, 0, NULL};
}

ContactArray contact_array_copy(const ContactArray* contacts) {
	ContactArray answer = contact_array_make();
	for (size_t i = 0; i < contacts->size; i++) {
		Contact copy = contact_copy(&contacts->array[i]);
		contact_array_push_back(&answer, &copy);
	}
	return answer;	
}

void contact_array_push_back(ContactArray* contacts, Contact* contact) {
	if (contacts->size == contacts->storage_size) {
		contacts->storage_size = contacts->storage_size ? contacts->storage_size * 2 : 1;
		contacts->array = realloc(contacts->array, sizeof(*contacts->array) * contacts->storage_size);
		assert(contacts->array);
	}

	contacts->array[contacts->size++] = contact_move(contact);
}

String contact_array_string(const ContactArray* contacts) {
	String answer = string_empty();
	for (size_t i = 0; i < contacts->size; i++) {
		String cs = contact_string(&contacts->array[i]);
		string_push_back(&cs, '\n');
		answer = string_concat(&answer, &cs);
	}
	return answer;
}

String contact_array_comma_string(const ContactArray* contacts) {
	String answer = string_empty();
	for (size_t i = 0; i < contacts->size; i++) {
		String cs = contact_comma_string(&contacts->array[i]);
		string_push_back(&cs, '\n');
		answer = string_concat(&answer, &cs);
	}
	return answer;
}

void contact_array_free(ContactArray* contacts) {
	for (size_t i = 0; i < contacts->size; i++)
		contact_free(&contacts->array[i]);
	free(contacts->array);
	*contacts = (ContactArray){0, 0, NULL};
}