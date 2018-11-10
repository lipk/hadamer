#include <vector.h>
#include <util.h>

Vector *Vector_create()
{
    Vector *vector = MALLOC(Vector);
    Vector_init(vector);
    return vector;
}

void Vector_push(Vector *vector, void *ptr)
{
    Vector_resize(vector, vector->size+1);
    vector->data[vector->size-1] = ptr;
}

void *Vector_pop(Vector *vector)
{
    assert(vector->size != 0);
    vector->size--;
    return vector->data[vector->size];
}

void Vector_destroy(Vector *vector)
{
    free(vector->data);
    free(vector);
}

void *Vector_swapErase(Vector *vector, uint64_t i)
{
    assert(vector->size > i);
    void *tmp = vector->data[vector->size-1];
    vector->data[vector->size-1] = vector->data[i];
    vector->data[i] = tmp;
    return Vector_pop(vector);
}

void Vector_clear(Vector *vector)
{
    vector->size = 0;
}

void Vector_init(Vector *vector)
{
    vector->size = 0;
    vector->capacity = 0;
    vector->data = NULL;
}

void Vector_resize(Vector *vector, uint64_t size)
{
    if (vector->capacity == 0) {
        vector->capacity = 8;
        vector->data = MALLOCN(void*, vector->capacity);
    }
    if (vector->capacity < size) {
        while (vector->capacity < size) {
            vector->capacity *= 2;
        }
        vector->data = realloc(vector->data, sizeof(void*)*vector->capacity);
        *vector->data;
    }
    vector->size = size;
}
