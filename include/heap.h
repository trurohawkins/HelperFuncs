#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef struct {
	void *data;
	void *scratch;

	size_t elementSize;
	size_t count;
	size_t capacity;

	int (*compare)(const void *, const void *, void *); //a , b, context
	void *compareContext;
} Heap;


bool heapInit(Heap *heap, size_t elementSize, size_t internalCapacity, int(*cmpFunc)(const void*, const void *, void*), void *cmpContext);
bool initMinHeap(Heap *heap, size_t elementSize, size_t internalCapacity);

bool heapReserve(Heap *heap, size_t capacity);
void heapDestroy(Heap *heap);
bool heapPush(Heap *heap, const void *element);
void *heapPeek(Heap *heap);
bool heapPop(Heap *heap, void *out);
bool heapRemoveAt(Heap *heap, size_t index, void *out);

bool heapIsEmpty(const Heap *heap);
void heapClear(Heap *heap);
/*

const void *heapPeekConst(const Heap *heap);


size_t heapCount(const Heap *heap);
size_t heapCapacity(const Heap *heap);
*/
