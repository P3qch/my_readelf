#pragma once
#ifndef H_ELF_HEADER_INFO
#define H_ELF_HEADER_INFO

#include <elf.h>
#include <stdio.h>

/* readers */
void read_elf32_header(FILE* f, Elf32_Ehdr* header);
void read_elf64_header(FILE* f, Elf64_Ehdr* header);

void read_program_header32(FILE* f, Elf32_Ehdr* header, Elf32_Phdr* program_header);
void read_program_header64(FILE* f, Elf64_Ehdr* header, Elf64_Phdr* program_header);

/* printers */
void print_elf32_header(const Elf32_Ehdr* header);
void print_elf64_header(const Elf64_Ehdr* header);

void print_elf_class(int class_);
void print_endianess(int endianess);
void print_osabi_name(int osabi);
void print_file_type(int ft);
#endif