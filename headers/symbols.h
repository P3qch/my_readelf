#pragma once
#include <elf.h>
#ifndef H_SYMBOLS_P
#define H_SYMBOLS_P
#include <stdio.h>
#include "filedata.h"

void print_symbol_table_64(Filedata64* fdata, Elf64_Shdr* symtab, Elf64_Shdr* strtab, FILE* f);

void print_symbol_tables_64(Filedata64* fdata, FILE* f);

#endif