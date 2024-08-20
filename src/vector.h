#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>

typedef struct Vector {
    void* data;
    size_t length;
    size_t capacity;
    size_t elementSize;

    void (*clear)(struct Vector* vector);
    void* (*getData)(const struct Vector* vector);
    size_t (*getLength)(const struct Vector* vector);
    size_t (*getCapacity)(const struct Vector* vector);
    size_t (*getElementSize)(const struct Vector* vector);
    void (*set)(struct Vector* vector, size_t index, const void* newElement);
    void* (*get)(const struct Vector* vector, size_t index);
    void* (*push)(struct Vector* vector, const void* element);
    void* (*pop)(struct Vector* vector);
    bool (*resize)(struct Vector* vector, size_t newCapacity);
    void (*free)(struct Vector* vector);
} Vector;

Vector* vector_init(size_t elementSize, size_t capacity);
#endif
