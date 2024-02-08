#include "elf_header_info.h"
#include <assert.h>
#include <string.h>

const char* architecture_names[] = {
    "No machine",
    "AT&T WE 32100",
    "SUN SPARC",
    "Intel 80386",
    "Motorola m68k family",
    "Motorola m88k family",
    "Intel MCU",
    "Intel 80860",
    "MIPS R3000 big-endian",
    "IBM System/370",
    "MIPS R3000 little-endian",
    "reserved","reserved","reserved","reserved",
    "HPPA",
    "reserved",
    "Fujitsu VPP500",
    "Sun's 'v8plus'",
    "Intel 80960",
    "PowerPC",
    "PowerPC 64-bit",
    "IBM S390",
    "IBM SPU/SPC",
    "reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved",
    "NEC V800 series",
    "Fujitsu FR20",
    "TRW RH-32",
    "Motorola RCE",
    "ARM",
    "Digital Alpha",
    "Hitachi SH",
    "SPARC v9 64-bit",
    "Siemens Tricore",
    "Argonaut RISC Core",
    "Hitachi H8/300",
    "Hitachi H8/300H",
    "Hitachi H8S",
    "Hitachi H8/500",
    "Intel Merced",
    "Stanford MIPS-X",
    "Motorola Coldfire",
    "Motorola M68HC12",
    "Fujitsu MMA Multimedia Accelerator",
    "Siemens PCP",
    "Sony nCPU embedded RISC",
    "Denso NDR1 microprocessor",
    "Motorola Start*Core processor",
    "Toyota ME16 processor",
    "STMicroelectronics ST100 processor",
    "Advanced Logic Corp. Tinyj emb. fam",
    "AMD x86-64 architecture",
    "Sony DSP Processor",
    "Digital PDP-10",
    "Digital PDP-11",
    "Siemens FX66 microcontroller",
    "STMicroelectronics ST9+ 8/16 mc",
    "STmicroelectronics ST7 8 bit mc",
    "Motorola MC68HC16 microcontroller",
    "Motorola MC68HC11 microcontroller",
    "Motorola MC68HC08 microcontroller",
    "Motorola MC68HC05 microcontroller",
    "Silicon Graphics SVx",
    "STMicroelectronics ST19 8 bit mc",
    "Digital VAX",
    "Axis Communications 32-bit emb.proc",
    "Infineon Technologies 32-bit emb.proc",
    "Element 14 64-bit DSP Processor",
    "LSI Logic 16-bit DSP Processor",
    "Donald Knuth's educational 64-bit proc",
    "Harvard University machine-independent object files",
    "SiTera Prism",
    "Atmel AVR 8-bit microcontroller",
    "Fujitsu FR30",
    "Mitsubishi D10V",
    "Mitsubishi D30V",
    "NEC v850",
    "Mitsubishi M32R",
    "Matsushita MN10300",
    "Matsushita MN10200",
    "picoJava",
    "OpenRISC 32-bit embedded processor",
    "ARC International ARCompact",
    "Tensilica Xtensa Architecture",
    "Alphamosaic VideoCore",
    "Thompson Multimedia General Purpose Proc",
    "National Semi. 32000",
    "Tenor Network TPC",
    "Trebia SNP 1000",
    "STMicroelectronics ST200",
    "Ubicom IP2xxx",
    "MAX processor",
    "National Semi. CompactRISC",
    "Fujitsu F2MC16",
    "Texas Instruments msp430",
    "Analog Devices Blackfin DSP",
    "Seiko Epson S1C33 family",
    "Sharp embedded microprocessor",
    "Arca RISC",
    "PKU-Unity & MPRC Peking Uni. mc series",
    "eXcess configurable cpu",
    "Icera Semi. Deep Execution Processor",
    "Altera Nios II",
    "National Semi. CompactRISC CRX",
    "Motorola XGATE",
    "Infineon C16x/XC16x",
    "Renesas M16C",
    "Microchip Technology dsPIC30F",
    "Freescale Communication Engine RISC",
    "Renesas M32C",
    "reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved",/* reserved 121-130 */
    "Altium TSK3000",
    "Freescale RS08",
    "Analog Devices SHARC family",
    "Cyan Technology eCOG2",
    "Sunplus S+core7 RISC",
    "New Japan Radio (NJR) 24-bit DSP",
    "Broadcom VideoCore III",
    "RISC for Lattice FPGA",
    "Seiko Epson C17",
    "Texas Instruments TMS320C6000 DSP",
    "Texas Instruments TMS320C2000 DSP",
    "Texas Instruments TMS320C55x DSP",
    "Texas Instruments App. Specific RISC",
    "Texas Instruments Prog. Realtime Unit",
    "reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved",/* reserved 145-159 */
    "STMicroelectronics 64bit VLIW DSP",
    "Cypress M8C",
    "Renesas R32C",
    "NXP Semi. TriMedia",
    "QUALCOMM DSP6",
    "Intel 8051 and variants",
    "STMicroelectronics STxP7x",
    "Andes Tech. compact code emb. RISC",
    "Cyan Technology eCOG1X",
    "Dallas Semi. MAXQ30 mc",
    "New Japan Radio (NJR) 16-bit DSP",
    "M2000 Reconfigurable RISC",
    "Cray NV2 vector architecture",
    "Renesas RX",
    "Imagination Tech. META",
    "MCST Elbrus",
    "Cyan Technology eCOG16",
    "National Semi. CompactRISC CR16",
    "Freescale Extended Time Processing Unit",
    "Infineon Tech. SLE9X",
    "Intel L10M",
    "Intel K10M",
    "reserved",
    "ARM AARCH64",
    "reserved",
    "Amtel 32-bit microprocessor",
    "STMicroelectronics STM8",
    "Tilera TILE64",
    "Tilera TILEPro",
    "Xilinx MicroBlaze",
    "NVIDIA CUDA",
    "Tilera TILE-Gx",
    "CloudShield",
    "KIPO-KAIST Core-A 1st gen.",
    "KIPO-KAIST Core-A 2nd gen.",
    "Synopsys ARCv2 ISA.",
    "Open8 RISC",
    "Renesas RL78",
    "Broadcom VideoCore V",
    "Renesas 78KOR",
    "Freescale 56800EX DSC",
    "Beyond BA1",
    "Beyond BA2",
    "XMOS xCORE",
    "Microchip 8-bit PIC(r)",
    "Intel Graphics Technology",
    "reserved","reserved","reserved","reserved",
    "KM211 KM32",
    "KM211 KMX32",
    "KM211 KMX16",
    "KM211 KMX8",
    "KM211 KVARC",
    "Paneve CDP",
    "Cognitive Smart Memory Processor",
    "Bluechip CoolEngine",
    "Nanoradio Optimized RISC",
    "CSR Kalimba",
    "Zilog Z80",
    "Controls and Data Services VISIUMcore",
    "FTDI Chip FT32",
    "Moxie processor",
    "AMD GPU",
    "reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved","reserved",
    
    "RISC-V",
    "reserved","reserved","reserved",
    "Linux BPF -- in-kernel virtual machine",
    "reserved","reserved","reserved","reserved",
    "C-SKY",
    "reserved","reserved","reserved","reserved","reserved",
    "LoongArch"
};


