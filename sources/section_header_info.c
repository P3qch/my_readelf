#include "section_header_info.h"
#include <stdio.h>
#include <stdlib.h>

#define SECTION_NAME_LEN 20


Elf32_Shdr* get_section_headers_32(FILE* f, const Elf32_Ehdr* file_header)
{
    Elf32_Shdr* result = (Elf32_Shdr*)calloc(file_header->e_shnum, file_header->e_shentsize);
    fseek(f, file_header->e_shoff, SEEK_SET);
    fread(result, file_header->e_shentsize, file_header->e_shnum, f);
    return result;
}


Elf64_Shdr* get_section_headers_64(FILE* f, const Elf64_Ehdr* file_header)
{
    Elf64_Shdr* result = (Elf64_Shdr*)calloc(file_header->e_shnum, file_header->e_shentsize);
    fseek(f, file_header->e_shoff, SEEK_SET);
    fread(result, file_header->e_shentsize, file_header->e_shnum, f);
    return result;
}

void get_string_at_offset(char* dst, FILE* f, int offset)
{
    fseek(f, offset, SEEK_SET);
    char ch = 0;
    int i = 0;
    while ((ch = fgetc(f)) != EOF && ch != 0 && i < SECTION_NAME_LEN - 1)
    {
        dst[i] = ch;
        i++;
    }
    dst[i] = 0;
}

const char* get_section_type(long type)
{
    switch (type) {
        case SHT_NULL:
            return "NULL";
        case SHT_PROGBITS:
            return "PROGBITS";
        case SHT_SYMTAB:
            return "SYMTAB";
        case SHT_STRTAB:
            return "STRTAB";
        case SHT_RELA:
            return "RELA";
        case SHT_HASH:
            return "HASH";
        case SHT_DYNAMIC:
            return "DYNAMIC";
        case SHT_NOTE:
            return "NOTE";
        case SHT_NOBITS:
            return "NOBITS";
        case SHT_REL:
            return "REL";
        case SHT_SHLIB:
            return "SHLIB";
        case SHT_DYNSYM:
            return "DYNSYM";
        case SHT_INIT_ARRAY:
            return "INIT_ARRAY";
        case SHT_FINI_ARRAY:
            return "FINI_ARRAY";
        case SHT_PREINIT_ARRAY:
            return "PREINIT_ARRAY";
        case SHT_GROUP:
            return "GROUP";
        case SHT_SYMTAB_SHNDX:
            return "SYMTAB_SHNDX";
        case SHT_RELR:
            return "RELR";
        case SHT_NUM:
            return "NUM";
        case SHT_LOOS:
            return "LOOS";
        case SHT_GNU_ATTRIBUTES:
            return "GNU_ATTRIBUTES";
        case SHT_GNU_HASH:
            return "GNU_HASH";
        case SHT_GNU_LIBLIST:
            return "GNU_LIBLIST";
        case SHT_CHECKSUM:
            return "CHECKSUM";
        case SHT_LOSUNW:
            return "LOSUNW";
        case SHT_SUNW_COMDAT:
            return "SUNW_COMDAT";
        case SHT_SUNW_syminfo:
            return "SUNW_syminfo";
        case SHT_GNU_verdef:
            return "GNU_verdef";
        case SHT_GNU_verneed:
            return "GNU_verneed";
        case SHT_GNU_versym:
            return "GNU_versym";
        case SHT_LOPROC:
            return "LOPROC";
        case SHT_HIPROC:
            return "HIPROC";
        case SHT_LOUSER:
            return "LOUSER";
        case SHT_HIUSER:
            return "HIUSER";
        default:
            return "Unknown";
    }
}


void print_section_headers_64(const Elf64_Shdr* sh, const Elf64_Ehdr* file_header, FILE* f)
{
    int sh_str_table = sh[file_header->e_shstrndx].sh_offset;
    char name[SECTION_NAME_LEN] = {0};
    int i = 0;
    puts("Section headers:");
    puts("  [Num]  Name              Type              Address           File offset");
    puts("         Size              Entry Size        ");
    for (i = 0; i < file_header->e_shnum; i++)
    {

        get_string_at_offset(name,f, sh_str_table + sh[i].sh_name); // name in section header is an offset

        printf("  [%3d]  %-16.16s  ", i, name);
        printf("%-16.16s  ", get_section_type(sh[i].sh_type));
        printf("%.016lx  ", sh[i].sh_addr);
        printf("%.016lx\n", sh[i].sh_offset);
        printf("         %016lx  ", sh[i].sh_size);
        printf("%.016lx  ", sh[i].sh_entsize);
        putchar('\n');
    }
}