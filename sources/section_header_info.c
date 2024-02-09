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


static char * get_section_flags(int flags)
{
    static char buff[17];
    char flag;
    char* p = buff;
    if (flags & SHF_WRITE)
    {
        *p = 'W';
        p++;
    }
    if (flags & SHF_ALLOC)
    {
        *p = 'A';
        p++;
    }
    if (flags & SHF_EXECINSTR)
    {
        *p = 'X';
        p++;
    }
    if (flags & SHF_MERGE)
    {
        *p = 'M';
        p++;
    }
    if (flags & SHF_STRINGS)
    {
        *p = 'S';
        p++;
    }
    if (flags & SHF_INFO_LINK)
    {
        *p = 'I';
        p++;
    }
    if (flags & SHF_LINK_ORDER)
    {
        *p = 'L';
        p++;
    }
    if (flags & SHF_OS_NONCONFORMING)
    {
        *p = 'O';
        p++;
    }
    if (flags & SHF_GROUP)
    {
        *p = 'G';
        p++;
    }
    if (flags & SHF_TLS)
    {
        *p = 'T';
        p++;
    }
    if (flags & SHF_COMPRESSED)
    {
        *p = 'C';
        p++;
    }
    if (flags & SHF_MASKOS)
    {
        *p = 'o';
        p++;
    }
    if (flags & SHF_MASKPROC)
    {
        *p = 'p';
        p++;
    }
    if (flags & SHF_GNU_RETAIN)
    {
        *p = 'R';
        p++;
    }
    if (flags & SHF_ORDERED)
    {
        *p = 'r';
        p++;
    }
    if (flags & SHF_EXCLUDE)
    {
        *p = 'E';
        p++;
    }
    *p = 0;
    return buff;
}

void print_section_headers_32(const Elf32_Shdr* sh, const Elf32_Ehdr* file_header, FILE* f)
{
    int sh_str_table = sh[file_header->e_shstrndx].sh_offset;
    char name[SECTION_NAME_LEN] = {0};
    int i = 0;
    puts("Section headers:");
    puts("  [Num]  Name                  Type              Address           File offset       Entry size");
    puts("         Size                  Entry Size        flags             Link  Info  Align");
    puts("  ========================================================================================================");
    for (i = 0; i < file_header->e_shnum; i++)
    {

        get_string_at_offset(name,f, sh_str_table + sh[i].sh_name); // name in section header is an offset

        printf("  [%3d]  %-20.20s  %-16.16s  %.016x  %.016x  %.016x\n", 
                    i,   name,  get_section_type(sh[i].sh_type), sh[i].sh_addr, sh[i].sh_offset, sh[i].sh_entsize);
        printf("         %016x      %.016x  %-16.16s  %4x  %4x   %4x\n",
                 sh[i].sh_size, sh[i].sh_entsize, get_section_flags(sh[i].sh_flags), sh[i].sh_link, sh[i].sh_info, (unsigned int)sh[i].sh_addralign);
    }
    puts("  Flag key: W - Should be writable during execution, A - Occupies memory during execution, X - Executable,");
    puts("            M - read here: https://docs.oracle.com/cd/E23824_01/html/819-0690/ggdlu.html, ");
    puts("            S - contains  null terminated strings, character size specified in the Entry Size field,");
    puts("            I - the Info field contains a section index, L - Preserve order after combining,");
    puts("            O - non-standard OS specific handling required, G - Section is member of a group, T - Contains TLS");
    puts("            C - Section with compressed data, o - os specific flags, p - Processor specific flag, R - section should not be");
    puts("            garbage collected by linker, r - special ordering requirement, E - section is excluded unless referenced or allocated");

}

void print_section_headers_64(const Elf64_Shdr* sh, const Elf64_Ehdr* file_header, FILE* f)
{
    int sh_str_table = sh[file_header->e_shstrndx].sh_offset;
    char name[SECTION_NAME_LEN] = {0};
    int i = 0;
    puts("Section headers:");
    puts("  [Num]  Name                  Type              Address           File offset       Entry size");
    puts("         Size                  Entry Size        flags             Link  Info  Align");
    puts("  ========================================================================================================");
    for (i = 0; i < file_header->e_shnum; i++)
    {

        get_string_at_offset(name,f, sh_str_table + sh[i].sh_name); // name in section header is an offset

        printf("  [%3d]  %-20.20s  %-16.16s  %.016lx  %.016lx  %.016lx\n", 
                    i,   name,  get_section_type(sh[i].sh_type), sh[i].sh_addr, sh[i].sh_offset, sh[i].sh_entsize);
        printf("         %016lx      %.016lx  %-16.16s  %4x  %4x   %4x\n",
                 sh[i].sh_size, sh[i].sh_entsize, get_section_flags(sh[i].sh_flags), sh[i].sh_link, sh[i].sh_info, (unsigned int)sh[i].sh_addralign);
    }
    puts("  Flag key: W - Should be writable during execution, A - Occupies memory during execution, X - Executable,");
    puts("            M - read here: https://docs.oracle.com/cd/E23824_01/html/819-0690/ggdlu.html, ");
    puts("            S - contains  null terminated strings, character size specified in the Entry Size field,");
    puts("            I - the Info field contains a section index, L - Preserve order after combining,");
    puts("            O - non-standard OS specific handling required, G - Section is member of a group, T - Contains TLS");
    puts("            C - Section with compressed data, o - os specific flags, p - Processor specific flag, R - section should not be");
    puts("            garbage collected by linker, r - special ordering requirement, E - section is excluded unless referenced or allocated");
}