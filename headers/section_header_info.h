#pragma once
#ifndef H_PROGRAM_HEADER_INFO
#define H_PROGRAM_HEADER_INFO
#include <elf.h>
#include <stdio.h>

Elf32_Shdr* get_section_headers_32(FILE* f, const Elf32_Ehdr* file_header);
Elf64_Shdr* get_section_headers_64(FILE* f, const Elf64_Ehdr* file_header);

void print_section_headers_32(const Elf32_Shdr* sh, const Elf32_Ehdr* file_header, FILE* f);
void print_section_headers_64(const Elf64_Shdr* sh, const Elf64_Ehdr* file_header, FILE* f);


#endif