#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>

typedef struct Vector {
    void* data;
    size_t length;
    size_t capacity;
    size_t elementSize;
} Vector;

Vector* vector_init(size_t elementSize, size_t capacity);

bool vector_resize(struct Vector* vector, size_t newCapacity);
void vector_free(struct Vector* vector);

void vector_clear(struct Vector* vector);
void* vector_getData(const struct Vector* vector);
size_t vector_getLength(const struct Vector* vector);
size_t vector_getCapacity(const struct Vector* vector);
size_t vector_getElementSize(const struct Vector* vector);
void vector_set(struct Vector* vector, size_t index, const void* newElement);
void* vector_get(const Vector* vector, size_t index);
void* vector_push(struct Vector* vector, const void* element);
void* vector_pop(struct Vector* vector);

#endif
