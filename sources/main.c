#include <elf.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "elf_header_info.h"
#include "program_header_info.h"
#include "section_header_info.h"
#include "filedata.h"

int main(int argc, char** argv)
{
    FILE *fptr = NULL;
    Filedata32 fdata32; Filedata64 fdata64;

    char print_file_header = 0, print_section_headers = 0, load_program_headers = 0,print_program_header  = 0;
    char load_section_headers = 0;

    int opt = 0;

    while ((opt = getopt(argc, argv, "hSl")) != -1)
    {
        switch (opt) {
            case 'h':
                print_file_header = 1;
                break;
            case 'S':
                print_section_headers = load_section_headers = 1;
                break;
            case 'l':
                print_program_header = load_program_headers = load_section_headers = 1;
                break;
            default:
                break;
        }
    }

    if (optind >= argc)
    {
        puts("Please supply file");
        return 1;
    }

    fptr = fopen(argv[optind], "rb");

    if (!fptr)
    {
        perror("Can not open file");
        return 1;
    }
    
    read_elf32_header(fptr, &fdata32.file_header);

    /* is 32 bits */
    if (fdata32.file_header.e_ident[EI_CLASS] == ELFCLASS32)
    {
        if (load_section_headers)
        {
            fdata32.section_headers = get_section_headers_32(fptr, &fdata32.file_header);
            get_shstrtab_32(fptr, &fdata32);
        }
        if (print_file_header)
            print_elf32_header(&fdata32.file_header);

        if (print_section_headers)
            print_section_headers_32(&fdata32, fptr);   
        
    }
    /* 64 bits */
    else if (fdata32.file_header.e_ident[EI_CLASS] == ELFCLASS64)
    {
        read_elf64_header(fptr, &fdata64.file_header); // get 64 bit info

        if (load_section_headers)
        {
            fdata64.section_headers = get_section_headers_64(fptr, &fdata64.file_header);
            get_shstrtab_64(fptr, &fdata64);
        }
        if (load_program_headers)
            fdata64.program_headers = get_program_headers_64(fptr, &fdata64.file_header);

        if (print_file_header)
            print_elf64_header(&fdata64.file_header);

        if (print_section_headers)
            print_section_headers_64(&fdata64, fptr);   
        
        if (print_program_header)
            print_program_header_64(&fdata64, fptr);
    }

    free(fdata32.section_headers);
    free(fdata32.program_headers);
    free(fdata32.shstrtab);
    free(fdata64.section_headers);
    free(fdata64.program_headers);
    free(fdata64.shstrtab);
    fclose(fptr);
    return 0;
}