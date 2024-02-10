#include <elf.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "elf_header_info.h"
#include "program_header_info.h"
#include "section_header_info.h"

int main(int argc, char** argv)
{
    FILE *fptr = NULL;
    Elf32_Ehdr file_header_32; Elf64_Ehdr file_header_64;
    Elf32_Shdr* section_headers_32 = NULL; Elf64_Shdr* section_headers_64 = NULL; 
    Elf32_Phdr* program_headers_32 = NULL; Elf64_Phdr* program_headers_64 = NULL;

    char print_file_header = 0, print_section_headers = 0, load_program_headers = 0,print_program_header  = 0;
    char load_section_headers = 0;

    int opt = 0, i = 0;

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
                print_program_header = load_program_headers = 1;
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
    
    read_elf32_header(fptr, &file_header_32);

    /* is 32 bits */
    if (file_header_32.e_ident[EI_CLASS] == ELFCLASS32)
    {
        if (load_section_headers)
            section_headers_32 = get_section_headers_32(fptr, &file_header_32);

        if (print_file_header)
            print_elf32_header(&file_header_32);

        if (print_section_headers)
            print_section_headers_32(section_headers_32, &file_header_32, fptr);   
    }
    /* 64 bits */
    else if (file_header_32.e_ident[EI_CLASS] == ELFCLASS64)
    {
        read_elf64_header(fptr, &file_header_64); // get 64 bit info

        if (load_section_headers)
            section_headers_64 = get_section_headers_64(fptr, &file_header_64);

        if (load_program_headers)
            program_headers_64 = get_program_headers_64(fptr, &file_header_64);

        if (print_file_header)
            print_elf64_header(&file_header_64);

        if (print_section_headers)
            print_section_headers_64(section_headers_64, &file_header_64, fptr);   
        
        if (print_program_header)
            print_program_header_64(program_headers_64, &file_header_64, fptr);
    }


    free(program_headers_32);
    free(program_headers_64);
    free(section_headers_32);
    free(section_headers_64);
    fclose(fptr);
    return 0;
}