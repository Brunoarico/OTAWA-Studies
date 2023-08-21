/*
 * GEL++ ELF definitions
 * Copyright (c) 2016, IRIT- université de Toulouse
 *
 * GEL++ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * GEL++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GEL++; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef GEL_ELF_DEFS_H
#define GEL_ELF_DEFS_H

#include <elm/types.h>
#include "common.h"
#include <config.h>

namespace gel { namespace elf {

using namespace elm;

typedef t::uint32 Elf32_Addr;
typedef t::uint16 Elf32_Half;
typedef t::uint32 Elf32_Off;
typedef t::int32  Elf32_Sword;
typedef t::uint32 Elf32_Word;

 /* auxiliairy vector types */
 #define AT_NULL			0
 #define AT_IGNORE		1
 #define AT_EXECFD		2
 #define AT_PHDR		3
 #define AT_PHENT		4
 #define AT_PHNUM		5
 #define AT_PAGESZ		6
 #define AT_BASE		7
 #define AT_FLAGS		8
 #define AT_ENTRY		9
 #define AT_DCACHEBSIZE	10
 #define AT_ICACHEBSIZE	11
 #define AT_UCACHEBSIZE	12


typedef struct Elf32_Ehdr {
        unsigned char e_ident[EI_NIDENT];
        Elf32_Half    e_type;
        Elf32_Half    e_machine;
        Elf32_Word    e_version;
        Elf32_Addr    e_entry;
        Elf32_Off     e_phoff;
        Elf32_Off     e_shoff;
        Elf32_Word    e_flags;
        Elf32_Half    e_ehsize;
        Elf32_Half    e_phentsize;
        Elf32_Half    e_phnum;
        Elf32_Half    e_shentsize;
        Elf32_Half    e_shnum;
        Elf32_Half    e_shstrndx;
} Elf32_Ehdr;

/* Section Header */
typedef struct Elf32_Shdr {
        Elf32_Word sh_name;
        Elf32_Word sh_type;
        Elf32_Word sh_flags;
        Elf32_Addr sh_addr;
        Elf32_Off  sh_offset;
        Elf32_Word sh_size;
        Elf32_Word sh_link;
        Elf32_Word sh_info;
        Elf32_Word sh_addralign;
        Elf32_Word sh_entsize;
} Elf32_Shdr;

/**
 * Elf file structure of a program header.
 * @ingroup		phdr
 */
typedef struct Elf32_Phdr {
        Elf32_Word p_type;		/**< Type of the program header. */
        Elf32_Off  p_offset;	/**< Offset in the file. */
        Elf32_Addr p_vaddr;		/**< Virtual address in host memory. */
        Elf32_Addr p_paddr;		/**< Physical address in host memory. */
        Elf32_Word p_filesz;	/**< Size in the file. */
        Elf32_Word p_memsz;		/**< Size in host memory */
        Elf32_Word p_flags;		/**< Flags: PF_X, PF_W and and PF_R. */
        Elf32_Word p_align;		/**< Alignment in power of two (2**n). */
} Elf32_Phdr;

/* Legal values for note segment descriptor types for core files. */
typedef struct Elf32_Dyn {
  Elf32_Sword d_tag;
  union {
    Elf32_Word d_val;
    Elf32_Addr d_ptr;
  } d_un;
} Elf32_Dyn;


/* ELF32_Sym */
typedef struct Elf32_Sym {
  Elf32_Word	st_name;
  Elf32_Addr	st_value;
  Elf32_Word	st_size;
  unsigned char	st_info;
  unsigned char st_other;
  Elf32_Half	st_shndx;
} Elf32_Sym;

#define ELF32_ST_BIND(i) ((i) >> 4)
#define ELF32_ST_TYPE(i) ((i) & 0xf)
#define ELF32_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

typedef struct Elf32_Rel {
	Elf32_Addr r_offset;
	Elf32_Word r_info;
} Elf32_Rel;

typedef struct Elf32_Rela {
	Elf32_Addr r_offset;
	Elf32_Word r_info;
	Elf32_Sword r_addend;
} Elf32_Rela;

#define ELF32_R_SYM(i) ((i)>>8)
#define ELF32_R_TYPE(i) ((unsigned char)(i))
#define ELF32_R_INFO(s,t) (((s)<<8)+(unsigned char)(t))


// ???
#define DF_ORIGIN		0x00000001
#define DF_SYMBOLIC		0x00000002
#define DF_TEXTREL		0x00000004
#define DF_BIND_NOW		0x00000008
#define DF_STATIC_TLS	0x00000010

} }		// gel::elf

#endif	/* GEL_ELF_DEFS_H */
