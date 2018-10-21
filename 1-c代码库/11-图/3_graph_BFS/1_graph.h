#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdio.h>
#include <stdlib.h>

#define N 9

//图的类型
typedef struct {
	int data[N];
	int matrix[N][N];
}graph_t; 				

graph_t * graph_create();
int graph_input(graph_t *g);

#endif
