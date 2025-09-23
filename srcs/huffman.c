#include "../lib/wood_pecker.h"

void print_heap(t_heap *h) {
	for (int i = 0; i < h->size; i++) {
		printf("%d\n", h->nodes[i]->freq);
	}
}

t_heap *create_min_heap(unsigned int capacity) {
	t_heap *new;

	new = malloc(sizeof(t_heap));
	if (new == NULL)
		return NULL;
	new->capacity = capacity;
	new->size = 0;
	new->nodes = calloc(capacity, sizeof(t_tree_node*));
	if (new->nodes == NULL) {
		free(new);
		return NULL;
	}
	return new;
}

t_tree_node *create_new_node(char c, unsigned int freq) {
	t_tree_node *new;

	new = malloc(sizeof(t_tree_node));
	if (new == NULL)
		return NULL;
	new->c = c;
	new->freq = freq;
	new->right = NULL;
	new->left = NULL;
	return new;
}

static void swap_nodes(t_tree_node **a, t_tree_node **b) {
	t_tree_node *tmp = *a;
    *a = *b;
    *b = tmp;
}

static void rearrange_heap(t_heap *h, unsigned int index) {
	if (index == 0)
		return;
	int parent = (index - 1) / 2;

	if (h->nodes[parent]->freq > h->nodes[index]->freq) {
		//swap
		swap_nodes(&h->nodes[parent], &h->nodes[index]);
		rearrange_heap(h, parent);
	}
}

static void min_heapify(t_heap *h, unsigned int index) {
	int right = (2 * index) + 2;
	int left = (2 * index) + 1;
	unsigned int smallest = index;

	if (left < h->size && h->nodes[left]->freq < h->nodes[smallest]->freq)
        smallest = left;

    if (right < h->size && h->nodes[right]->freq < h->nodes[smallest]->freq)
        smallest = right;

    if (smallest != index) {
        swap_nodes(&h->nodes[index], &h->nodes[smallest]);
        min_heapify(h, smallest);
	}
}

t_tree_node *extract_min(t_heap *h) {
	if (h->size == 0) {
		printf("Error cant delete item on empty heap\n");
		return NULL;
	}
	t_tree_node *delete = h->nodes[0];
	h->nodes[0] = h->nodes[h->size - 1];
	h->size--;
	min_heapify(h, 0);
	return delete;
}

void insert_heap(t_heap *h, t_tree_node *node) {
	if (h->size < h->capacity) {
		h->nodes[h->size] = node;
		rearrange_heap(h, h->size);
		h->size++;
	}
}

int huffman(char *str) {
	// printf("Enconding: %s\n", str);
	t_heap *heap = create_min_heap(50);
	t_tree_node *leaf_3 = create_new_node('a', 5);
	t_tree_node *leaf_0 = create_new_node('d', 1);
	t_tree_node *leaf_1 = create_new_node('c', 2);
	t_tree_node *leaf_2 = create_new_node('b', 3);
	insert_heap(heap, leaf_0);
	insert_heap(heap, leaf_3);
	insert_heap(heap, leaf_2);
	insert_heap(heap, leaf_1);
	print_heap(heap);
	extract_min(heap);
	print_heap(heap);
	return 0;
}