#include "graph.h"

Graph *makeGraph(int neighbors) {
	Graph *g = calloc(1, sizeof(Graph));
	g->neighbors = calloc(neighbors, sizeof(Graph*));
	g->maxNeighbors = neighbors;
	return g;
}

void freeGraphPoint(Graph *g) {
	free(g->data);
	free(g->neighbors);
	free(g);
}