void read_elf32_header(FILE* f, Elf32_Ehdr* header)
{
    fseek(f, 0, SEEK_SET);
    fread(header, sizeof(Elf32_Ehdr), 1, f);
    assert(strncmp((const char *)header->e_ident, "\x7f""ELF", 4) == 0); // make sure it is an elf file
}

void read_elf64_header(FILE* f, Elf64_Ehdr* header)
{
    fseek(f, 0, SEEK_SET);
    fread(header, sizeof(Elf64_Ehdr), 1, f);
    assert(strncmp((const char *)header->e_ident, "\x7f""ELF", 4) == 0); // make sure it is an elf file
}

void print_elf32_header(const Elf32_Ehdr* header)
{
    printf("ELF header:\n");
    printf("  Magic:  ");
    for (int i = 0; i < EI_NIDENT; i++)
    {
        printf("%02x ", header->e_ident[i]);
    }
    printf("\n  Class:                             ");
    print_elf_class(header->e_ident[EI_CLASS]);
    printf("  Endianness:                        ");
    print_endianess(header->e_ident[EI_DATA]);
    printf("  File version:                      %d\n", header->e_ident[EI_VERSION]);
    printf("  OS ABI:                            ");
    print_osabi_name(header->e_ident[EI_OSABI]);
    printf("  File version:                      %d\n", header->e_ident[EI_ABIVERSION]);
    printf("  File type:                         ");
    print_file_type(header->e_type);
    printf("  Machine type (architecture):       %s\n", architecture_names[header->e_machine]);
    printf("  Entry address:                     0x%x\n", header->e_entry);
    printf("  Program header offset:             %d (bytes into this file)\n", header->e_phoff);
    printf("  Section header offset:             %d (bytes into this file)\n", header->e_shoff);
    printf("  Flags:                             %d\n", header->e_flags);
    printf("  Size of this header:               %d bytes\n", header->e_ehsize);
    printf("  Program header entry size:         %d bytes\n", header->e_phentsize);
    printf("  Program header entry count:        %d entries\n", header->e_phnum);
    printf("  Section header entry size:         %d bytes\n", header->e_shentsize);
    printf("  Section header entry count:        %d entries\n", header->e_shnum);
    printf("  Section header string table index: %d\n", header->e_shstrndx);
}

