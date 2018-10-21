//图
#include <stdio.h>
#include <stdlib.h>

#define N 5

//图的类型
typedef struct {
	int data[N];
	int matrix[N][N];
}graph_t; 				

//创建没有关系的图
graph_t * graph_create()
{
	graph_t * g = (graph_t*)malloc(sizeof(graph_t)); 		//开辟图的空间

	int i;
	for(i=0; i<N; i++)
	{
		g->data[i] = i;
	}

	return g;
}

//输入图的关系
int graph_input(graph_t *g)
{
	int i,j;
	while(scanf("(V%d,V%d)",&i,&j) == 2)		//(V0,V1)					//无向图的输入格式
	{
		getchar(); 								//吃掉一个垃圾字符
		g->matrix[i][j] = 1;
		g->matrix[j][i] = 1;
	}

	return 0;
}

//输出图的关系
int graph_output(graph_t *g)
{
	int i, j;
	printf("   V0 V1 V2 V3 V4\n");

	for(i=0; i<N; i++)
	{
		printf("V%d ",i);
		for(j=0; j<N; j++)
		{
			printf(" %d ",g->matrix[i][j]);
		}
		putchar(10);
	}

	return 0;
}



int main(int argc, const char *argv[])
{
	graph_t *g = graph_create();
	graph_input(g);
	graph_output(g);

	
	return 0;
}
