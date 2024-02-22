#include "program_header_info.h"
#include "stdlib.h"
#include <elf.h>
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

void print_program_header_64(Filedata64* fdata, FILE* f)
{
    printf("\n\n\nThere are %d program headers starting at offset %p:\n\nProgram headers:\n", fdata->file_header.e_phnum, (void*)fdata->file_header.e_phoff);
    printf("  [Num]  Type            Offset            Virtual addr      Physical addr\n");
    printf("                         Size in file      Size in memory    Flags  Align\n");
    char interp[1024] = {0};
    int i  = 0 , len =0;
    unsigned long start_of_segment = 0, end_of_segment = 0;

    for (i = 0; i < fdata->file_header.e_phnum; i++)
    {
        printf("  [%3d]  %-14s  %016lx  %016lx  %016lx\n", 
                i,get_p_type(fdata->program_headers[i].p_type), 
                fdata->program_headers[i].p_offset, fdata->program_headers[i].p_vaddr, 
                fdata->program_headers[i].p_paddr);
        printf("                         %016lx  %016lx  %s    0x%lx\n", 
            fdata->program_headers[i].p_filesz, fdata->program_headers[i].p_memsz, get_p_flags(fdata->program_headers[i].p_flags), fdata->program_headers[i].p_align);

        if (fdata->program_headers[i].p_type == PT_INTERP)
        {
            fseek(f, fdata->program_headers[i].p_offset, SEEK_SET);
            fread(interp, 1, fdata->program_headers[i].p_filesz, f);
            interp[fdata->program_headers[i].p_filesz -1 ] = 0;
            printf("        Interpreter: %s\n", interp);
        }    
    }
    
    printf("  Section to segment mapping:\n    Segment  Sections\n");
    for (i = 0; i < fdata->file_header.e_phnum; i++)
    {
        printf("    %02d      ", i);
        start_of_segment = fdata->program_headers[i].p_vaddr;
        end_of_segment = start_of_segment + fdata->program_headers[i].p_memsz;
        for (int j = 0; j < fdata->file_header.e_shnum; j++)
        {
            if (fdata->section_headers[j].sh_addr >= start_of_segment && 
                fdata->section_headers[j].sh_addr < end_of_segment &&
                fdata->section_headers[j].sh_flags & SHF_ALLOC)
            {
                printf("%s ", &fdata->shstrtab[fdata->section_headers[j].sh_name]);
            }        

        }
        putchar('\n');
    }
}