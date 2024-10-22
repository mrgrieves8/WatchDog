
#include <stdlib.h> /* size_t malloc() realloc() free() */
#include <assert.h> /* assert() */

#include "dvector.h"

#define GROWTHFACTOR 2

struct dvector
{
    size_t capacity;
    size_t size;
    size_t element_size;
    char *buffer;
};

static int IsVectorFull(const dvector_t *vector);

dvector_t *DvectorCreate(size_t capacity, size_t element_size)
{
	size_t buffer_size = 0;
	dvector_t *vector_ptr = NULL;
	
	assert(capacity > 0);
	assert(element_size > 0);
	
	vector_ptr = (dvector_t *)malloc(sizeof(dvector_t));
	buffer_size = sizeof(char) * capacity * element_size;
	
	if(NULL == vector_ptr)
	{
		return NULL;
	}
	
	vector_ptr->capacity = capacity;
	vector_ptr->element_size = element_size;
	vector_ptr->size = 0;
	vector_ptr->buffer = (char *)malloc(buffer_size);
	
	if(NULL == vector_ptr->buffer)
	{
		free(vector_ptr);
		return NULL;
	}
	
	return vector_ptr;
}

void DvectorDestroy(dvector_t *vector)
{
	assert(vector);
	
	free(vector->buffer);
	free(vector);
	vector = NULL;
}

int DvectorPushBack(dvector_t *vector, const void *new_element)
{
	char *char_new_element = (char *)new_element;
	size_t i = 0;
	size_t new_index = 0;
	int status = 0;
	
	assert(vector);
	assert(new_element);
	
	new_index = vector->size * vector->element_size;
	
	if (IsVectorFull(vector))
	{
		status = DvectorReserve(vector, (vector->capacity * GROWTHFACTOR));
		if (0 != status)
		{
			return status;
		}
	}
	
	for (; i < vector->element_size; ++i , ++char_new_element)
	{
		vector->buffer[new_index + i] = *char_new_element;
	}

	++(vector->size);
	
	return status;
}

void DvectorPopBack(dvector_t *vector)
{
	assert(vector);
	assert(vector->size > 0);
	--(vector->size);
}
     
int DvectorShrink(dvector_t *vector)
{
	assert(vector);
	return DvectorReserve(vector, vector->size);
}

int DvectorReserve(dvector_t *vector, size_t new_size)
{
	char *temp_buffer = NULL;
	
	assert(vector); 
	assert(new_size > 0);
	
	if (new_size < vector->capacity)
	{
		vector->size = new_size;
	}
	
	temp_buffer = (char *)realloc(vector->buffer,
									 (new_size * vector->element_size));
	if (NULL == temp_buffer)
	{
		free(vector->buffer);
		free(vector);
		return -1;
	}
	
	vector->buffer = temp_buffer;
	vector->capacity = new_size;
	return 0;
}

size_t DvectorSize(const dvector_t *vector)
{
	assert(vector);
	return (vector->size);
}

size_t DvectorCapacity(const dvector_t *vector)
{
	assert(vector);
	return (vector->capacity);
}

void *DvectorGetElement(const dvector_t *vector, size_t index)
{
	size_t vector_index = 0;
	
	assert(vector);
	assert(index < vector->size);
	
	vector_index = index * vector->element_size;
	
	return (vector->buffer + vector_index);
}

static int IsVectorFull(const dvector_t *vector)
{
	assert(vector);
	return (vector->size >= vector->capacity);
}




