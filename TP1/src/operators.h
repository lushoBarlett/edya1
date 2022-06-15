#pragma once

/* Implementaciones de los diferentes operadores, con su (aridad)
 * - suma           (2)
 * - resta          (2)
 * - opuesto        (1)
 * - multiplicación (2)
 * - división       (2)
 * - módulo         (2)
 * - potencia       (2)
 */

int sum_impl(int* args);
int sub_impl(int* args);
int neg_impl(int* args);
int mul_impl(int* args);
int div_impl(int* args);
int mod_impl(int* args);
int pow_impl(int* args);