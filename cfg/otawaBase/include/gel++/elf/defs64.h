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
#ifndef GEL_ELF_DEFS64_H
#define GEL_ELF_DEFS64_H

#include <elm/types.h>
#include <config.h>
#include "common.h"

namespace gel { namespace elf {

using namespace elm;

typedef t::uint64	Elf64_Addr;
typedef t::uint64	Elf64_Off;
typedef t::uint16	Elf64_Half;
typedef t::uint32	Elf64_Word;
typedef t::int32	Elf64_Sword;
typedef t::uint64	Elf64_Xword;
typedef t::int64	Elf64_Sxword;

typedef struct Elf64_Ehdr {
        unsigned char e_ident[EI_NIDENT];
        Elf64_Half    e_type;
        Elf64_Half    e_machine;
        Elf64_Word    e_version;
        Elf64_Addr    e_entry;
        Elf64_Off     e_phoff;
        Elf64_Off     e_shoff;
        Elf64_Word    e_flags;
        Elf64_Half    e_ehsize;
        Elf64_Half    e_phentsize;
        Elf64_Half    e_phnum;
        Elf64_Half    e_shentsize;
        Elf64_Half    e_shnum;
        Elf64_Half    e_shstrndx;
} Elf64_Ehdr;

// Section Header entries
typedef struct Elf64_Shdr {
	Elf64_Word 	sh_name;
	Elf64_Word 	sh_type;
	Elf64_Xword	sh_flags;
	Elf64_Addr 	sh_addr;
	Elf64_Off	sh_offset;
	Elf64_Xword	sh_size;
	Elf64_Word	sh_link;
	Elf64_Word	sh_info;
	Elf64_Xword	sh_addralign;
	Elf64_Xword	sh_entsize;
} Elf64_Shdr;

// ELF-64 Symbol Table Entry
typedef struct Elf64_Sym {
	Elf64_Word	st_name;
	unsigned char	st_info;
	unsigned char st_other;
	Elf64_Half	st_shndx;
	Elf64_Addr	st_value;
	Elf64_Xword	st_size;
} Elf64_Sym;

#define ELF64_ST_BIND(i) ((i) >> 4)
#define ELF64_ST_TYPE(i) ((i) & 0xf)
#define ELF64_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

// ELF-64 Relocation Entries
typedef struct Elf64_Rel {
	Elf64_Addr	r_offset;
	Elf64_Xword	r_info;
} Elf64_Rel;

typedef struct Elf64_Rela {
	Elf64_Addr		r_offset;
	Elf64_Xword		r_info;
	Elf64_Sxword	r_addend;
} Elf64_Rela;

#define ELF64_R_SYM(i)		((i) >> 32)
#define ELF64_R_TYPE(i)		((i) & 0xffffffffL)
#define ELF64_R_INFO(s, t)	(((s) << 32) + ((r) & 0xffffffffL))

// ELF-64 Program Header Table Entry
typedef struct Elf64_Phdr {
	Elf64_Word	p_type;
	Elf64_Word	p_flags;
	Elf64_Off	p_offset;
	Elf64_Addr	p_vaddr;
	Elf64_Addr	p_paddr;
	Elf64_Xword p_filesz;
	Elf64_Xword p_memsz;
	Elf64_Xword p_align;
} Elf64_Phdr;

// Dynamic Table Structure
typedef struct Elf64_Dyn {
  Elf64_Sxword d_tag;
  union {
    Elf64_Xword d_val;
    Elf64_Addr d_ptr;
  } d_un;
} Elf64_Dyn;

} }		// gel::elf

#endif	/* GEL_ELF_DEFS64_H */