void print_elf64_header(const Elf64_Ehdr* header)
{
    printf("ELF header:\n");
    printf("  Magic:  ");
    for (int i = 0; i < EI_NIDENT; i++)
    {
        printf("%02x ", header->e_ident[i]);
    }
    printf("\n  Class:                             ");
    print_elf_class(header->e_ident[EI_CLASS]);
    printf("  Endianness:                        ");
    print_endianess(header->e_ident[EI_DATA]);
    printf("  File version:                      %d\n", header->e_ident[EI_VERSION]);
    printf("  OS ABI:                            ");
    print_osabi_name(header->e_ident[EI_OSABI]);
    printf("  File version:                      %d\n", header->e_ident[EI_ABIVERSION]);
    printf("  File type:                         ");
    print_file_type(header->e_type);
    printf("  Machine type (architecture):       %s\n", architecture_names[header->e_machine]);
    printf("  Entry address:                     %p\n", (void*)header->e_entry);
    printf("  Program header offset:             %ld (bytes into this file)\n", header->e_phoff);
    printf("  Section header offset:             %ld (bytes into this file)\n", header->e_shoff);
    printf("  Flags:                             %d\n", header->e_flags);
    printf("  Size of this header:               %hd bytes\n", header->e_ehsize);
    printf("  Program header entry size:         %hd bytes\n", header->e_phentsize);
    printf("  Program header entry count:        %hd entries\n", header->e_phnum);
    printf("  Section header entry size:         %hd bytes\n", header->e_shentsize);
    printf("  Section header entry count:        %hd entries\n", header->e_shnum);
    printf("  Section header string table index: %hd\n", header->e_shstrndx);
}



void print_elf_class(int class_)
{
    switch (class_) 
    {
        case ELFCLASS32:
            puts("ELF32");
            break;
        case ELFCLASS64:
            puts("ELF64");
            break;
        case ELFCLASSNONE:
            puts("Invalid class");
            break;
        default:
            puts("Unknown class");
            break;
    }

}

void print_endianess(int endianess)
{
    switch (endianess) {
        case ELFDATA2LSB:
            puts("2's complement, little endian");
            break;
        case ELFDATA2MSB:
            puts("2's complement, big endian");
            break;
        default:
            puts("invalid endianess");
    }
}

void print_osabi_name(int osabi) {
    switch (osabi) {
        case ELFOSABI_NONE:
            printf("UNIX System V ABI\n");
            break;
        case ELFOSABI_HPUX:
            printf("HP-UX\n");
            break;
        case ELFOSABI_NETBSD:
            printf("NetBSD\n");
            break;
        case ELFOSABI_GNU:
            printf("GNU ELF extensions\n");
            break;
        case ELFOSABI_SOLARIS:
            printf("Sun Solaris\n");
            break;
        case ELFOSABI_AIX:
            printf("IBM AIX\n");
            break;
        case ELFOSABI_IRIX:
            printf("SGI Irix\n");
            break;
        case ELFOSABI_FREEBSD:
            printf("FreeBSD\n");
            break;
        case ELFOSABI_TRU64:
            printf("Compaq TRU64 UNIX\n");
            break;
        case ELFOSABI_MODESTO:
            printf("Novell Modesto\n");
            break;
        case ELFOSABI_OPENBSD:
            printf("OpenBSD\n");
            break;
        case ELFOSABI_ARM_AEABI:
            printf("ARM EABI\n");
            break;
        case ELFOSABI_ARM:
            printf("ARM\n");
            break;
        case ELFOSABI_STANDALONE:
            printf("Standalone (embedded) application\n");
            break;
        default:
            printf("Unknown OSABI\n");
            break;
    }
}

void print_file_type(int ft) {
    switch (ft) {
        case ET_NONE:
            printf("No file type\n");
            break;
        case ET_REL:
            printf("Relocatable file\n");
            break;
        case ET_EXEC:
            printf("Executable file\n");
            break;
        case ET_DYN:
            printf("Shared object file (PIE enabled executable)\n");
            break;
        case ET_CORE:
            printf("Core file\n");
            break;
        default:
            if(ft >= ET_LOOS && ft <= ET_HIOS)
            {
                printf("OS specific - %d\n", ft);
            }
            else if (ft >= ET_LOPROC && ft <= ET_HIPROC)
            {
                printf("Processor specific - %d\n", ft);
            }
            else 
            {
                printf("Unknown file type\n");
            }
            break;
    }
}