#pragma once

typedef struct s_tree_node {
	char c;
	unsigned int freq;
	struct s_tree_node *left;
	struct s_tree_node *right;
} t_tree_node;

typedef struct s_heap {
	int size;
	int capacity;
	t_tree_node **nodes;
} t_heap;

t_heap *create_min_heap(unsigned int capacity);
t_tree_node *create_new_node(char c, unsigned int freq);
void insert_heap(t_heap *h, t_tree_node *node);
t_tree_node *extract_min(t_heap *h);
int huffman(char *str);
