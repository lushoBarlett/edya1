#include "operators.h"

#include <math.h>

int sum_impl(int* args) {
	return *args + *(args + 1);
}

int sub_impl(int* args) {
	return *args - *(args + 1);
}

int neg_impl(int* args) {
	return - *args;
}

int mul_impl(int* args) {
	return *args * *(args + 1);
}

int div_impl(int* args) {
	return *args / *(args + 1);
}

int mod_impl(int* args) {
	return *args % *(args + 1);
}

int pow_impl(int* args) {
	return pow(*args, *(args + 1));
}