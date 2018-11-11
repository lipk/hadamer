#pragma once

#define MALLOC(type) ((type*) malloc(sizeof(type)))
#define MALLOCN(type, n) ((type*) malloc(sizeof(type)*n))
#define DESTROY(type, var) type##_destroy(var); var = NULL

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define CONCAT2(x, y) x ## y
#define CONCAT(x, y) CONCAT2(x, y)
