#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef double* Vector;

double vector_length(Vector v) {
	double length = 0.0;
	size_t size = sizeof(v)/sizeof(v[0]);

	for(int i=0;i<size;i++) {
		length += v[i] * v[i];
	}
	printf("%zu\n", size);

	length = sqrt(length);

	return length;
}
