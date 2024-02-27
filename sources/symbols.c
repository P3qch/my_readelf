#include "symbols.h"
#include <elf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

const char* get_symbol_bind_str(unsigned char st_info)
{
    unsigned char bind = ELF32_ST_BIND(st_info);
    switch (bind)
    {
        case STB_LOCAL:
            return "LOCAL";
        case STB_GLOBAL:
            return "GLOBAL";
        case STB_WEAK:
            return "WEAK";
        default:
            return "";
    }
}

const char* get_symbol_vis_str(unsigned char st_other)
{
    unsigned char vis = ELF32_ST_VISIBILITY(st_other);

    switch(vis)
    {
        case STV_DEFAULT:
            return "DEFAULT";
        case STV_INTERNAL:
            return "INTERNAL";
        case STV_HIDDEN:
            return "HIDDEN";
        case STV_PROTECTED:
            return "PROTECTED";
        default:
            return "";
    }
}

static char* get_shndx_str(uint16_t shndx)
{
    static char result_string[20] = "";

    switch (shndx) 
    {
        case SHN_ABS:
            strcpy(result_string, "ABS");
            break;
        case SHN_UNDEF:
            strcpy(result_string, "UND");
            break;
        case SHN_COMMON:
            strcpy(result_string, "CMN");
            break;
        case SHN_XINDEX:
            strcpy(result_string, "XIN"); // TODO GIVE THIS THE SPECIAL TREATMENT IT NEEDS
            break;
        default:
            result_string[0] = 0;
            sprintf(result_string, "%3hu", shndx);
            break;
    }
    return result_string;
}

void print_symbol_table_32(Filedata32* fdata, Elf32_Shdr* symtab, Elf32_Shdr* strtab, FILE* f)
{
    Elf32_Sym* symbols = (Elf32_Sym*)malloc(symtab->sh_size);
    Elf32_Syminfo syminfo;
    int symbolnum = symtab->sh_size / symtab->sh_entsize;
    char* strings = (char*)malloc(symtab->sh_size);

    printf("\nSymbol table %s contains %d entries.\n", &fdata->shstrtab[symtab->sh_name], symbolnum);

    fseek(f, symtab->sh_offset, SEEK_SET);
    fread(symbols, symtab->sh_size, 1, f);

    fseek(f, strtab->sh_offset, SEEK_SET);
    fread(strings, strtab->sh_size, 1, f);

    printf("  Num: Value             Size  Type     Bind     Vis        NDX  Name\n");
    for (int i = 0; i < symbolnum;i++)
    {
        printf("  %3u: %016x  %4u  %-7s  %-7s  %-9s  %s  %s\n", 
            i, symbols[i].st_value, symbols[i].st_size, 
            get_symbol_type_str(symbols[i].st_info), 
            get_symbol_bind_str(symbols[i].st_info),
            get_symbol_vis_str(symbols[i].st_other),
            get_shndx_str(symbols[i].st_shndx),
            &strings[symbols[i].st_name]);
    }
    free(symbols);
    free(strings);
}

void print_symbol_tables_32(Filedata32* fdata, FILE* f)
{
    Elf32_Shdr* symtab = NULL, * strtab = NULL;
    for (int i = 0; i < fdata->file_header.e_shnum; i++)
    {
        symtab = &fdata->section_headers[i];
        if (symtab->sh_type == SHT_SYMTAB || symtab->sh_type == SHT_DYNSYM)
        {   
            strtab = &fdata->section_headers[symtab->sh_link];
            print_symbol_table_32(fdata, symtab, strtab, f);
        }
    }
}


void print_symbol_table_64(Filedata64* fdata, Elf64_Shdr* symtab, Elf64_Shdr* strtab, FILE* f)
{
    Elf64_Sym* symbols = (Elf64_Sym*)malloc(symtab->sh_size);
    Elf64_Syminfo syminfo;
    int symbolnum = symtab->sh_size / symtab->sh_entsize;
    char* strings = (char*)malloc(symtab->sh_size);

    printf("\nSymbol table %s contains %d entries.\n", &fdata->shstrtab[symtab->sh_name], symbolnum);

    fseek(f, symtab->sh_offset, SEEK_SET);
    fread(symbols, symtab->sh_size, 1, f);

    fseek(f, strtab->sh_offset, SEEK_SET);
    fread(strings, strtab->sh_size, 1, f);

    printf("  Num: Value             Size  Type     Bind     Vis        NDX  Name\n");
    for (int i = 0; i < symbolnum;i++)
    {
        printf("  %3u: %016lx  %4lu  %-7s  %-7s  %-9s  %s  %s\n", 
            i, symbols[i].st_value, symbols[i].st_size, 
            get_symbol_type_str(symbols[i].st_info), 
            get_symbol_bind_str(symbols[i].st_info),
            get_symbol_vis_str(symbols[i].st_other),
            get_shndx_str(symbols[i].st_shndx),
            &strings[symbols[i].st_name]);
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
        if (symtab->sh_type == SHT_SYMTAB || symtab->sh_type == SHT_DYNSYM)
        {   
            strtab = &fdata->section_headers[symtab->sh_link];
            print_symbol_table_64(fdata, symtab, strtab, f);
        }
    }
}