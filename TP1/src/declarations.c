#include "declarations.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DArray declarations_assign(DArray decls, char* identifier, Expression expr) {
	for (size_t i = 0; i < d_array_size(decls); i++) {
		Declaration* data = d_array_get(decls, i);

		// Redeclaración
		if (!strcmp(data->identifier, identifier)) {
			free(identifier);
			expression_free(data->expr);
			data->expr = expr;
			return decls;
		}
	}

	Declaration new_node;
	new_node.expr = expr;
	new_node.identifier = identifier;
	decls = d_array_insert(decls, &new_node);

	return decls;
}

Expression declaration_find(DArray decls, const char* identifier) {
	for (size_t i = 0; i < d_array_size(decls); i++) {
		Declaration* data = d_array_get(decls, i);
		if (!strcmp(data->identifier, identifier))
			return data->expr;
	}

	return expression_make_empty();
}

void declaration_free(void* decl) {
	free(((Declaration*)decl)->identifier);
	expression_free(((Declaration*)decl)->expr);
}