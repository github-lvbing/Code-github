//创建完全二叉树

#include <stdio.h>
#include <stdlib.h>

typedef struct tree{
	int data; 				//编号
	struct tree *lchild;
	struct tree *rchild;
}linktree_t;

//创建完全二叉树
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

//先序遍历访问
int linktree_pre_order(linktree_t *root)
{
	printf("%d  ",root->data);

	if(root->lchild != NULL)
	{
		linktree_pre_order(root->lchild); 			
	}

	if(root->rchild != NULL)
	{
		linktree_pre_order(root->rchild);
	}
	return 0;
}

//中序
int linktree_mid_order(linktree_t *root)
{

	if(root->lchild != NULL)
	{
		linktree_mid_order(root->lchild); 			
	}

	printf("%d  ",root->data);

	if(root->rchild != NULL)
	{
		linktree_mid_order(root->rchild);
	}

	return 0;
}



int main(int argc, const char *argv[])
{
	int n = 8; 			//总节点数
	int i = 1; 			//根节点的标号
	linktree_t *root = linktree_create(n,i);

	//先须遍历访问
	linktree_pre_order(root);
	putchar(10);

	//中序遍历访问
	linktree_mid_order(root);
	putchar(10);
	
	return 0;
}



