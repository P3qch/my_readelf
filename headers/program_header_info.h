#pragma once
#ifndef H_PROGRAM_HEADER_INFO
#define H_PROGRAM_HEADER_INFO
#include <elf.h>
#include <stdio.h>
#include "filedata.h"

Elf32_Phdr* get_program_headers_32(FILE* f, const Elf32_Ehdr* file_header);
Elf64_Phdr* get_program_headers_64(FILE* f, const Elf64_Ehdr* file_header);

void print_program_header_32(Filedata32* fdata, FILE* f);
void print_program_header_64(Filedata64* fdata, FILE* f);


#endif