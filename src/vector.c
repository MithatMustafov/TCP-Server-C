#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include "vector.h"

#define VECTOR_INIT_CAPACITY 1;

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

struct Vector* vector_init(size_t elementSize, size_t capacity)
{
    assert(elementSize > 0);

    struct Vector *vector = malloc(sizeof(struct Vector));
    
    if (!vector) { return NULL; }

    vector->length = 0;
    vector->capacity = (capacity > 0) ? capacity : VECTOR_INIT_CAPACITY;
    vector->elementSize = elementSize;
    vector->data = malloc(elementSize * vector->capacity);
    
    
    if (!vector->data)
    {
        free(vector);
        return NULL;
    }

    vector->clear = vector_clear;
    vector->getData = vector_getData;
    vector->getLength = vector_getLength;
    vector->getCapacity = vector_getCapacity;
    vector->getElementSize = vector_getElementSize;
    vector->set = vector_set;
    vector->get = vector_get;
    vector->push = vector_push;
    vector->pop = vector_pop;

    return vector;
}

void vector_free(struct Vector* vector)
{
    assert(vector != NULL);

    if(vector->data)
    {
        free(vector->data);
        vector->data = NULL;
    }

    free(vector);
    vector = NULL;
}

void vector_clear(struct Vector* vector)
{
    assert(vector != NULL);

    vector->length = 0;
}

void* vector_getData(const struct Vector* vector)
{
    assert(vector != NULL);

    return vector->data;
}

size_t vector_getLength(const struct Vector* vector)
{
    assert(vector != NULL);

    return vector->length;
}


size_t vector_getCapacity(const struct Vector* vector)
{
    assert(vector != NULL);

    return vector->capacity;
}

size_t vector_getElementSize(const struct Vector* vector)
{
    assert(vector != NULL);

    return vector->elementSize;
}

void vector_set(struct Vector* vector, size_t index, const void* newElement)
{
    assert(vector != NULL);
    assert(index < vector->length);

    char* destination = (char *)vector->data + (index * vector->elementSize);
    const char* source = (const char *)newElement;

    for (size_t i = 0; i<vector->elementSize; i++)
    {
        destination[i] = source[i];
    }
}

void* vector_get(const struct Vector* vector, size_t index)
{
    assert(vector != NULL);
    assert(index < vector->length);

    return (char *)vector->data + (index * vector->elementSize);
}

void* vector_push(struct Vector* vector, const void* element)
{
    assert(vector != NULL);

    if (vector->length >= vector->capacity
        && !vector_resize(vector, vector->capacity * 2))
    {
        return 0; 
    }

    char* destination = (char *)vector->data + (vector->length * vector->elementSize);
    const char* source = (const char*)element;

    for (size_t i = 0; i < vector->elementSize; i++)
    {
        destination[i] = source[i];
    }

    vector->length++;
}

bool vector_resize(struct Vector *vector, size_t newCapacity)
{
    void *newData = realloc(vector->data, vector->elementSize * newCapacity);
    if (!newData) { return false; }

    vector->data = newData;
    vector->capacity = newCapacity;

    return true;
}

void* vector_pop(struct Vector* vector)
{
    assert(vector != NULL);
    assert(vector->length > 0);

    vector->length--;

    return (char *)vector->data + (vector->length * vector->elementSize);
}
