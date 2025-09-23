#include "../lib/wood_pecker.h"


int	main(int argc, char **argv)
{
	// int	 a	fd;
	(void) argv;
	(void) argc;
	// t_binary_tree *tree;


	if (argc != 2)
	{
		printf("./%s <file_name>\n",EXEC_NAME);
		return 0;
	}

	compress_data ("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vestibulum vel turpis nibh. Vivamus cursus vestibulum imperdiet. Vestibulum aliquet ac neque in ullamcorper. Curabitur vulputate leo eros, sed ultrices tellus laoreet nec. Vestibulum aliquam orci nec ipsum suscipit, vel vestibulum lacus varius. Fusce finibus hendrerit mauris, nec consectetur quam tempus nec. Aenean cursus euismod risus non rutrum.");

	// tree = NULL;
	// printf("Hacemos el arbol!\n");
	// tree = ft_binarytree_create(1);
    // ft_binarytree_insert(tree,8);
    // ft_binarytree_insert(tree,4);
    // ft_binarytree_insert(tree,2);
    // ft_binarytree_insert(tree,3);
	// printf("Visualizamos el arbol!  %p\n",tree);
    // ft_binarytree_visualize(tree);


    return 0 ;
}
