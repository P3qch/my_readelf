#include "program_header_info.h"
#include "stdlib.h"
#include <elf.h>
#include <stdio.h>

#define IN_RANGE(N, MIN, MAX) ((N) >= (MIN) && (N) <= (MAX))

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


int section_in_segment_32(Elf32_Shdr* section, Elf32_Phdr* segment)
{
    /* Only PT_LOAD, PT_GNU_RELRO and PT_TLS segments can contain SHF_TLS sections */
    if (section->sh_flags & SHF_TLS)
    {
        if (!(segment->p_type == PT_LOAD || segment->p_type == PT_GNU_RELRO || segment->p_type & PT_TLS))
            return 0;
    }
    /* Non-TLS sections cannot be in PT_TLS or PT_PHDR segments. */
    else 
    {
        if (segment->p_type == PT_TLS || segment->p_type == PT_PHDR)
            return 0;
    }

    /* PT_LOAD and similar segments only have SHF_ALLOC sections */
    if (!(section->sh_flags & SHF_ALLOC)) {
        if (segment->p_type == PT_LOAD || segment->p_type == PT_DYNAMIC ||
            segment->p_type == PT_GNU_EH_FRAME || segment->p_type == PT_GNU_STACK ||
            segment->p_type == PT_GNU_RELRO)
            return 0;
    }

    /* All section types excpet SHT_NOBITS must have offset inside segment offset */
    if (section->sh_type != SHT_NOBITS)
    {
        if (!IN_RANGE(section->sh_offset,                    segment->p_offset, segment->p_offset + segment->p_filesz) ||
            !IN_RANGE(section->sh_offset + section->sh_size, segment->p_offset, segment->p_offset + segment->p_filesz))
            return 0;
    }

    if (section->sh_flags & SHF_ALLOC)
    {
        if (!IN_RANGE(section->sh_addr,                    segment->p_vaddr, segment->p_vaddr + segment->p_memsz) ||
            !IN_RANGE(section->sh_addr + section->sh_size, segment->p_vaddr, segment->p_vaddr + segment->p_memsz))
            return 0;
    }

    return 1;
}


int section_in_segment_64(Elf64_Shdr* section, Elf64_Phdr* segment)
{
    /* Only PT_LOAD, PT_GNU_RELRO and PT_TLS segments can contain SHF_TLS sections */
    if (section->sh_flags & SHF_TLS)
    {
        if (!(segment->p_type == PT_LOAD || segment->p_type == PT_GNU_RELRO || segment->p_type & PT_TLS))
            return 0;
    }
    /* Non-TLS sections cannot be in PT_TLS or PT_PHDR segments. */
    else 
    {
        if (segment->p_type == PT_TLS || segment->p_type == PT_PHDR)
            return 0;
    }

    /* PT_LOAD and similar segments only have SHF_ALLOC sections */
    if (!(section->sh_flags & SHF_ALLOC)) {
        if (segment->p_type == PT_LOAD || segment->p_type == PT_DYNAMIC ||
            segment->p_type == PT_GNU_EH_FRAME || segment->p_type == PT_GNU_STACK ||
            segment->p_type == PT_GNU_RELRO)
            return 0;
    }

    /* All section types excpet SHT_NOBITS must have offset inside segment offset */
    if (section->sh_type != SHT_NOBITS)
    {
        if (!IN_RANGE(section->sh_offset,                    segment->p_offset, segment->p_offset + segment->p_filesz) ||
            !IN_RANGE(section->sh_offset + section->sh_size, segment->p_offset, segment->p_offset + segment->p_filesz))
            return 0;
    }

    if (section->sh_flags & SHF_ALLOC)
    {
        if (!IN_RANGE(section->sh_addr,                    segment->p_vaddr, segment->p_vaddr + segment->p_memsz) ||
            !IN_RANGE(section->sh_addr + section->sh_size, segment->p_vaddr, segment->p_vaddr + segment->p_memsz))
            return 0;
    }

    return 1;
}


void print_program_header_32(Filedata32* fdata, FILE* f)
{
    printf("\n\n\nThere are %d program headers starting at offset 0x%x:\n\nProgram headers:\n", fdata->file_header.e_phnum, fdata->file_header.e_phoff);
    printf("  [Num]  Type            Offset            Virtual addr      Physical addr\n");
    printf("                         Size in file      Size in memory    Flags  Align\n");
    char interp[1024] = {0};
    int i  = 0 , len =0;


    for (i = 0; i < fdata->file_header.e_phnum; i++)
    {
        printf("  [%3d]  %-14s  %016x  %016x  %016x\n", 
                i, get_p_type(fdata->program_headers[i].p_type), 
                fdata->program_headers[i].p_offset, fdata->program_headers[i].p_vaddr, 
                fdata->program_headers[i].p_paddr);
        printf("                         %016x  %016x  %s    0x%x\n", 
            fdata->program_headers[i].p_filesz, fdata->program_headers[i].p_memsz, get_p_flags(fdata->program_headers[i].p_flags), fdata->program_headers[i].p_align);

        if (fdata->program_headers[i].p_type == PT_INTERP)
        {
            fseek(f, fdata->program_headers[i].p_offset, SEEK_SET);
            fread(interp, 1, fdata->program_headers[i].p_filesz, f);
            interp[fdata->program_headers[i].p_filesz -1 ] = 0;
            printf("        Interpreter: %s\n", interp);
        }    
    }
    
    printf("  Section to segment mapping:\n    Segment Sections\n");
    for (i = 0; i < fdata->file_header.e_phnum; i++)
    {
        printf("    %02d    ", i);
        for (int j = 0; j < fdata->file_header.e_shnum; j++)
        {
            if (section_in_segment_32(&fdata->section_headers[j], &fdata->program_headers[i]))
            {
                printf("%s ", &fdata->shstrtab[fdata->section_headers[j].sh_name]);
            }        

        }
        putchar('\n');
    }
}


void print_program_header_64(Filedata64* fdata, FILE* f)
{
    printf("\n\n\nThere are %d program headers starting at offset %p:\n\nProgram headers:\n", fdata->file_header.e_phnum, (void*)fdata->file_header.e_phoff);
    printf("  [Num]  Type            Offset            Virtual addr      Physical addr\n");
    printf("                         Size in file      Size in memory    Flags  Align\n");
    char interp[1024] = {0};
    int i  = 0 , len =0;


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
    
    printf("  Section to segment mapping:\n    Segment Sections\n");
    for (i = 0; i < fdata->file_header.e_phnum; i++)
    {
        printf("    %02d    ", i);
        for (int j = 0; j < fdata->file_header.e_shnum; j++)
        {
            if (section_in_segment_64(&fdata->section_headers[j], &fdata->program_headers[i]))
            {
                printf("%s ", &fdata->shstrtab[fdata->section_headers[j].sh_name]);
            }        

        }
        putchar('\n');
    }
}