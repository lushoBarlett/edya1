#include "string.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

String string_empty() {
	char* empty = malloc(sizeof(char));
	assert(empty);
	empty[0] = '\0';
	return (String){0, 1, empty};
}

bool string_is_freed(const String* string) {
	return !string->c_str;
}

String string_freed() {
	return (String){-1, 0, NULL};
}

String string_getline() {
	String line = string_empty();
	for (char c; (c = getchar()) != '\n';)
		string_push_back(&line, c);

	return line;
}

void string_reverse(String* string) {
	if (!string->length)
		return;

	for (size_t l = 0, r = string->length - 1; l < r; l++, r--) {
		// XOR swap, no funciona cuando l == r
		// pero eso nunca pasa
		string->c_str[r] ^= string->c_str[l];
		string->c_str[l] ^= string->c_str[r];
		string->c_str[r] ^= string->c_str[l];
	}
}

void string_push_back(String* string, char c) {
	if (string->length + 1 == string->storage_size) {
		string->storage_size *= 2;
		string->c_str = realloc(string->c_str, sizeof(char) * string->storage_size);
		assert(string->c_str);
	}

	string->length++;
	string->c_str[string->length - 1] = c;
	string->c_str[string->length] = '\0';
}

void string_append(String* string, const char* cs) {
	while (*cs)
		string_push_back(string, *cs++);
}

String string_copy(const String* string) {
	char* copy = malloc(sizeof(char) * (string->length + 1));
	assert(copy);
	strcpy(copy, string->c_str);
	return (String){string->length, string->length + 1, copy};
}

String string_move(String* string) {
	String move = (String){
		string->length,
		string->storage_size,
		string->c_str
	};
	*string = string_freed();
	return move;
}

String string_concat(String* a, String* b) {
	for (const char* c = b->c_str; *c; c++)
		string_push_back(a, *c);

	free(b->c_str);
	*b = string_freed();
	return string_move(a);
}

String string_from_uint(unsigned d) {
	String string = string_empty();
	if (!d)
		string_push_back(&string, '0');

	while(d) {
		string_push_back(&string, d % 10 + '0');
		d /= 10;
	}
	string_reverse(&string);
	return string;
}

int string_comp(const String* a, const String* b) {
	int cmp = strcmp(a->c_str, b->c_str);

	if (cmp > 0)
		cmp = 1;
	else if (cmp < 0)
		cmp = -1;

	return cmp;
}

void string_free(String* string) {
	free(string->c_str);
	*string = string_freed();
}