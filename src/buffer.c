#include <buffer.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

Buffer* Buffer_create(uint32_t align, uint32_t itemsize, const uint64_t size[])
{
	Buffer *buffer = MALLOC(Buffer);
	buffer->align = align;
	buffer->itemsize = itemsize;
	buffer->rawsize = itemsize;
	for (int i = 0; i<DIM; ++i) {
		buffer->rawsize *= size[i];
		buffer->size[i] = size[i];
	}
	buffer->data = malloc(buffer->rawsize);
	buffer->stride[0] = 1;
	for (int i = 1; i<DIM; ++i) {
		buffer->stride[i] = buffer->stride[i-1] * size[i-1];
	}
	buffer->refcount = 0;
	return buffer;
};

void Buffer_destroy(Buffer *buffer)
{
	assert(buffer->refcount == 0);
	free(buffer->data);
	free(buffer);
}

Array* Array_create(Buffer *buffer,	const uint64_t size[], const uint64_t pos[])
{
	Array *array = MALLOC(Array);
	array->buffer = buffer;
	buffer->refcount += 1;
	array->offset = 0;
	for (int i = 0; i<DIM; i += 1) {
		array->size[i] = size[i];
		array->pos[i] = pos[i];
	}
	return array;
}

void Array_destroy(Array *array)
{
	assert(array->buffer->refcount > 0);
	array->buffer->refcount -= 1;
	if (array->buffer->refcount == 0) {
		Buffer_destroy(array->buffer);
	}
	free(array);
}
