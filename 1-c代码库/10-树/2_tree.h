#ifndef _TREE_H
#define _TREE_H 

#include <stdio.h>
#include <stdlib.h>

typedef struct tree{
	int data; 				//编号
	struct tree *lchild;
	struct tree *rchild;
}linktree_t;

linktree_t * linktree_create(int n, int i);

#endif



