#include "../lib/wood_pecker.h"

// static void print_heap(t_heap *h) {
// 	for (int i = 0; i < h->size; i++) {
// 		printf("%d\n", h->nodes[i]->freq);
// 	}
// }

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
	}
}


void add_codes(t_list *lst,t_tree_node *n, int depth, int arr[]) {
	
	t_list *aux;
	
	if (n->left) {
		arr[depth] = 0;
		add_codes(lst,n->left, depth + 1, arr);
	}
	if (n->right) {
		arr[depth] = 1;
		add_codes(lst,n->right, depth + 1, arr);
	}
	if (n->c != '\0') {

		aux = ft_huffman_find(lst, n->c);
		// printf("   %c   |", n->c);
		for (int i = 0; i < depth; i++) {
			
			((t_huffman_count_list *)aux->content)->code[i] = arr[i] + 48;
			// printf("%d", arr[i]);
		}
		// printf("\n");
	}
}

void engrave_code (t_list * lst,int *txt,int *bitposition, char c)
{
	t_list * aux;
	size_t pos_bit;
	size_t pos;
	size_t i;
	
	i = 0;
	aux = ft_huffman_find(lst,c);
	pos = (size_t) (*bitposition / BITS_INTEGER);
	pos_bit = (BITS_INTEGER - 1) - (size_t)(*bitposition % BITS_INTEGER);
	if (aux)
	{
		while (((t_huffman_count_list *)aux->content)->code[i])
		{
			if (((t_huffman_count_list *)aux->content)->code[i] == '1')
				txt[pos] |= (1 << pos_bit);
			
			if (pos_bit == 0)
			{
				pos_bit = BITS_INTEGER;
				pos++;
			}
			i++;
			pos_bit--;
		}
		*bitposition += i;
	}
}

int * parse_test(char *str,t_list *lst)
{	
	int *txt;
	int bit_pos;
	int bit_count;
	int len;
	size_t text_size;
	
	bit_count = ft_huffman_calculate_bitsize(lst);
	if (bit_count % BITS_INTEGER != 0)
		text_size = bit_count / BITS_INTEGER + 1;
	else
		text_size = bit_count / BITS_INTEGER;

	txt = calloc(text_size,sizeof(int));
	txt[0] = bit_count;
	printf("Tamaños en bits %d \n",bit_count);
	len = ft_strlen(str);
	bit_pos = BITS_INTEGER;
	for (int i = 0 ; i < len ; i++)
		engrave_code(lst,txt,&bit_pos,str[i]);

	return txt;
}


void read_code(int *txt)
{
    size_t pos ,pos_bit;
	size_t bit_count, text_size;
	// size_t bit_test_size = text_size * BITS_INTEGER;

	bit_count = (size_t)txt[0];
	if (bit_count % BITS_INTEGER != 0)
		text_size = bit_count / BITS_INTEGER + 1;
	else
		text_size = bit_count / BITS_INTEGER;
	text_size+=1;

	pos_bit = BITS_INTEGER - 1;
	pos = 1;
    while (pos < text_size)
    {
		if ((txt[pos] & (1 << pos_bit)) == 0)
			printf("0");
		else
			printf("1");
		
		if (pos_bit == 0)
		{
			pos++;
			pos_bit = BITS_INTEGER;
		}
		pos_bit--;
		bit_count--;
		if (bit_count == 0)
			break;
	    }
    printf("\n");
}



void unparse_code(int *txt,t_tree_node *tree )
{
    size_t pos ,pos_bit;
	size_t bit_count ,text_size;
	t_tree_node *aux;

	bit_count = (size_t)txt[0];
	if (bit_count % BITS_INTEGER != 0)
		text_size = bit_count / BITS_INTEGER + 1;
	else
		text_size = bit_count / BITS_INTEGER;
	text_size+=1;

	pos_bit = BITS_INTEGER - 1;
	aux = tree;
	pos = 1;
    while (pos < text_size)
    {
		if ((txt[pos] & (1 << pos_bit)) == 0)
			aux = aux->left;
		else
			aux = aux->right;

		if (aux->c != '\0')
		{
			printf("%c",aux->c);
			aux = tree;
		}
		if (pos_bit == 0)
		{
			pos++;
			pos_bit = BITS_INTEGER;
		}
		pos_bit--;
		bit_count--;
		if (bit_count == 0)
			break;
    }
    printf("\n");
}


int set_truth_table(t_tree_node *tree,char *str, int pos)
{
	if (tree->left) {
		pos = set_truth_table(tree->left,str,pos);
	}
	if (tree->right) {
		pos = set_truth_table(tree->right,str,pos);
	}
	if (tree->c != '\0') {
		str[pos] = tree->c;
		pos++;
	}
	return pos;
}

char * get_tree_in_char(t_tree_node *tree, t_list *lst)
{
	char *aux;
	size_t size;

	size = ft_lstsize(lst);
	aux = calloc(size + 1,sizeof(char));
	set_truth_table(tree,aux,0);

	// for (size_t i = 0;i < size;i++)
	// {
	// 	printf("Tabla de la verdad para %d: '%c'\n",i,aux[i]);
	// }

	return aux;
}


t_heap * get_tree_from_char(char *str)
{
	t_heap *heap = create_min_heap(MAX_MIN_HEAP);
	int		i;

	i = 0;
	while (str[i] != '\0')
	{
		insert_heap(heap, create_new_node(str[i], i));
		i++;
	}

	return heap;
}


int huffman(char *str) {
	t_list *list;
	t_list *aux;
	int *txt;
	char *table;
	printf("Frase a codificar\n%s\n",str);
    list = NULL;
    for (int i = 0; i < ft_strlen(str);i++)
    {
        list =  ft_huffman_add_character(list, str[i]); //ft_count_huffman_character(list,str[i]);
    }
	list =  ft_huffman_add_character(list, ETX);
	t_heap *heap = create_min_heap(MAX_MIN_HEAP);
	
	aux = list;
	while (aux)
	{
		insert_heap(heap, create_new_node(((t_huffman_count_list *)aux->content)->c, ((t_huffman_count_list *)aux->content)->count));
		aux = aux->next;
	}

	t_tree_node *tree = build_huffman_tree(heap);
	// print_heap(heap);
	print_tree(tree, 0);
	int arr[50];
	add_codes(list,tree, 0, arr);

	// ft_huffman_visualize(list);

	// txt = parse_test(str,list);
	// printf("Frase codificada , atrevete a leer esto\n");
	// read_code(txt);
	// printf("Ahora toca descodificar\n");
	// unparse_code(txt,tree);

	printf("- - - - - - - - - - - \n");

	table = get_tree_in_char(tree,list);

	t_heap *heap_decode = get_tree_from_char(table);
	t_tree_node *tree_decode = build_huffman_tree(heap_decode);
	print_tree(tree_decode, 0);


	free(txt);
	free(heap->nodes);
	free(heap);
	return 0;
}