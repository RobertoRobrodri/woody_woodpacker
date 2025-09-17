#include "../lib/wood_pecker.h"


int	main(int argc, char **argv)
{
	int		fd;
	
	if (argc != 2)
	{
		printf("./%s <file_name>\n",EXEC_NAME);
		return 0;
	}


	fd = open(argv[1], O_RDONLY);
	ft_nm(argv[1],fd);//, flags, multiple_commands);
	if (fd != -1)
		close(fd);


    return 0 ;
}