#pragma once
#ifndef H_PROGRAM_HEADER_INFO
#define H_PROGRAM_HEADER_INFO
#include <elf.h>
#include <stdio.h>

Elf32_Phdr* get_program_headers_32(FILE* f, const Elf32_Ehdr* file_header);
Elf64_Phdr* get_program_headers_64(FILE* f, const Elf64_Ehdr* file_header);

void print_program_header_32(Elf32_Phdr* program_header, Elf32_Ehdr* file_header, FILE* f);
void print_program_header_64(Elf64_Phdr* program_header, Elf64_Ehdr* file_header, FILE* f);


#endif