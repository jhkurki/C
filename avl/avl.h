#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct avl_node {
	void *key;
	void *val;
	int height;
	struct avl_node *parent;
	struct avl_node *left;
	struct avl_node *right;
} avl_node;

typedef struct avl_tree {
	avl_node *root;
	int (*keycmp)(void *k1, void *k2);
} avl_tree;

avl_tree * avl_construct();
avl_tree * avl_construct_with_keycmp(int (*keycmp)(void *k1, void *k2));

void * avl_get(avl_tree *tree, void *key);
void * avl_put(avl_tree *tree, void *key, void *val);

void avl_destruct(avl_tree *tree);

#endif