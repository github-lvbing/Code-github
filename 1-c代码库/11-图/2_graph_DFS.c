//深度优先搜索
#include <stdio.h>
#include <stdlib.h>

#define N 9

//图的类型
typedef struct {
	int data[N];
	int matrix[N][N];
}graph_t; 				

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

//找第一个临接点
int graph_find_first(graph_t *g, int v)
{
	int i;
	for(i=0; i<N; i++)
	{
		if(g->matrix[v][i] == 1) 		//判断是否有关系
		{
			return i; 					//返回第一个临接点的下标
		}
	}
	return -1;
}

//找第二个临接点
int graph_find_next(graph_t *g, int v, int f)
{
	int i;
	for(i=f+1; i<N; i++)
	{
		if(g->matrix[v][i] == 1)
		{
			return i;
		}
	}
	return -1;
}

//深度优先搜索
int graph_DFS(graph_t *g, int v)
{
	int f;
	printf("%d  ",v); 					//v：每一个节点的标号
	sign[v] = 1;

	f = graph_find_first(g,v); 
	while(f >= 0) 						//真：代表有该临接点
	{
		if(sign[f] == 0) 				//真：该临接点没有被标记
		{
			graph_DFS(g, f); 			//标记并深度访问
		}
		f = graph_find_next(g,v,f); 	//标记完后找下一个临接点
	}
	return 0;
}


int main(int argc, const char *argv[])
{
	graph_t *g = graph_create();
	graph_input(g);

	graph_DFS(g,0);
	putchar(10);

	
	return 0;
}
