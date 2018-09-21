#include <vector.h>


Vector *Vector_create()
{
	Vector *vector = MALLOC(Vector);
	vector->size = 0;
	vector->data = NULL;
	vector->capacity = 0;
	return vector;
}

void Vector_push(Vector *vector, void *ptr)
{
	if (vector->capacity == 0) {
		vector->capacity = 8;
		vector->data = MALLOCN(void*, vector->capacity);
	}
	if (vector->size == vector->capacity) {
		vector->capacity *= 2;
		vector->data = realloc(vector->data, vector->capacity);
	}
	vector->data[vector->size++] = ptr;
}

void *Vector_pop(Vector *vector)
{
	return vector->data[vector->size--];
}

void Vector_destroy(Vector *vector)
{
	assert(vector->size == 0);
	free(vector->data);
	free(vector);
}
