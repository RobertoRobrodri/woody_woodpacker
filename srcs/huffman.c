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
		for (int i = 0; i < depth; i++) {
			
			((t_huffman_count_list *)aux->content)->code[i] = arr[i] + 48;
		}
	}
}

void engrave_code (t_list * lst,int *txt,int *bitposition, char c){
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



void engrave_number (int *txt,int *bitposition, int freq){
	int pos_bit ,pos;
	int table_pos_bit;
	
	pos = (*bitposition / BITS_INTEGER);
	pos_bit = (BITS_INTEGER - 1) - (*bitposition % BITS_INTEGER);
	table_pos_bit = BITS_INTEGER - 1;
	while (table_pos_bit >= 0)
	{
		if ((freq & (1 << table_pos_bit)) != 0)
			txt[pos] |= (1 << pos_bit);
		if (pos_bit == 0)
		{
			pos_bit = BITS_INTEGER;
			pos++;
		}
		pos_bit--;
		table_pos_bit--;
		*bitposition+=1;
	}
}


void engrave_character (int *txt,int *bitposition, char c){
	int pos_bit ,pos;
	int table_pos_bit;
	
	pos = (*bitposition / BITS_INTEGER);
	pos_bit = (BITS_INTEGER - 1) - (*bitposition % BITS_INTEGER);
	table_pos_bit = BITS_CHAR - 1;
	while (table_pos_bit >= 0)
	{
		if ((c & (1 << table_pos_bit)) != 0)
			txt[pos] |= (1 << pos_bit);
		if (pos_bit == 0)
		{
			pos_bit = BITS_INTEGER;
			pos++;
		}
		pos_bit--;
		table_pos_bit--;
		*bitposition+=1;
	}
}


int read_number (int *txt,int *bitposition){
	int number;
	int pos_bit ,pos;
	int table_pos_bit;
	
	number = 0;
	pos = (*bitposition / BITS_INTEGER);
	pos_bit = (BITS_INTEGER - 1) - (*bitposition % BITS_INTEGER);
	table_pos_bit = BITS_INTEGER - 1;
	while (table_pos_bit >= 0)
	{
		if ((txt[pos] & (1 << pos_bit)) != 0)
			number |= (1 << table_pos_bit);
		if (pos_bit == 0)
		{
			pos_bit = BITS_INTEGER;
			pos++;
		}
		pos_bit--;
		table_pos_bit--;
		*bitposition+=1;
	}
	return number;
}

char read_character (int *txt,int *bitposition){
	char c;
	int pos_bit ,pos;
	int table_pos_bit;
	
	pos = (*bitposition / BITS_INTEGER);
	pos_bit = (BITS_INTEGER - 1) - (*bitposition % BITS_INTEGER);
	table_pos_bit = BITS_CHAR - 1;
	c = 0;
	while (table_pos_bit >= 0)
	{
		if ((txt[pos] & (1 << pos_bit)) != 0)
			c |= (1 << table_pos_bit);

		if (pos_bit == 0)
		{
			pos_bit = BITS_INTEGER;
			pos++;
		}
		pos_bit--;
		table_pos_bit--;
		*bitposition+=1;
	}
	return (c);
}

char read_code_2(int *txt, int *bitposition, t_tree_node *tree ){
    char c;
	int pos_bit ,pos;
	t_tree_node *aux;
	
	pos = (*bitposition / BITS_INTEGER);
	pos_bit = (BITS_INTEGER - 1) - (*bitposition % BITS_INTEGER);
	c = 0;
	aux = tree;
    while (aux->c == '\0')
    {
		if ((txt[pos] & (1 << pos_bit)) == 0)
		{
			aux = aux->left;
		}
		else
		{
			aux = aux->right;
		}

		if (pos_bit == 0)
		{
			pos++;
			pos_bit = BITS_INTEGER;
		}
		pos_bit--;
		*bitposition += 1;
    }
	c = aux->c;
	return c;
}

char * unhuffman(int *txt){
    int pos_bit, table_size;
	t_heap *heap;
	t_tree_node *tree;
	char *str;
	int test_i;
	char test_c;

	if ((void *)txt == NULL)
		return "";

	pos_bit = 0;
	table_size = read_number(txt,&pos_bit);

	heap = create_min_heap(MAX_MIN_HEAP);
	for (int i = 0; i < table_size; i++)
	{
		test_c = read_character(txt,&pos_bit);
		test_i = read_number(txt,&pos_bit);
		insert_heap(heap, create_new_node(test_c, test_i));
	}
	tree = build_huffman_tree(heap);

	table_size = read_number(txt,&pos_bit);
	str = calloc (table_size + 1, sizeof(char));
	for (int i = 0; i < table_size; i++)
	{
		test_c = read_code_2(txt,&pos_bit,tree);
		str[i] = test_c;
	}
	free(heap->nodes);
	free(heap);
	return (str);
}


int *huffman(int *txt_size, char *str){	
	int *txt, bit_pos, bit_count, len;
	int arr[50];
	t_list *aux, *lst;
	t_heap *heap;
	t_tree_node *tree;

	//We establish the truth table for the code
    lst = NULL;
	len = ft_strlen(str);

	if (len == 0)
		return NULL;

    for (int i = 0; i < ft_strlen(str);i++)
	{
        lst =  ft_huffman_add_character(lst, str[i]); 
	}
	heap = create_min_heap(MAX_MIN_HEAP);
	aux = lst;
	while (aux)
	{
		insert_heap(heap, create_new_node(((t_huffman_count_list *)aux->content)->c, ((t_huffman_count_list *)aux->content)->count));
		aux = aux->next;
	}
	tree = build_huffman_tree(heap);
	// print_heap(heap);
	// print_tree(tree, 0);	
	add_codes(lst,tree, 0, arr);

	// Tamaño de la reserva es : Tamaño de la tabla + tabla de la verdad + Tamaño del mensaje + Mensaje encriptado
	//	                             (1 int)         (1 char + 1 int * x)    (1 int)             (X size in bits)
	bit_count = ft_huffman_calculate_bitsize(lst) + (BITS_INTEGER * 2) + ((int)ft_huffman_list_count(lst) * ((sizeof(char) + sizeof(int)) * BITS_IN_BYTE )) ;
	if (bit_count % BITS_INTEGER != 0)
		*txt_size = bit_count / BITS_INTEGER + 1;
	else
		*txt_size = bit_count / BITS_INTEGER;

	txt = calloc(*txt_size,sizeof(int));
	
	
	//We start engraving the code into its binary form
	bit_pos = 0;
	engrave_number(txt,&bit_pos,(int)ft_huffman_list_count(lst)); //first we engrave the size of the table (char + int)
	aux = lst;
	while (aux)
	{
		engrave_character(txt,&bit_pos,((t_huffman_count_list *)aux->content)->c);
		engrave_number(txt,&bit_pos,((t_huffman_count_list *)aux->content)->count);
		aux = aux->next;
	}
	engrave_number(txt,&bit_pos,(int)ft_strlen(str)); //then we engrave the size of code len (normal size)
	for (int i = 0 ; i < len ; i++)
		engrave_code(lst,txt,&bit_pos,str[i]);

	fd_huffman_destroy_list(lst);
	free(heap->nodes);
	free(heap);
	printf("\ndevolvemos %d\n",*txt_size);
	printf("\ndevolvemos %p\n",txt);
	return txt;
}


// int huffman(char *str) {
// 	// t_list *list;
// 	// t_list *aux;
// 	int *txt;
	// // char *table;
	// printf("Frase a codificar\n%s\n",str);
    // list = NULL;
    // for (int i = 0; i < ft_strlen(str);i++)
    // {
    //     list =  ft_huffman_add_character(list, str[i]); //ft_count_huffman_character(list,str[i]);
    // }
	// // list =  ft_huffman_add_character(list, ETX);
	// t_heap *heap = create_min_heap(MAX_MIN_HEAP);
	
	// aux = list;
	// while (aux)
	// {
	// 	insert_heap(heap, create_new_node(((t_huffman_count_list *)aux->content)->c, ((t_huffman_count_list *)aux->content)->count));
	// 	aux = aux->next;
	// }

	// t_tree_node *tree = build_huffman_tree(heap);
	// // print_heap(heap);
	// // print_tree(tree, 0);
	// int arr[50];
	// add_codes(list,tree, 0, arr);

	// ft_huffman_visualize(list);

// 	txt = parse_test(str);
// 	printf("- - - - - - - - - - - \n");
// 	// unparse_test(txt);

// 	printf("\n- - - - - - - - - - - \n");

	

// 	free(txt);
	
// 	return 0;
// }