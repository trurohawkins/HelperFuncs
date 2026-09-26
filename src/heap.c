#include "heap.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

static bool grow(Heap *heap);
static void *elementAt(Heap *heap, size_t index);
static void heapifyUp(Heap *heap, size_t index);
static void swap(Heap *heap, size_t a, size_t b);
/*
static void heapifyDown(Heap *heap, size_t index);
*/
int compareIntMin(const void *a, const void *b, void *context);


bool heapInit(Heap *heap, size_t elementSize, size_t internalCapacity, int(*cmpFunc)(const void*, const void *, void*), void *cmpContext) {
	if (heap == NULL || elementSize == 0 || cmpFunc == NULL) {
		return false;
	}
	heap->data = NULL;
	heap->elementSize = elementSize;
	heap->count = 0;
	heap->capacity = 0;
	heap->compare = cmpFunc;
	heap->compareContext = cmpContext;

	if (internalCapacity > 0) {
		if (!heapReserve(heap, internalCapacity)) {
			heap->elementSize = 0;
			heap->compare = NULL;
			heap->compareContext = NULL;
			return false;
		}
	}
	heap->scratch = calloc(1, elementSize);
	return true;
}

bool initMinHeap(Heap *heap, size_t elementSize, size_t internalCapacity) {
	return heapInit(heap, elementSize, internalCapacity, compareIntMin, 0);
}


bool heapReserve(Heap *heap, size_t capacity) {
	if (heap == NULL) {
		return false;
	}
	if (capacity <= heap->capacity) {
		return true;
	}
	//check multplication overflow
	if (capacity > SIZE_MAX / heap->elementSize) {
		return false;
	}
	size_t bytes = capacity * heap->elementSize;
	void *newData = realloc(heap->data, bytes);
	if (newData == NULL) {
		return false;
	}
	heap->data = newData;
	heap->capacity = capacity;
	return true;
}

void heapDestroy(Heap *heap) {
	if (heap == NULL) {
		return;
	}
	free(heap->data);
	free(heap->scratch);

	heap->data = 0;
	heap->elementSize = 0;
	heap->count = 0;
	heap->capacity = 0;
	heap->compare = NULL;
	heap->compareContext = NULL;
}

bool heapPush(Heap *heap, const void *element) {
	if (heap == NULL || element == NULL) {
		return false;
	}
	if (heap->count == heap->capacity) {
		if (!grow(heap)) {
			return false;
		}
	}
	void *destination = elementAt(heap, heap->count);
	memcpy(destination, element, heap->elementSize);
	heap->count++;
	heapifyUp(heap, heap->count - 1);
	return true;
}

static bool grow(Heap *heap) {
	size_t newCapacity;
	if (heap->capacity == 0) {
		newCapacity = 16;
	} else {
		if (heap->capacity > SIZE_MAX / 2) {
			return false;
		}
		newCapacity = heap->capacity * 2;
	}
	return heapReserve(heap, newCapacity);
}

static void *elementAt(Heap *heap, size_t index) {
	return (char*)heap->data + index * heap->elementSize;
}

static void swap(Heap *heap, size_t a, size_t b) {
	if (a == b) {
		return;
	}
	void *elementA = elementAt(heap, a);
	void *elementB = elementAt(heap, b);
	memcpy(heap->scratch, elementA, heap->elementSize);
	memcpy(elementA, elementB, heap->elementSize);
	memcpy(elementB, heap->scratch, heap->elementSize);
}

static void heapifyUp(Heap *heap, size_t index) {
	while (index > 0) {
		size_t parent = (index - 1) / 2;

		void *childElement = elementAt(heap, index);
		void *parentElement = elementAt(heap, parent);

		// if parent already at higher pirority, heap property is satsified
		if (heap->compare(childElement, parentElement, heap->compareContext) >= 0){
			break;
		}
		swap(heap, index, parent);
		index = parent;
	}
}

static void heapifyDown(Heap *heap, size_t index) {
	while (true) {
		size_t left = index * 2 + 1;
		size_t right = index * 2 + 2;

		if (left >= heap->count) {
			break;
		}
		//assume left child has higher priority
		size_t highestPriority = left;
		if (right < heap->count) {
			void *leftElement = elementAt(heap, left);
			void *rightElement = elementAt(heap, right);
			if (heap->compare(rightElement, leftElement, heap->compareContext) < 0) {
				highestPriority = right;
			}
		}
		void *currentElement = elementAt(heap, index);
		void *childElement = elementAt(heap, highestPriority);
		//current element already has higher priority
		if (heap->compare(childElement, currentElement, heap->compareContext) >= 0) {
			break;
		}
		swap(heap, index, highestPriority);
		index = highestPriority;
	}
}

void *heapPeek(Heap *heap) {
	if (heap == NULL || heap->count == 0) {
		return NULL;
	} else {
		return elementAt(heap, 0);
	}
}

bool heapPop(Heap *heap, void *out) {
	if (heap == NULL || heap->count == 0) {
		return false;
	}
	if (out != NULL) {
		memcpy(out, elementAt(heap, 0), heap->elementSize);
	} 
	heap->count--;
	//if root was only element were finished
	if (heap->count == 0) {
		return true;
	}
	//move last element into root
	memcpy(elementAt(heap, 0), elementAt(heap, heap->count), heap->elementSize);
	heapifyDown(heap, 0);
	return true;
}

bool heapRemoveAt(Heap *heap, size_t index, void *out) {
	if (heap == NULL || index >= heap->count) {
		return false;
	}
	// return removed element to caller
	if (out != NULL) {
		memcpy(out, elementAt(heap, index), heap->elementSize);
	}
	heap->count--;
	// Removing the last element requires no further work
	if (index == heap->count) {
		return true;
	}
	// move final element into removed spot
	memcpy(elementAt(heap, index), elementAt(heap, heap->count), heap->elementSize);
	//replacement might need to move up or down
	if (index > 0) {
		size_t parent = (index - 1) / 2;
		void *replacement = elementAt(heap, index);
		void *parentElement = elementAt(heap, parent);
		if (heap->compare(replacement, parentElement, heap->compareContext) < 0) {
			heapifyUp(heap, index);
			return true;
		}
	} else {
		heapifyDown(heap, index);
	}
	return true;
}

bool heapIsEmpty(const Heap *heap) {
	return heap == NULL || heap->count == 0;
}

void heapClear(Heap *heap) {
		if (heap == NULL) {
			return;
		} else {
			heap->count = 0;
		}
}

int compareIntMin(const void *a, const void *b, void *context) {
	int x = *(const int *)a;
	int y = *(const int *)b;

	if (x < y) {
		return -1;
	} else if (x > y) {
		return 1;
	} else {
		return 0;
	}
}

