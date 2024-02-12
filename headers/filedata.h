#pragma once
#ifndef H_FILEDATA
#define H_FILEDATA
#include <elf.h>

typedef struct {
    Elf32_Ehdr file_header;
    Elf32_Shdr* section_headers; /* free this */
    Elf32_Phdr* program_headers; /* free this */
    char* shstrtab; /* free this */
} Filedata32;

typedef struct {
    Elf64_Ehdr file_header;
    Elf64_Shdr* section_headers; /* free this */
    Elf64_Phdr* program_headers; /* free this */
    char* shstrtab;
} Filedata64;

#endif