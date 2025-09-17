#include "../lib/wood_pecker.h"

int debug_type_file (Elf64_Half type )
{
    // ET_NONE (Undefined): ELF Format unknown or not specified.
    // ET_EXEC: (Executable file): An ELF executable.
    // ET_DYN: (Shared object): A library or a dynamically-linked executable.
    // ET_REL (Relocatable file): Relocatable files (.o object files).
    // ET_CORE (Core dump): A core dump file.
    if (DEBUG)
    {
        if ( type == ET_NONE)
			printf("(Undefined): ELF Format unknown or not specified.\n");
        else if (type == ET_EXEC)
			printf("(Executable file): An ELF executable.\n");
        else if (type == ET_DYN)
			printf("(Shared object): A library or a dynamically-linked executable.\n");
        else if (type == ET_REL)
			printf("(Relocatable file): Relocatable files (.o object files).\n");
        else if (type == ET_CORE)
			printf("(Core dump): A core dump file.\n");
    }
    return 1;
}

int debug_sym64(Elf64_Sym *sym, elf64_manager *org) {
    Elf64_Shdr *shdr;
    const char *sh_type_str;
    uint8_t st_type;
    uint8_t st_bind;
    const char *type_str;
    const char *bind_str;

    
    if (DEBUG) {
        printf("Symbol\n");

        if (ELF64_ST_TYPE(sym->st_info) == STT_SECTION)
            printf("name->%s\n", (const char *)&org->sh_strtab[org->shdr[sym->st_shndx].sh_name]);
        else
            printf("name->%s\n", (const char *)&org->sym_strtab[sym->st_name]);
        
        if (sym->st_shndx != SHN_ABS)
            printf("section name->%s\n", (const char *)&org->sh_strtab[org->shdr[sym->st_shndx].sh_name]);
        else
            printf("section name- NONE\n");
        printf("sh_name->%d\n", sym->st_name);
        printf("Type determined         [%c]\n", get_type_sym64(sym ,org));
        printf("Type by special section [%c]\n", get_type_sym_special_sections_64(sym ,org));
        st_type = ELF64_ST_TYPE(sym->st_info);
        st_bind = ELF64_ST_BIND(sym->st_info);
        
        type_str = NULL;
        switch (st_type) {
            case STT_NOTYPE: type_str = "STT_NOTYPE (0)"; break;
            case STT_OBJECT: type_str = "STT_OBJECT (1)"; break;
            case STT_FUNC: type_str = "STT_FUNC (2)"; break;
            case STT_SECTION: type_str = "STT_SECTION (3)"; break;
            case STT_FILE: type_str = "STT_FILE (4)"; break;
            case STT_COMMON: type_str = "STT_COMMON (5)"; break;
            default: type_str = "UNKNOWN"; break;
        }
        printf("sh_type: %s\n", type_str);
        
        bind_str = NULL;
        switch (st_bind) {
            case STB_LOCAL: bind_str = "STB_LOCAL (0)"; break;
            case STB_GLOBAL: bind_str = "STB_GLOBAL (1)"; break;
            case STB_WEAK: bind_str = "STB_WEAK (2)"; break;
            default: bind_str = "UNKNOWN"; break;
        }
        printf("sh_bind: %s\n", bind_str);
        printf("st_shndx: %d\n", sym->st_shndx);
        
        if (sym->st_shndx == SHN_ABS)
        {
            printf("Section type: %s\n", "ABSOLUTE");
            printf("Section flags: %s\n", "NONE");
            // Información adicional de la sección
            printf("st_size: %ld\n", sym->st_size);
            printf("st_value: %ld\n", sym->st_value);
            printf("-----------------------------------------------------------------------------------\n");

            return (0);
        }

        // Verificar la sección asociada al símbolo
        shdr = &org->shdr[sym->st_shndx];
        sh_type_str = NULL;
        
        switch (shdr->sh_type) {
            case SHT_NULL: sh_type_str = "SHT_NULL"; break;
            case SHT_PROGBITS: sh_type_str = "SHT_PROGBITS"; break;
            case SHT_SYMTAB: sh_type_str = "SHT_SYMTAB"; break;
            case SHT_STRTAB: sh_type_str = "SHT_STRTAB"; break;
            case SHT_RELA: sh_type_str = "SHT_RELA"; break;
            case SHT_HASH: sh_type_str = "SHT_HASH"; break;
            case SHT_DYNAMIC: sh_type_str = "SHT_DYNAMIC"; break;
            case SHT_NOTE: sh_type_str = "SHT_NOTE"; break;
            case SHT_NOBITS: sh_type_str = "SHT_NOBITS"; break;
            case SHT_REL: sh_type_str = "SHT_REL"; break;
            case SHT_SHLIB: sh_type_str = "SHT_SHLIB"; break;
            case SHT_DYNSYM: sh_type_str = "SHT_DYNSYM"; break;
            case SHT_SUNW_move: sh_type_str = "SHT_SUNW_move"; break;
            case SHT_SUNW_COMDAT: sh_type_str = "SHT_SUNW_COMDAT"; break;
            case SHT_SUNW_syminfo: sh_type_str = "SHT_SUNW_syminfo"; break;
            case SHT_GNU_verdef: sh_type_str = "SHT_GNU_verdef"; break;
            case SHT_GNU_verneed: sh_type_str = "SHT_GNU_verneed"; break;
            case SHT_GNU_versym: sh_type_str = "SHT_GNU_versym"; break;
            case SHT_LOPROC: sh_type_str = "SHT_LOPROC"; break;
            case SHT_HIPROC: sh_type_str = "SHT_HIPROC"; break;
            case SHT_LOUSER: sh_type_str = "SHT_LOUSER"; break;
            case SHT_HIUSER: sh_type_str = "SHT_HIUSER"; break;
            case SHT_INIT_ARRAY: sh_type_str = "SHT_INIT_ARRAY"; break;
            case SHT_PREINIT_ARRAY: sh_type_str = "SHT_PREINIT_ARRAY"; break;
            case SHT_FINI_ARRAY: sh_type_str = "SHT_FINI_ARRAY"; break;
            default: sh_type_str = "UNKNOWN"; break;
        }
        
        printf("Section type: %s\n", sh_type_str);
        
        // Mostrar flags de la sección
        printf("Section flags: ");
        if (shdr->sh_flags & SHF_WRITE) printf("SHF_WRITE ");
        if (shdr->sh_flags & SHF_ALLOC) printf("SHF_ALLOC ");
        if (shdr->sh_flags & SHF_EXECINSTR) printf("SHF_EXECINSTR ");
        if (shdr->sh_flags & SHF_MERGE) printf("SHF_MERGE ");
        if (shdr->sh_flags & SHF_STRINGS) printf("SHF_STRINGS ");
        if (shdr->sh_flags & SHF_INFO_LINK) printf("SHF_INFO_LINK ");
        if (shdr->sh_flags & SHF_LINK_ORDER) printf("SHF_LINK_ORDER ");
        if (shdr->sh_flags & SHF_OS_NONCONFORMING) printf("SHF_OS_NONCONFORMING ");
        if (shdr->sh_flags & SHF_GROUP) printf("SHF_GROUP ");
        if (shdr->sh_flags & SHF_MASKOS) printf("SHF_MASKOS ");
        if (shdr->sh_flags & SHF_ORDERED) printf("SHF_ORDERED ");
        if (shdr->sh_flags & SHF_EXCLUDE) printf("SHF_EXCLUDE ");
        if (shdr->sh_flags & SHF_MASKPROC) printf("SHF_MASKPROC ");
        printf("\n");

        // Información adicional de la sección
        printf("st_size: %ld\n", sym->st_size);
        printf("st_value: %ld\n", sym->st_value);

        // Línea de separación
        printf("-----------------------------------------------------------------------------------\n");
    }
    
    return 1;
}

