#include "symbols.h"
#include <elf.h>
#include <stdlib.h>

const char* get_symbol_type_str(unsigned char st_info)
{
    unsigned char type = ELF32_ST_TYPE(st_info);
    switch (type) // its ok to use the ELF32 version, as the st_info field is structured the same way in both 32 and 64 bit systems.
    {
        case STT_NOTYPE:
            return "NOTYPE";
        case STT_OBJECT:
            return "OBJECT";
        case STT_FUNC:
            return "FUNC";
        case STT_SECTION:
            return "SECTION";
        case STT_FILE:
            return "FILE";
        case STT_COMMON:
            return "COMMON";
        case STT_TLS:
            return "TLS";
        default:
            return "";
    }
}

void print_symbol_table_64(Filedata64* fdata, Elf64_Shdr* symtab, Elf64_Shdr* strtab, FILE* f)
{
    Elf64_Sym* symbols = (Elf64_Sym*)malloc(symtab->sh_size);
    Elf64_Syminfo syminfo;
    int symbolnum = symtab->sh_size / symtab->sh_entsize;
    char* strings = (char*)malloc(symtab->sh_size);

    printf("Symbol table %s contains %d entries.\n\n", &fdata->shstrtab[symtab->sh_name], symbolnum);

    fseek(f, symtab->sh_offset, SEEK_SET);
    fread(symbols, symtab->sh_size, 1, f);

    fseek(f, strtab->sh_offset, SEEK_SET);
    fread(strings, strtab->sh_size, 1, f);

    printf("  Num: Value            Type\n");
    for (int i = 0; i < symbolnum;i++)
    {
        printf("  %03u: %016lx  %04lu", i, symbols[i].st_value, symbols[i].st_size);
    }
    free(symbols);
    free(strings);
}

void print_symbol_tables_64(Filedata64* fdata, FILE* f)
{
    Elf64_Shdr* symtab = NULL, * strtab = NULL;
    for (int i = 0; i < fdata->file_header.e_shnum; i++)
    {
        symtab = &fdata->section_headers[i];
        if (symtab->sh_type == SHT_SYMTAB)
        {   
            strtab = &fdata->section_headers[symtab->sh_link];
            print_symbol_table_64(fdata, symtab, strtab, f);
        }
    }
}