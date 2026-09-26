#include "helper.h"

#define LEN 7

int main() {
	Heap minHeap;
	if (!initMinHeap(&minHeap, sizeof(int), 8)) {
		printf("failed to initalize heap\n");
	}
	int *peek = heapPeek(&minHeap);
	if (!peek) {
		printf("nothing to peek at\n");
	}
	int arr[LEN] = {10, 12, 3, 1, 56, 69, 420};
	for (int i = 0; i < LEN; i++) {
		if (heapPush(&minHeap, arr+i)) {
			printf("pushed %i into heap\n", arr[i]);
		} else {
			printf("failed to push %i element into heap\n", i);
		}
	}
	peek = heapPeek(&minHeap);
	int peeked = *peek;
	int out;
	heapPop(&minHeap, &out);
	if (peek) {
		printf("peeked at %i and popped %i\n", peeked, out);
	}
	heapRemoveAt(&minHeap, 4, &out);
	printf("from index 4 we removed %i\n", out);
	printf("emptying heap\n");
	while (!heapIsEmpty(&minHeap)) {
		heapPop(&minHeap, &out);
		printf(" popped out %i\n", out);
	}

	heapDestroy(&minHeap);
	return 0;
}
