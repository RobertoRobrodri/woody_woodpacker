#include "../lib/wood_pecker.h"

int	main(int argc, char **argv)
{
	// int		fd;
	
	// if (argc != 2)
	// {
	// 	printf("./%s <file_name>\n",EXEC_NAME);
	// 	return 0;
	// }


	// printf("%s\n", argv[1]);
	// fd = open(argv[1], O_RDONLY);
	// // ft_nm(argv[1],fd);//, flags, multiple_commands);
	// close(fd);
	char keys[] = {'a', 'b', 'c'};
	unsigned int values[] = {1, 2, 3};
	huffman(keys, values, sizeof(keys));
    return 0 ;
}