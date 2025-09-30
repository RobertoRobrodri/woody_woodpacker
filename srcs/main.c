#include "../lib/wood_pecker.h"

int	main(int argc, char **argv)
{
	int * txt;
	int txt_size;
	(void) argc; 
	(void) argv; 
	// int		fd;
	
	// if (argc != 2)
	// {
	// 	printf("./%s <file_name>\n",EXEC_NAME);
	// 	return 0;
	// }
	txt = NULL;
	txt_size = 0;
	// printf("%s\n", argv[1]);
	// fd = open(argv[1], O_RDONLY);
	// // ft_nm(argv[1],fd);//, flags, multiple_commands);
	// close(fd);
	// char keys[] = {'a', 'b', 'c'};
	// unsigned int values[] = {1, 2, 3};
	// huffman(keys, values, sizeof(keys));
	// huffman("Pellentesque tincidunt ex sed erat rhoncus, interdum venenatis mauris vestibulum. Vestibulum a erat odio. Quisque scelerisque tempor arcu quis venenatis. Aliquam turpis eros, rutrum sed nibh eu, suscipit hendrerit neque. Mauris iaculis ligula risus, quis posuere dolor faucibus sed. Fusce dictum porta tortor, egestas lacinia dolor ultricies vestibulum. Phasellus eu massa leo. Phasellus rutrum ullamcorper purus placerat fermentum. Vestibulum ultrices lacus neque, id semper nibh viverra eget. Sed luctus ac erat non hendrerit. Phasellus pretium quam vitae sapien euismod, sit amet porta eros auctor. Duis mollis sodales est, sit amet sollicitudin enim posuere at. ");
    // txt =  huffman("MANY OF YOU ARE GONNtxtA DIE BUT THATS A SACRIFICE IM WILLING TO MAKE");
	// txt =  huffman("hola");
	txt = huffman(&txt_size,"Never gonna give you up, never gonna let you down Never gonna run around and desert you Never gonna make you cry, never gonna say goodbye Never gonna tell a lie and hurt you");
	printf ( "________________\n");
	printf ( "devuelto %p\n",txt);
	for (int i = 0; i < txt_size; i++)
	{
		printf ( "%d\n", txt[i]);
	}
	printf ( "%s\n", unhuffman(txt));
	free(txt);
	return 0 ;
}