/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wood_pecker.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 12:51:39 by mortiz-d          #+#    #+#             */
/*   Updated: 2025/09/24 13:44:38 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <elf.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "huffman.h"
// #include <stdlib.h>
// #include <strings.h>
#include "../libft/lib/libft.h"

#ifndef DEBUG
# define DEBUG 1
#endif

#define EXEC_NAME "woody_woodpacker"

#define NM_FILE_NONE        (printf("%s: No file type\n", EXEC_NAME) ,0)
#define NM_FILE_REL         (printf("%s: Relocatable file\n", EXEC_NAME) ,0)
#define NM_FILE_EXEC        (printf("%s: Executable file\n", EXEC_NAME) ,0)
#define NM_FILE_DYN         (printf("%s: Shared object file\n", EXEC_NAME) ,0)
#define NM_FILE_CORE_DUMP   (printf("%s: Core file\n", EXEC_NAME) ,0)

#define NM_FILE_NOT_FOUND(filename) (printf("%s: '%s': file does not exist\n",EXEC_NAME, filename), 0)
#define NM_IS_DIR(filename)         (printf("%s: warning: '%s' is a directory\n",EXEC_NAME, filename), 0)
#define NM_CANT_MMAP(filename)      (printf("%s: warning: '%s' can't mmap\n",EXEC_NAME, filename), 0)
#define NM_CANT_MUNMAP(filename)    (printf("%s: warning: '%s' can't unmmap\n",EXEC_NAME, filename), 0)

//MANAGERS 32/64 Bits
typedef struct elf64_manager{                   
	int num_symbols;
	Elf64_Sym *symbols;
	Elf64_Ehdr *elf_header;
	Elf64_Shdr *shdr;
	const char *sym_strtab;
	const char *sh_strtab;
} elf64_manager;


int	ft_nm(char *filename, int fd);


// //FLAG STRUCT
// typedef struct active_flags{                   
//     int a;
//     int g;
//     int u;
//     int r;
//     int p;
// } active_flags;


//BASE FUNCTIONS
int	ft_nm			(char *filename, int fd);
int	analisis_ELF64	(void * _map);
int	analisis_ELF32	(void * _map);
char get_type_sym64(Elf64_Sym *sym, elf64_manager * org);
char get_type_sym_special_sections_64(Elf64_Sym *sym, elf64_manager * org);

// //SORTING
// void	bubble_sort_sym64(elf64_manager * org, active_flags flags);
// void	bubble_sort_sym32(elf32_manager * org, active_flags flags);

// //DEBUG
// int	debug_type_file	(Elf64_Half type );
int	debug_sym64		(Elf64_Sym *sym ,elf64_manager * org);
// int	debug_shdr64	(Elf64_Shdr *shdr,elf64_manager * org);
// int	debug_sym32		(Elf32_Sym *sym ,elf32_manager * org);
// int	debug_shdr32	(Elf32_Shdr *shdr,elf32_manager * org);
int encode(int fd);