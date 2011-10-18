/*
 *
 * AVL Tree
 *
 */

#include "avl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int ptr_keycmp(void *k1, void *k2) {
	if (k1 > k2) {
		return 1;
	} 
	if (k1 < k2) {
		return -1;
	}
	return 0;
}

avl_tree * avl_construct() {
	return avl_construct_with_keycmp(&ptr_keycmp);
}

avl_tree * avl_construct_with_keycmp(int (*keycmp)(void *k1, void *k2)) {
	avl_tree *tree = (avl_tree *)malloc(sizeof(avl_tree));
	if (tree != NULL) {
		tree->root = NULL;
		tree->keycmp = keycmp;
	}
	return tree;
}

void * avl_get(avl_tree *tree, void *key) {
	if (tree->root == NULL) {
		return NULL;
	}

	avl_node *node = tree->root;
	
	while (1) {
		int c = tree->keycmp(node->key, key);
		
		if (c < 0) {
			if (node->left == NULL) {
				return NULL;
			}
			node = node->left;
		} else if (c > 0) {
			if (node->right == NULL) {
				return NULL;
			}
			node = node->right;
		} else {
			return node->val;
		}
	}
}

static avl_node * avl_node_construct(avl_node *parent, void *key, void *val) {
	avl_node *node = (avl_node *)malloc(sizeof(avl_node));
	if (node != NULL) {
		node->parent = parent;
		node->key = key;
		node->val = val;
		node->height = 0;
		node->left = node->right = NULL;
	}
	return node;
}

static int max(int x, int y) {
	return x > y ? x : y;
}

static int height(avl_node *node) {
	return node == NULL ? -1 : node->height;
}

static int balancing_factor(avl_node *node) {
	return node == NULL ? 0 : height(node->right) - height(node->left);
}

static avl_node * rotate_right(avl_tree *tree, avl_node *node) {
	avl_node *left = node->left;
	node->left = left->right;
	
	if (left->right != NULL) {
		left->right->parent = node;
	}
	
	left->parent = node->parent;
	
	if (node->parent == NULL) {
		tree->root = left;
	} else if (node->parent->right == node) {
		node->parent->right = left;
	} else {
		node->parent->left = left;
	}
	
	left->right = node;
	node->parent = left;     
        
	node->height = max(height(node->left), height(node->right)) + 1;
	left->height = max(height(left->left), node->height) + 1;
	
	return left;
}

static avl_node * rotate_left(avl_tree *tree, avl_node *node) {
	avl_node *right = node->right;
	node->right = right->left;
	
	if (right->left != NULL) {
		right->left->parent = node;
	}
	
	right->parent = node->parent;
	
	if (node->parent == NULL) {
		tree->root = right;
	} else if (node->parent->left == node) {
		node->parent->left = right;
	} else {
		node->parent->right = right;
	}
	
	right->left = node;
	node->parent = right;     
        
	node->height = max(height(node->right), height(node->left)) + 1;
	right->height = max(height(right->right), node->height) + 1;
	
	return right;
}

static avl_node * double_rotate_right(avl_tree *tree, avl_node *node) {
	node->left = rotate_left(tree, node->left);
	return rotate_right(tree, node);
}

static avl_node * double_rotate_left(avl_tree *tree, avl_node *node) {
	node->right = rotate_right(tree, node->right);
	return rotate_left(tree, node);
}

static void avl_balance(avl_tree *tree, avl_node *node) {
	while (node != NULL) {
		int b = balancing_factor(node);
		
		if (b > 1) {
			if (balancing_factor(node->right) >= 0) {
				node = rotate_left(tree, node);
			} else {
				node = double_rotate_left(tree, node);
			}
		} else if (b < -1) {
			if (balancing_factor(node->left) <= 0) {
				node = rotate_right(tree, node);
			} else {
				node = double_rotate_right(tree, node);
			}
		} else {
			node->height = max(height(node->left), height(node->right)) + 1;
		}
		
		node = node->parent;
	}
}

void * avl_put(avl_tree *tree, void *key, void *val) {
	if (tree->root == NULL) {
		tree->root = avl_node_construct(NULL, key, val);
		return NULL;
	} 
	
	avl_node *node = tree->root;
	
	while (1) {
		int c = tree->keycmp(node->key, key);
		
		if (c < 0) {
			if (node->left == NULL) {
				node->left = avl_node_construct(node, key, val);
				avl_balance(tree, node);
				return NULL;
			}
			node = node->left;
		} else if (c > 0) {
			if (node->right == NULL) {
				node->right = avl_node_construct(node, key, val);
				avl_balance(tree, node);
				return NULL;
			}
			node = node->right;
		} else {
			void *prev_val = node->val;
			node->val = val;
			return prev_val;
		}
	}
}

static void avl_node_destruct(avl_node *node) {
	if (node == NULL) {
		return;
	}
	
	avl_node_destruct(node->left);
	avl_node_destruct(node->right);

	free(node);
}

void avl_destruct(avl_tree *tree) {
	avl_node_destruct(tree->root);
	free(tree);
}
