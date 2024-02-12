#pragma once
#ifndef H_SECTION_HEADER_INFO
#define H_SECTION_HEADER_INFO
#include <elf.h>
#include <stdio.h>
#include "filedata.h"

Elf32_Shdr* get_section_headers_32(FILE* f, const Elf32_Ehdr* file_header);
Elf64_Shdr* get_section_headers_64(FILE* f, const Elf64_Ehdr* file_header);

void print_section_headers_32(Filedata32* fdata, FILE* f);
void print_section_headers_64(Filedata64* fdata, FILE* f);


#endif