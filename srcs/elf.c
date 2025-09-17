#include "../lib/wood_pecker.h"


char get_type_sym_special_sections_64(Elf64_Sym *sym, elf64_manager * org) {
    char const * name ;
    char type;
    char *sections [13] = {".bss",".sbss",".dynamic",".got",".data",".data1",".init_array",".preinit_array",".fini_array",".sdata",".text" ,".rodata",".rodata1"};
    char char_type[13] = {'b','s','d','d','d','d','d','d','d','g','t','r','r'};
    type = '?';
    if ( sym->st_shndx == SHN_ABS)
        return (type);
    name = (const char *)&org->sh_strtab[org->shdr[sym->st_shndx].sh_name];
    for (int i = 0; i < 13; i++)
    {
        if(strcmp(sections[i],name) == 0)
        {
            type = (char_type[i]);
            break;
        }
    }
    if (ELF64_ST_BIND(sym->st_info) == STB_GLOBAL)
        type = toupper(type);

    return (type);

}

char get_type_sym64(Elf64_Sym *sym, elf64_manager * org) {
    int shdr_tipo;
    int shdr_flag;
    int sym_bind;
    int sym_type;
    char r;
    
    sym_bind = ELF64_ST_BIND(sym->st_info);
    sym_type = ELF64_ST_TYPE(sym->st_info);

	// A/a The symbol's value is absolute
    if ( sym->st_shndx == SHN_ABS)
    {
        if (sym_bind == STB_GLOBAL)
            return 'A';
        return 'a';
    }

	shdr_tipo = org->shdr[sym->st_shndx].sh_type;
    shdr_flag = org->shdr[sym->st_shndx].sh_flags;

    // V/v The symbol is a weak object 
    // W/w The symbol is a weak symbol
    if (sym_bind == STB_WEAK)
    {  
        if (sym_type == STT_OBJECT)
        {
            if (shdr_tipo == SHT_NULL)
                return 'v';
            return 'V';
        }  
        if (shdr_tipo == SHT_NULL)
            return 'w';
        return 'W';
    }

    //If symbol has special tag , we can identify for it and categorize it
    r = get_type_sym_special_sections_64(sym,org);
    if (r != '?')
        return (r);
    
    // //i STT_GNU_IFUNC  ///* Symbol is indirect code object */
    if ( sym_type == STT_GNU_IFUNC) 
        return 'I';
    
    // // u The symbol is a unique global symbol.
    if (sym_bind == STB_GNU_UNIQUE)
        return 'u';
    
    // U The symbol is undefined.
    if ( sym->st_shndx == SHN_UNDEF)
        return 'U';

    // // 'B/b' The symbol is in the BSS data section
    if (shdr_tipo == SHT_NOBITS && (shdr_flag == (SHF_WRITE | SHF_ALLOC)))
        r = 'b';

    // // 'C' SHN_COMMON - Symbols defined relative to this section are common symbols
    if (shdr_tipo == SHN_COMMON)
        r = 'c';

    //D/d The symbol is in the initialized data section.
    if ((shdr_tipo == SHT_DYNAMIC && shdr_flag == (SHF_ALLOC | SHF_WRITE)))
        r = 'd';

    // // D/d    The symbol is in the initialized data section .data && .data1  
    if ((shdr_tipo == SHT_PROGBITS && shdr_flag == (SHF_ALLOC | SHF_WRITE)))
        r = 'd';

    // // R   - The symbol is in a read only data section.
    if ( ((shdr_tipo == SHT_PROGBITS) && (shdr_flag & ~SHF_ALLOC) == 0) || shdr_tipo == SHT_NOTE )
        r = 'r';

    // // -T/t The symbol is in the text (code) section.
    if ((shdr_tipo == SHT_PROGBITS && (shdr_flag & (SHF_EXECINSTR | SHF_ALLOC)) == (SHF_EXECINSTR | SHF_ALLOC) ))
        r = 't';
    
    if (sym_bind == STB_GLOBAL && r != '?')
        return (toupper(r));

    return (r);
}

