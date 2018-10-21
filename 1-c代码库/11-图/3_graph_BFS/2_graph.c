#include "1_graph.h"
#include "3_linkqueue.h"

int sign[N];

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

//广度优先搜索
int graph_BFS(graph_t *g, linkqueue_t *queue)
{
	int v,i;
	linkqueue_input(queue, 0);
	sign[0] = 1; 									//标记

	while(!linkqueue_is_empty(queue))
	{
		v = linkqueue_output(queue);
		printf("%d  ",v);

		for(i=0; i<N; i++)
		{
			if(g->matrix[v][i] == 1 && sign[i] == 0) 	//没有被访问过的邻接点入队
			{
				linkqueue_input(queue, i);
				sign[i] = 1; 							//标记
			}
		}
	}

	putchar(10);
	return 0;
}


int main(int argc, const char *argv[])
{
	graph_t *g = graph_create();
	graph_input(g);

	linkqueue_t *queue = linkqueue_create();
	graph_BFS(g,queue);



	
	return 0;
}
