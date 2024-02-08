#include "section_header_info.h"
#include <stdio.h>
#include <stdlib.h>

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

void print_at_offset(FILE* f, int offset)
{
    fseek(f, offset, SEEK_SET);
    char ch = 0;
    while ((ch = fgetc(f)) != EOF && ch != 0)
        putchar(ch);
}

void print_section_header_64(const Elf64_Shdr* sh, const Elf64_Ehdr* file_header, FILE* f)
{
    int i = 0;
    int sh_str_table = sh[file_header->e_shstrndx].sh_offset;

    for (i = 0; i < file_header->e_shnum; i++)
    {
        print_at_offset(f, sh_str_table + sh[i].sh_name); // name in section header is an offset
        putchar('\n');
    }
}