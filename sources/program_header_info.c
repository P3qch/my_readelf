#include "program_header_info.h"
#include "stdlib.h"
#include <stdio.h>

Elf32_Phdr* get_program_headers_32(FILE* f, const Elf32_Ehdr* file_header)
{
    Elf32_Phdr* result = (Elf32_Phdr*)calloc(file_header->e_phnum, file_header->e_phentsize);

    fseek(f, file_header->e_phoff, SEEK_SET);
    fread(result, file_header->e_phentsize, file_header->e_phnum, f);
    return result;
}


Elf64_Phdr* get_program_headers_64(FILE* f, const Elf64_Ehdr* file_header)
{
    Elf64_Phdr* result = (Elf64_Phdr*)calloc(file_header->e_phnum, file_header->e_phentsize);

    fseek(f, file_header->e_phoff, SEEK_SET);
    fread(result, file_header->e_phentsize, file_header->e_phnum, f);
    return result;
}

const char* get_p_type(const int p_type)
{
    switch (p_type)
    {
        case PT_NULL:
            return "NULL";
        case PT_LOAD:
            return "LOAD";
        case PT_DYNAMIC:
            return "DYNAMIC";
        case PT_INTERP:
            return "INTERP";
        case PT_NOTE:
            return "NOTE";
        case PT_SHLIB:
            return "SHLIB";
        case PT_PHDR:
            return "PHDR";
        case PT_TLS:
            return "TLS";
        case PT_GNU_EH_FRAME:
            return "GNU_EH_FRAME";
        case PT_GNU_STACK:
            return "GNU_STACK";
        case PT_GNU_RELRO:
            return "GNU_RELRO";
        case PT_GNU_PROPERTY:
            return "GNU_PROPERTY";
    }

    return "";
}

static char * get_p_flags(int p_flags)
{
    static char buf[4] = {0};
    buf[0] = p_flags & PF_R ? 'R' : '-';
    buf[1] = p_flags & PF_W ? 'W' : '-'; 
    buf[2] = p_flags & PF_X ? 'X' : '-';
    buf[3] = 0;
    return buf;
}

void print_program_header_64(Elf64_Phdr* program_header, Elf64_Ehdr* file_header, FILE* f)
{
    printf("There are %d program headers starting at offset %p:\nProgram headers:\n", file_header->e_phnum, (void*)file_header->e_phoff);
    printf("  Type            Offset            Virtual addr      Physical addr\n");
    printf("                  Size in file      Size in memory    Flags  Align\n");
    char interp[1024] = {0};
    for (int i = 0; i < file_header->e_phnum; i++)
    {
        printf("  %-14s  %016lx  %016lx  %016lx\n", 
                get_p_type(program_header[i].p_type), 
                program_header[i].p_offset, program_header[i].p_vaddr, 
                program_header[i].p_paddr);
        printf("                  %016lx  %016lx  %s    0x%lx\n", 
            program_header[i].p_filesz, program_header[i].p_memsz, get_p_flags(program_header[i].p_flags), program_header[i].p_align);

        if (program_header[i].p_type == PT_INTERP)
        {
            fseek(f, program_header[i].p_offset, SEEK_SET);
            fread(interp, 1, program_header->p_filesz, f);
            interp[program_header->p_filesz -1 ] = 0;
            printf("        Interpreter: %s\n", interp);
        }    
    }
}