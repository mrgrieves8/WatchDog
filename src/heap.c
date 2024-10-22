
#include <stdlib.h> /* malloc(), free() */
#include <string.h> /* memcpy() */
#include <assert.h> /* assert() */

#include "dvector.h"
#include "heap.h"

#define INITIAL_CAPACITY 16

struct heap
{
	dvector_t *vector;
	heap_compare_func_t compare_func;
};

static void HeapifyUp(heap_t *heap, size_t index);
static void HeapifyDown(heap_t *heap, size_t index);
static size_t GetSmallerChildIndex(heap_t *heap, size_t index);
static void Swap(void *data1, void *data2);

static int dummy = 0;

heap_t *HeapCreate(heap_compare_func_t compare_func)
{
	heap_t *heap = NULL;
	int status = 0;
	
	assert(compare_func);
	
	heap = (heap_t *)malloc(sizeof (heap_t));
	
	if(NULL == heap)
	{
		return NULL;
	}
	
	heap->vector = DvectorCreate(INITIAL_CAPACITY, sizeof(void *));
	
	if(NULL == heap->vector)
	{
		free(heap);
		return NULL;
	}
	
	status = DvectorPushBack(heap->vector, &dummy);
	
	if(status)
	{
		DvectorDestroy(heap->vector);
		free(heap);
		return NULL;
	}
	
	heap->compare_func = compare_func;
	
	return heap;
}

void HeapDestroy(heap_t *heap)
{
	assert(heap);
	
	DvectorDestroy(heap->vector);
	free(heap);
}

int HeapPush(heap_t *heap, void *data)
{
	int status = 0;
	
	assert(heap);
	
	status = DvectorPushBack(heap->vector, &data);
	
	HeapifyUp(heap, DvectorSize(heap->vector) - 1);
	
	return status;
}

void HeapPop(heap_t *heap)
{
	assert(heap);
	
	Swap(DvectorGetElement(heap->vector, 1), 
			DvectorGetElement(heap->vector, HeapSize(heap)));
	DvectorPopBack(heap->vector);
	HeapifyDown(heap, 1);
}

void *HeapPeek(const heap_t *heap)
{
	void **ret = NULL;
	
	assert(heap);
	
	ret = DvectorGetElement(heap->vector, 1);
	return *ret;
}

size_t HeapSize(const heap_t *heap)
{
	assert(heap);
	
	return (DvectorSize(heap->vector) - 1);
}

int HeapIsEmpty(const heap_t *heap)
{
	assert(heap);
	
	return HeapSize(heap) == 0;
}

void *HeapRemove(heap_t *heap, is_match_func_t is_match, void *data_to_match)
{
	size_t index = 1;
	size_t size = 0;
	void *ret = NULL;
	
	assert(heap);
	assert(is_match);
	
	size = HeapSize(heap);
	
	while (index <= size && is_match(*(void **)DvectorGetElement(heap->vector, index),
										data_to_match) != 1)
	{
		++index;
	}
	
	if (index > size)
	{
		return NULL;
	}
	
	
	ret = *(void **)DvectorGetElement(heap->vector, index);
	Swap(DvectorGetElement(heap->vector, index), 
			DvectorGetElement(heap->vector, HeapSize(heap)));
	
	DvectorPopBack(heap->vector);
	HeapifyDown(heap, index);
	
	return ret;
}


/************************* STATIC HELPER FUNCTIONS ************************/

static void HeapifyUp(heap_t *heap, size_t index)
{
	void **node = NULL;
	void **parent = NULL;
	size_t parent_index = index/2;
	
	assert(heap);
	
	node = (void **)DvectorGetElement(heap->vector, index);
	parent = (void **)DvectorGetElement(heap->vector, parent_index);
	
	while (parent_index != 0 && heap->compare_func(*parent, *node) > 0)
	{
		Swap(parent, node);
		
		parent_index /= 2;
		node = parent;
		parent = (void **)DvectorGetElement(heap->vector, parent_index);
	}
}

static void HeapifyDown(heap_t *heap, size_t index)
{
	void **node = NULL;
	void **child = NULL;
	size_t child_index = 0;
	size_t size = 0;
	
	assert(heap);
	
	child_index = GetSmallerChildIndex(heap, index);
	
	size = HeapSize(heap);
	
	if (child_index <= size)
	{
		node = (void **)DvectorGetElement(heap->vector, index);
		child = (void **)DvectorGetElement(heap->vector, child_index);
	}
	
	while (child_index <= size)
	{
		node = (void **)DvectorGetElement(heap->vector, index);
		child = (void **)DvectorGetElement(heap->vector, child_index);
		if (heap->compare_func(*node, *child) > 0)
		{
			Swap(node, child);
		}
		index = child_index;
		child_index = GetSmallerChildIndex(heap, index);
	}
}

static void Swap(void *data1, void *data2)
{
	char *buffer = NULL; 
	
	buffer = (char *)malloc(sizeof(void *));
	
	if (!buffer)
	{
		return;
	}
	
    memcpy(buffer, data1, sizeof(void *));
    memcpy(data1, data2, sizeof(void *));
    memcpy(data2, buffer, sizeof(void *));
    
	free(buffer);
}

static size_t GetSmallerChildIndex(heap_t *heap, size_t index)
{
	void **left_child = NULL;
	void **right_child = NULL;
	
	assert(heap);
	
	if (HeapSize(heap) <= 2 * index)
	{
		return 2 * index;
	}
	else
	{
		left_child = (void **)DvectorGetElement(heap->vector, 2*index);
		right_child = (void **)DvectorGetElement(heap->vector, 2*index + 1);
		return (heap->compare_func(*left_child, *right_child) < 0) ? 
											2*index : 2*index + 1;
	}
	
}











