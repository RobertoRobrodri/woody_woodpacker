#include "../lib/wood_pecker.h"

int	analisis_ELF64(void * map)
{
	elf64_manager org;

	org.elf_header =  (Elf64_Ehdr *)map;                               //Sacamos el header del archivo elf
	if (org.elf_header->e_type == ET_NONE)
        return (NM_FILE_NONE);
	if (org.elf_header->e_type == ET_REL)
        return (NM_FILE_REL);
	if (org.elf_header->e_type == ET_EXEC)
        return (NM_FILE_EXEC);
	if (org.elf_header->e_type == ET_DYN)
        return (NM_FILE_DYN);
	if (org.elf_header->e_type == ET_CORE)
        return (NM_FILE_CORE_DUMP);

	printf("File type %d\n",org.elf_header->e_type);
	
    // org.shdr = (Elf64_Shdr *)((char *)_map + org.elf_header->e_shoff);  //Sacamos la lista de los shoulders
	// extract_Sym64(&org,_map);       //Extraigo los simbolos
	// process_sym64(&org, flags);     //Se procesan para mostrarse
	// free(org.symbols);              //Los liberamos de la memoria
    return (0);
}

static int	header_checker(void *_map)
{
	unsigned char	*e_ident;

	e_ident = (unsigned char *)_map;
	if (e_ident[EI_MAG0] != ELFMAG0 || \
		e_ident[EI_MAG1] != ELFMAG1 || \
		e_ident[EI_MAG2] != ELFMAG2 || \
		e_ident[EI_MAG3] != ELFMAG3)
	{
		printf("El archivo no es un ELF válidgo\n");
		return (-1);
	}
	return (e_ident[EI_CLASS]);
}

static	int class_analisis(void *map, int ei_class)//, active_flags flags)
{
	(void)map;

	if (ei_class == ELFCLASSNONE)
		printf("EL ELF NO TIENE VERSION\n");
	else if (ei_class == ELFCLASS32)
		printf("FILE TYPE CLASS 32\n");
		// analisis_ELF32(_map,flags);
	else if (ei_class == ELFCLASS64)
	{

		printf("FILE TYPE CLASS 64\n");
		analisis_ELF64(map);
	}
	else
		return (1);
	return (0);
}

int	ft_nm(char *filename, int fd)
{
	struct stat	file_data; 		//Estructura util para almacenar los datos del archivo que se lee
	void		*map;            	//El mapeo de los datos para poder extraer que necesitamos en una direccion
	int			ei_class;			//indent del archivo

	(void)ei_class;

	if (fstat(fd, &file_data) == -1 || fd == -1 ) 
	    return NM_FILE_NOT_FOUND(filename);
	if (S_ISDIR(file_data.st_mode))
	    return NM_IS_DIR(filename);

	map = mmap(NULL,file_data.st_size,PROT_READ,MAP_PRIVATE,fd,0);
	if (map == MAP_FAILED)
	    return NM_CANT_MMAP(filename);
	ei_class = header_checker(map);
	class_analisis(map, ei_class);
	if (munmap(map, file_data.st_size) == -1)
	    return NM_CANT_MUNMAP(filename);
	return (1); // Retorno exitoso
}


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