#include "avl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

// helpers

static int max(int x, int y) {
	return x > y ? x : y;
}

static int height(avl_node *node) {
	return node == NULL ? -1 : node->height;
}

static int balancing_factor(avl_node *node) {
	return node == NULL ? 0 : height(node->right) - height(node->left);
}

// invariants

void assert_bst_node_invariant(avl_tree *tree, avl_node *node) {
	if (node->left != NULL) {
		assert(tree->keycmp(node->key, node->left->key) < 0);
		assert_bst_node_invariant(tree, node->left);
	}
	if (node->right != NULL) {
		assert(tree->keycmp(node->key, node->right->key) > 0);
		assert_bst_node_invariant(tree, node->right);
	}
}

void assert_bst_invariant(avl_tree *tree) {
	if (tree->root != NULL) {
		assert_bst_node_invariant(tree, tree->root);
	}
}

int assert_node_height_invariant(avl_node *node) {
	if (node == NULL) {
		return 0;
	}
	
	int l_height = assert_node_height_invariant(node->left);
	int r_height = assert_node_height_invariant(node->right);

	int height = max(l_height, r_height);

	assert(height == node->height);
	
	return height + 1;
}

void assert_height_invariant(avl_tree *tree) {
	if (tree->root != NULL) {
		assert_node_height_invariant(tree->root);
	}
}

void assert_avl_node_invariant(avl_node *node) {
	if (node == NULL) {
		return;
	}
	
	int b = balancing_factor(node);
	assert(b == -1 || b == 0 || b == 1);
	
	assert_avl_node_invariant(node->left);
	assert_avl_node_invariant(node->right);
}

void assert_avl_tree_invariant(avl_tree *tree) {
	if (tree->root != NULL) {
		assert_avl_node_invariant(tree->root);
	}
}

// generate test data

int digits(int i) {
	return i == 0 ? 1 : floor(log10(i)) + 1;
}

char ** sample_data(int items) {
	char **data = (char **)malloc(items * sizeof(char *));
	
	if (data == NULL) {
		return NULL;
	}
	
	int i;
	int d;
	
	for (i = 0; i < items; i++) {
		d = digits(i);
		char *str = (char *)malloc(d * sizeof(char));
		if (str != NULL) {
			snprintf(str, d, "%i", i);	
			data[i] = str;
		}
	}
	
	return data;
}

// pretty print

void avl_node_print(avl_node *node, int depth) {
	if (node != NULL) {
		avl_node_print(node->right, depth + 1);
		printf("\n");
		int i;
		for (i = 0; i < depth - 1; i++) {
			printf("\t");
		}
		printf(" ---> %s", (char *)node->key);
		avl_node_print(node->left, depth + 1);
	}
}

void avl_tree_print(avl_tree *tree) {
	avl_node_print(tree->root, 1);
}

// strcmp based key comparison

int str_keycmp(void *k1, void *k2) {
	return strcmp((char *)k1, (char *)k2);
}

int main(void) {
	//avl_tree *tree = avl_construct();
	avl_tree *tree = avl_construct_with_keycmp(&str_keycmp);
	
	int items = 1000000;
	
	char **data = sample_data(items);
	
	int i;
	
	for (i = 0; i < items; i++) {
		avl_put(tree, data[i], data[i]);
	}
	
	for (i = 0; i < items; i++) {
		assert(strcmp(avl_get(tree, data[i]), data[i]) == 0);
	}
	
	assert_bst_invariant(tree);
	assert_height_invariant(tree);
	assert_avl_tree_invariant(tree);
	
	// avl_tree_print(tree);
	
	for (i = 0; i < items; i++) {
		free(data[i]);
	}
	free(data);
	
	avl_destruct(tree);
	
	return 0;
}