void	show_Sym64(Elf64_Sym *sym, elf64_manager *org)
{
    (void) sym;
    (void) org;
	// char			type;
    // const char *sym_name;
	// unsigned char	info;
    // unsigned char	bind;

	// info = ELF64_ST_TYPE(sym->st_info);
    // bind = ELF64_ST_BIND(sym->st_info);
	// type = get_type_sym64(sym, org);
	// if (info == STT_SECTION)
	// 	sym_name = (const char *)&org->sh_strtab[org->shdr[sym->st_shndx].sh_name];
	// else
    //     sym_name = (const char *)&org->sym_strtab[sym->st_name];
    if (DEBUG)
    {
        debug_sym64(sym, org);
        return ;
    }
	return;
    

}

static void	 extract_Sym64(elf64_manager * org,void * _map)
{
	Elf64_Sym	*symbols_temp;
	const char	*strtab_aux;
	int			c_aux;

	strtab_aux = NULL;
	org->sym_strtab = NULL;
    org->sh_strtab = NULL;
	//Miramos en cada seccion 
	for (int i = 0; i < org->elf_header->e_shnum; i++) {
		//Buscamos el shoulder que tenga la tabla de simbolos
		if (org->shdr[i].sh_type == SHT_SYMTAB) {  // debug_shdr64(&org->shdr[i]);
			symbols_temp = (Elf64_Sym *)(_map + org->shdr[i].sh_offset);
			org->num_symbols = org->shdr[i].sh_size / sizeof(Elf64_Sym);
			org->symbols = calloc (org->num_symbols + 1 , sizeof(Elf64_Sym));
			c_aux = 0;
            //Extraemos cada simbolo y quitamos la cabecera vacia (1er sym) de nuestros simbolos
			for (int i = 0; i < org->num_symbols; i++) {
				if ( symbols_temp[i].st_name == 0 && symbols_temp[i].st_info == 0 && symbols_temp[i].st_other == 0 && \
					symbols_temp[i].st_shndx == 0 && symbols_temp[i].st_size == 0 && symbols_temp[i].st_value == 0)
				{
					c_aux++;
					org->num_symbols--;
				}
				org->symbols[i] = symbols_temp[c_aux++];                
			}
        }
        /*Buscamos la seccion con la listas de nombres (puede haber varias 
        , primero buscamos la especifica de los simbolos sino asumimos que la destinada a la 
        seccion tiene tambien los simbolos) */
        if (org->shdr[i].sh_type == SHT_STRTAB ) {
            strtab_aux = (const char *)(_map + org->shdr[i].sh_offset);
            if ( i != org->elf_header->e_shstrndx)
                org->sym_strtab = strtab_aux;
            else
                org->sh_strtab = strtab_aux;

        }
    }

	if (org->sym_strtab == NULL)
		org->sym_strtab = strtab_aux;

	return;
}


static	int	process_sym64(elf64_manager *org)
{
	if (org->symbols && org->sym_strtab) {
		for (int i = 0; i < org->num_symbols; i++) {
			show_Sym64(&org->symbols[i], org); //Mostramos los simbolos
		}
	}
	return (0);
}

int	analisis_ELF64(void * map)
{
	elf64_manager org;
    int status = 0;

    (void) status;
	org.elf_header =  (Elf64_Ehdr *)map;                               //Sacamos el header del archivo elf
	if (org.elf_header->e_type == ET_NONE)
        return (NM_FILE_NONE);
	if (org.elf_header->e_type == ET_REL)
        status = NM_FILE_REL;
	if (org.elf_header->e_type == ET_DYN)
        return (NM_FILE_DYN);
	if (org.elf_header->e_type == ET_CORE)
        return (NM_FILE_CORE_DUMP);
    if (org.elf_header->e_type == ET_EXEC)
            status = NM_FILE_EXEC;
	printf("File type %d\n",org.elf_header->e_type);
	
    org.shdr = (Elf64_Shdr *)((char *)map + org.elf_header->e_shoff);  //Sacamos la lista de los shoulders
	extract_Sym64(&org,map);       //Extraigo los simbolos
    printf("Extraemos las cosas\n");
	process_sym64(&org);     //Se procesan para mostrarse
    printf("Simbolo porcesados\n");
	free(org.symbols);              //Los liberamos de la memoria
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

