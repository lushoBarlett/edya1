#include "expression.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "operator_types.h"

/* NodeType representa los dos tipos que puede tener una expresión
 * 1. Un número entero
 * 2. Un operador
 */
typedef enum { NumberNode, OperatorNode } NodeType;

/* Una expresión es un árbol binario, cuyos valores de los nodos internos
 * son operadores, y los valores de las hojas son enteros. Una expresión
 * encabezada por un operador tiene dos subexpresiones a la izquiera y a
 * la derecha.
 * 
 * Representamos el tipo de nodo (entero o operador) usando un NodeType (type).
 * Guardamos un int (number) o un Operator (operator) de acuerdo al caso.
 * Finalemnte guardamos las dos subexpresiones (left) y (right)
 * 
 * Por convención, cuando un operador es unario, la subexpresión que le corresponde
 * será la derecha.
 */
struct _ExprNode {
	NodeType type;
	Operator operator;
	int number;
	struct _ExprNode* left;
	struct _ExprNode* right;
};

Expression expression_make_empty() {
	return NULL;
}

Expression expression_from_number(int number) {
	Expression new_tree = malloc(sizeof(struct _ExprNode));
	assert(new_tree);

	new_tree->type = NumberNode;
	new_tree->number = number;
	new_tree->left = NULL;
	new_tree->right = NULL;

	return new_tree;
}

void expression_free(Expression expr) {
	if (expr == NULL)
		return;

	expression_free(expr->left);
	expression_free(expr->right);
	free(expr);
}

void expression_pointer_free(void* expr_pointer) {
	expression_free(*(Expression*)expr_pointer);
}

bool expression_is_empty(Expression expr) {
	return expr == NULL;
}

Expression expression_join(Operator operator, Expression left, Expression right) {
	Expression new_tree = malloc(sizeof(struct _ExprNode));
	assert(new_tree);

	new_tree->type = OperatorNode;
	new_tree->operator = operator;
	new_tree->left = left;
	new_tree->right = right;

	return new_tree;
}

int expression_eval(Expression expr) {
	if (expr == NULL)
		return 0;

	if (expr->type == NumberNode)
		return expr->number;

	int left = expression_eval(expr->left);
	int right = expression_eval(expr->right);

	int resultados[2];
	if (expr->left == NULL) {
		resultados[0] = right;
	} else {
		resultados[0] = left;
		resultados[1] = right;
	}

	return expr->operator.implementation(resultados);
}

void expression_print_infix(Expression expr) {
	if (expr == NULL)
		return;

	if (expr->type == NumberNode) {
		printf("%d", expr->number);
	} else {
		printf("(");
		if (expr->operator.arity == 2) {
			expression_print_infix(expr->left);
			printf(" ");
		}
		printf("%s ", expr->operator.symbol);
		expression_print_infix(expr->right);
		printf(")");
	}
}