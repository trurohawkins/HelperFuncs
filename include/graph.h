#pragma once
#include <stdlib.h>
typedef struct Graph {
	int maxNeighbors;
	struct Graph **neighbors;
	void *data;

	int lastVisit;
} Graph;

Graph *makeGraph(int neighbors);
void freeGraphPoint(Graph *g);
