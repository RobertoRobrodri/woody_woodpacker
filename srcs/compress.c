#include "../lib/wood_pecker.h"

//Codificacion de Huffman

int compress_data (char * str )
{
    t_list *list;
    // t_binary_tree *tree;
    // char * compress_data;
    // compress_data = calloc(strlen(str),sizeof(char));
    list = NULL;
    // tree = NULL;
    // printf("WOLOLO\n");
    for (int i = 0; i < ft_strlen(str);i++)
    {
        // tree = ft_binarytree_insert(tree,(int)str[i] );
        list = ft_count_huffman_character(list,str[i]);
    }
    // printf("WOLOLO end :v\n");
    ft_visualize_huffman_list(list);

	// printf("Visualizamos el arbol!  %p\n",tree);
    // ft_binarytree_visualize(tree);
    
   
    return 1;
}