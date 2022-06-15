#pragma once

#include <stdbool.h>

#include "operator_types.h"

typedef struct _ExprNode* Expression;

/* Construye una expresión vacía
 */
Expression expression_make_empty();

/* Construye una expresión que consta de un único entero
 */
Expression expression_from_number(int number);

/* Libera la memoria de una expresión, incluyendo
 * la de todas sus subexpresiones
 */
void expression_free(Expression expr);

/* Libera la memoria de un puntero a una expresión,
 * análogamente a (expression_free)
 */
void expression_pointer_free(void* expr_pointer);

/* Predicado para saber si la expresión está vacía
 */
bool expression_is_empty(Expression expr);

/* Construye una expresión con un operador y sus subexpresiones izquierda y derecha.
 * Si el operador es unario se construye con la subexpresión izquierda, ignorando la derecha
 */
Expression expression_join(Operator operator, Expression left, Expression right);

/* Evalúa matemáticamente la expresión
 */
int expression_eval(Expression expr);

/* Imprime la expresión en su notación infija, con paréntesis alrededor
 * de cada subexpresión, excepto alrededor de expresiones de enteros
 */
void expression_print_infix(Expression expr);