//创建完全二叉树
#include "2_tree.h"
#include "4_linkqueue.h"

linktree_t * linktree_create(int n, int i) 					
{
	linktree_t *root = (linktree_t*)malloc(sizeof(linktree_t));
	root->data = i;

	if(2*i <= n)
	{
		root->lchild = linktree_create(n, 2*i);
	}
	else
	{
		root->lchild = NULL;
	}

	if(2*i+1 <= n)
	{
		root->rchild = linktree_create(n, 2*i+1);
	}
	else
	{
		root->rchild = NULL;
	}

	return root;
}

//层次遍历
int level_order(linktree_t *root)
{
	linkqueue_t *queue = linkqueue_create(); 	//创建队列
	linkqueue_input(queue, root); 				//根节点入队

	while(!linkqueue_is_empty(queue)) 			//空：return 1
	{
		root = linkqueue_output(queue);
		printf("%d  ",root->data);

		if(root->lchild != NULL)
		{
			linkqueue_input(queue, root->lchild);
		}

		if(root->rchild != NULL)
		{
			linkqueue_input(queue, root->rchild);
		}
	}

	return 0;
}




int main(int argc, const char *argv[])
{
	int n = 8; 			//总节点数
	int i = 1; 			//根节点的标号
	linktree_t *root = linktree_create(n,i);

	//层次遍历访问
	level_order(root);
	putchar(10);
	
	return 0;
}



