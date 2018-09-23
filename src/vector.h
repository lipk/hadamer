#pragma once

#include <stdint.h>
#include <config.h>

typedef struct Vector
{
	void **data;
	uint64_t size;
	uint64_t capacity;
} Vector;

Vector *Vector_create();
void Vector_init(Vector *vector);
void Vector_destroy(Vector *vector);
void Vector_push(Vector *vector, void *ptr);
void* Vector_pop(Vector *vector);
void* Vector_swapErase(Vector *vector, uint64_t i);
void Vector_clear(Vector *vector);
void Vector_resize(Vector *vector, uint64_t size);