int debug_shdr64(Elf64_Shdr *shdr,elf64_manager * org) {
    if (DEBUG) {
        printf("name->%s\n",(const char *)&org->sh_strtab[shdr->sh_name]);
        printf("sh_name: %u\n", shdr->sh_name);
        printf("Section Header:\n");
        printf("sh_type: %u\n", shdr->sh_type);
        printf("sh_flags: %lx\n", shdr->sh_flags);
        printf("sh_addr: %lx\n", shdr->sh_addr);
        printf("sh_offset: %lx\n", shdr->sh_offset);
        printf("sh_size: %lx\n", shdr->sh_size);
        printf("sh_link: %u\n", shdr->sh_link);
        printf("sh_addralign: %lx\n", shdr->sh_addralign);
        printf("sh_info: %u\n", shdr->sh_info);
        printf("sh_entsize: %lx\n", shdr->sh_entsize);
        printf("-----------\n");
    }
    return (1);
}

// int debug_sym32 ( Elf32_Sym *sym ,elf32_manager * org)
// {
//     if (DEBUG)
//     {
// 		printf("Symbol\n");
//         printf("name->%s\n",(const char *)&org->sym_strtab[sym->st_name]);
//         printf("sh_name->%d\n",sym->st_name);
// 		printf("sh_type:%d\n",ELF64_ST_TYPE(sym->st_info));
// 		printf("sh_bind:%d\n",ELF64_ST_BIND(sym->st_info));
//         printf("st_shndx:%d\n",sym->st_shndx);
// 		printf("st_size:%d\n",sym->st_size);
//         printf("st_value:%d\n",sym->st_value);
// 		printf("-----------\n");
//     }
//     return (1);
// }

// int debug_shdr32(Elf32_Shdr *shdr,elf32_manager * org) {
//     if (DEBUG) {
//         printf("name->%s\n",(const char *)&org->sh_strtab[shdr->sh_name]);
//         printf("sh_name: %u\n", shdr->sh_name);
//         printf("Section Header:\n");
//         printf("sh_type: %u\n", shdr->sh_type);
//         printf("sh_flags: %x\n", shdr->sh_flags);
//         printf("sh_addr: %x\n", shdr->sh_addr);
//         printf("sh_offset: %x\n", shdr->sh_offset);
//         printf("sh_size: %x\n", shdr->sh_size);
//         printf("sh_link: %u\n", shdr->sh_link);
//         printf("sh_addralign: %x\n", shdr->sh_addralign);
//         printf("sh_info: %u\n", shdr->sh_info);
//         printf("sh_entsize: %x\n", shdr->sh_entsize);
//         printf("-----------\n");
//     }
//     return (1);
// }