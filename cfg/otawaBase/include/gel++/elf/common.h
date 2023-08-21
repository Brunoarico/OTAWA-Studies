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
#ifndef GEL___ELF_COMMON_H_
#define GEL___ELF_COMMON_H_

namespace gel { namespace elf {

// ELF identification, e_ident
#define	EI_MAG0			0
#define EI_MAG1			1
#define EI_MAG2			2
#define EI_MAG3			3
#define EI_CLASS		4
#define EI_DATA			5
#define EI_VERSION		6
#define EI_OSABI		7
#define EI_ABIVERSION	8
#define EI_PAD			9
#define EI_NIDENT		16

// ELF magic number
#define	ELFMAG0 0x7f
#define ELFMAG1 'E'
#define	ELFMAG2 'L'
#define ELFMAG3 'F'

// Object File Classes, e_ident[EI_CLASS]
#define	ELFCLASSNONE	0
#define	ELFCLASS32		1
#define	ELFCLASS64		2

// Data Encodings, e_ident[EI_DATA]
#define ELFDATANONE	0
#define ELFDATA2LSB	1
#define ELFDATA2MSB 2

// Operating System and ABI Identifiers, e_ident[EI_OSABI]
#define ELFOSABI_SYSV		0
#define ELFOSABI_HPUX		1
#define ELFOSABI_STANDALONE	255

//Object File Types, e_type
#define ET_NONE		0
#define ET_REL 		1
#define ET_EXEC 	2
#define ET_DYN 		3
#define ET_CORE 	4
#define ET_LOOS		0xFE00
#define ET_HIOS		0xFEFF
#define ET_LOPROC	0xFF00
#define ET_HIPROC	0xFFFF

// ELF versions, e_ident[EI_VERSION]
#define EV_NONE		0
#define EV_CURRENT	1

// Special Section Indices
#define SHN_UNDEF	0
#define	SHN_LOPROC	0xFF00
#define SHN_HIPROC	0xFF1F
#define SHN_LOOS	0xFF20
#define SHN_HIOS	0xFF3F
#define SHN_ABS		0xFFF1
#define SHN_COMMON	0xFFF2

// Section Types, sh_type
#define SHT_NULL		0
#define SHT_PROGBITS	1
#define SHT_SYMTAB		2
#define SHT_STRTAB		3
#define SHT_RELA		4
#define SHT_HASH		5
#define SHT_DYNAMIC		6
#define SHT_NOTE		7
#define SHT_NOBITS		8
#define SHT_REL			9
#define SHT_SHLIB		10
#define SHT_DYNSYM		11
#define SHT_LOOS		0x60000000
#define SHT_HIOS		0x6FFFFFFF
#define LOPROC			0x70000000
#define HIPROC			0x7FFFFFFF

// Section Attributes, sh_flags
#define SHF_WRITE		0x1
#define SHF_ALLOC		0x2
#define SHF_EXECINSTR	0x4
#define SHF_MASKOS		0x0F000000
#define SHF_MASKPROC	0xF0000000

// Symbol Bindings
#define STB_LOCAL	0
#define STB_GLOBAL	1
#define STB_WEAK	2
#define STB_LOOS	10
#define STB_HIOS	12
#define STB_LOPROC	13
#define STB_HIPROC	15

// Symbol Types
#define STT_NOTYPE	0
#define STT_OBJECT	1
#define STT_FUNC	2
#define STT_SECTION	3
#define STT_FILE	4
#define STT_LOOS	10
#define STT_HIOS	12
#define STT_LOPROC	13
#define STT_HIPROC	15

// Segment Types, p_type
#define PT_NULL		0
#define PT_LOAD		1
#define PT_DYNAMIC	2
#define PT_INTERP	3
#define PT_NOTE		4
#define PT_SHLIB	5
#define PT_PHDR	6
#define PT_LOOS		0x60000000
#define PT_HIOS		0x6FFFFFFF
#define PT_LOPROC	0x70000000
#define PT_HIPROC	0x7FFFFFFF

// Segment Attributes, p_flags
#define PF_X		0x1
#define PF_W		0x2
#define PF_R		0x4
#define PF_MASKOS	0x00FF0000
#define PF_MASKPROC	0xFF000000

// Dynamic Table Entries
#define DT_NULL		 		0
#define DT_NEEDED 	 		1	/* d_val */
#define DT_PLTRELSZ	 		2	/* d_val */
#define DT_PLTGOT	 		3	/* d_ptr */
#define DT_HASH		 		4	/* d_ptr */
#define DT_STRTAB	 		5	/* d_ptr */
#define DT_SYMTAB	 		6	/* d_ptr */
#define DT_RELA		 		7	/* d_ptr */
#define DT_RELASZ	 		8	/* d_val */
#define DT_RELAENT	 		9	/* d_val */
#define DT_STRSZ			10	/* d_val */
#define DT_SYMENT			11	/* d_val */
#define DT_INIT				12	/* d_ptr */
#define DT_FINI				13	/* d_ptr */
#define DT_SONAME			14	/* d_val */
#define DT_RPATH			15	/* d_val */
#define DT_SYMBOLIC			16
#define DT_REL				17	/* d_ptr */
#define DT_RELSZ			18	/* d_val */
#define DT_RELENT			19	/* d_val */
#define DT_PLTREL			20	/* d_val */
#define DT_DEBUG			21	/* d_ptr */
#define DT_TEXTREL			22
#define DT_JMPREL			23	/* d_ptr */
#define DT_BIND_NOW			24
#define DT_INIT_ARRAY		25	/* d_ptr */
#define DT_FINI_ARRAY		26	/* d_ptr */
#define DT_INIT_ARRAYSZ		27	/* d_val */
#define DT_FINI_ARRAYSZ		28	/* d_val */

// no more in documentation?
#define DT_RUNPATH			29	/* d_val */
#define DT_FLAGS			30	/* d_val */
#define DT_ENCODING			32
#define DT_PREINIT_ARRAY	32	/* d_ptr */
#define DT_PREINIT_ARRAYSZ	33	/* d_val */
#define DT_SYMTAB_SHNDX		34	/* d_ptr */
#define DT_COUNT			35
// end no more

#define DT_LOOS		0x60000000
#define DT_HIOS		0x6fffffff
#define DT_LOPROC	0x70000000
#define DT_HIPROC	0x7fffffff

// useful macros
#define SWAP2(x) ((((x) & 0xFF) << 8) | (((x) & 0xFF00 ) >> 8))
#define SWAP4(x) ((((SWAP2((x) & 0xFFFF))) << 16) | SWAP2(((x) & 0xFFFF0000) >> 16))
#define SWAP8(x) (((SWAP4((x) & 0xFFFFFFFF)) << 32) | SWAP4(((x) >> 32) & 0xFFFFFFFF))

#ifdef BIGENDIAN
#define ENDIAN2(e,v) (((e) == ELFDATA2MSB) ? (v) : SWAP2(v))
#define ENDIAN4(e,v) (((e) == ELFDATA2MSB) ? (v) : SWAP4(v))
#define ENDIAN8(e,v) (((e) == ELFDATA2MSB) ? (v) : SWAP8(v))
#else
#define ENDIAN2(e,v) (((e) == ELFDATA2LSB) ? (v) : SWAP2(v))
#define ENDIAN4(e,v) (((e) == ELFDATA2LSB) ? (v) : SWAP4(v))
#define ENDIAN8(e,v) (((e) == ELFDATA2LSB) ? (v) : SWAP8(v))
#endif
#define UN_ENDIAN2(e, v)	ENDIAN2(e, v)
#define UN_ENDIAN4(e, v)	ENDIAN4(e, v)
#define UN_ENDIAN8(e, v)	ENDIAN8(e, v)

} }	// gel::elf

#endif /* GEL___ELF_COMMON_H_ */
