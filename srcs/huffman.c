#include "../lib/wood_pecker.h"

static void print_heap(t_heap *h) {
	for (int i = 0; i < h->size; i++) {
		printf("%d\n", h->nodes[i]->freq);
	}
}

//helper function that I copied, sorry Morti
static void print_tree(t_tree_node *root, int depth) {
	if (root == NULL)
		return;

	// Print right subtree
	print_tree(root->right, depth + 1);

	// Indent based on depth
	for (int i = 0; i < depth; i++)
		printf("    ");  // 4 spaces per level

	if (root->c != '\0')  // leaf node (character)
		printf("'%c' (%d)\n", root->c, root->freq);
	else                  // internal node
		printf("• (%d)\n", root->freq);

	// Print left subtree
	print_tree(root->left, depth + 1);
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

// https://www.programiz.com/dsa/huffman-coding
t_tree_node *build_huffman_tree(t_heap *h) {
	while (h->size > 1) {
		/* Create an empty node z. Assign the minimum frequency to the left child of z
		and assign the second minimum frequency to the right child of z.
		Set the value of the z as the sum of the above two minimum frequencies.*/
		t_tree_node *left = extract_min(h);
		t_tree_node *right = extract_min(h);
		t_tree_node *new = create_new_node('\0', left->freq + right->freq);
		
		new->left = left;
		new->right = right;
		// Insert node z into the tree and repeat
		insert_heap(h, new);
	}
	return extract_min(h);
}

void print_codes(t_tree_node *n, int depth, int arr[]) {
	if (n->left) {
		arr[depth] = 0;
		print_codes(n->left, depth + 1, arr);
	}
	if (n->right) {
		arr[depth] = 1;
		print_codes(n->right, depth + 1, arr);
	}
	if (n->c != '\0') {
		printf("   %c   |", n->c);
		for (int i = 0; i < depth; i++) {
			printf("%d", arr[i]);
		}
		printf("\n");
	} // leaf node
}

int huffman(char key[], unsigned int value[], unsigned int len) {
	t_heap *heap = create_min_heap(MAX_MIN_HEAP);
	for (unsigned int i = 0; i < len; i++) {
		insert_heap(heap, create_new_node(key[i], value[i]));
	}
	print_heap(heap);
	t_tree_node *tree = build_huffman_tree(heap);
	print_tree(tree, 0);
	printf("\n Printing codes\n");
	int arr[50];
	print_codes(tree, 0, arr);
	free(heap->nodes);
	free(heap);
	return 0;
}