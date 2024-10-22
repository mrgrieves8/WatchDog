#include <assert.h> /* assert() */
#include <stdlib.h> /* malloc(), free() */

#include "heap_pq.h"
#include "heap.h"

struct heap_pq
{
	heap_t *heap;
};


heap_pq_t *HeapPQCreate(heap_pq_compare_func_t cmp_func)
{
	heap_pq_t *queue = NULL;
	
	assert(cmp_func);
	
	queue = (heap_pq_t *)malloc(sizeof(heap_pq_t));
		
	if (NULL == queue)
	{
		return NULL;
	}
	
	queue->heap = HeapCreate(cmp_func);
	
	if (NULL == queue->heap)
	{
		free(queue);
		return NULL;
	}
	
	return queue;
}

void HeapPQDestroy(heap_pq_t *queue)
{
	assert(queue);
	assert(queue->heap);
	
	HeapDestroy(queue->heap);
	free(queue);
	queue = NULL;
}

int HeapPQEnqueue(heap_pq_t *queue, void *data)
{
	int status = 0;
	assert(queue);
	assert(queue->heap);
	
	status = HeapPush(queue->heap, data);
	
	return status;
}

void *HeapPQDequeue(heap_pq_t *queue)
{
	void *ret = NULL;
	assert(queue);
	assert(queue->heap);
	
	ret = HeapPeek(queue->heap);
	HeapPop(queue->heap);
	
	return ret;
}

void *HeapPQPeek(const heap_pq_t *queue)
{
	assert(queue);
	assert(queue->heap);
	
	return HeapPeek(queue->heap);
}

int HeapPQIsEmpty(const heap_pq_t *queue)
{
	assert(queue);
	assert(queue->heap);
	
	return HeapIsEmpty(queue->heap);
}

size_t HeapPQSize(const heap_pq_t *queue)
{
	assert(queue);
	assert(queue->heap);
	
	return HeapSize(queue->heap);
}

void HeapPQClear(heap_pq_t *queue)
{
	assert(queue);
	
	while(!HeapPQIsEmpty(queue))
	{
		HeapPQDequeue(queue);
	}
}

void *HeapPQErase(heap_pq_t *queue, heap_pq_match_func_t is_match, void *data)
{	
	assert(queue);
	
	return HeapRemove(queue->heap, is_match, data);
}

