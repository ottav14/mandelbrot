#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>

Vector create_vector(double* data, size_t size) {
	Vector v = (Vector){.data = data, .size = size};
	return v;
}

