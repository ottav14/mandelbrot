#include <stddef.h>

#ifndef VECTOR_H
#define VECTOR_H

typedef struct { double* data; size_t size; } Vector;
Vector create_vector(double* data, size_t size);


#endif
