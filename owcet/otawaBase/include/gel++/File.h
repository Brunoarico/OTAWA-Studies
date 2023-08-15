/*
 * GEL++ File interface
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

#ifndef GELPP_FILE_H_
#define GELPP_FILE_H_

#include <elm/data/Array.h>
#include <elm/data/HashMap.h>
#include <elm/sys/Path.h>
#include <gel++/base.h>

namespace gel {

using namespace elm;
class Image;
class Manager;
class Parameter;
namespace elf {
	class File;
	class File64;
}

typedef t::uint32 flags_t;
typedef enum {
	IS_EXECUTABLE	= 0x0001,
	IS_WRITABLE		= 0x0002,
	IS_READABLE		= 0x0004,
	HAS_CONTENT		= 0x0008,
	IS_LOADABLE		= 0x0010,
	IS_DEBUG		= 0x0020,
	IS_STRING_TABLE	= 0x0040,
	IS_SYMBOL_TABLE	= 0x0080,
	IS_RELOC		= 0x0100
} flag_values_t;

class Segment {
public:
	static cstring defaultName(Segment *seg);	
	virtual ~Segment();
	virtual cstring name() = 0;
	virtual address_t baseAddress() = 0;
	virtual address_t loadAddress() = 0;
	virtual size_t size() = 0;
	virtual size_t alignment() = 0;
	virtual bool isExecutable() = 0;
	virtual bool isWritable() = 0;
	virtual bool hasContent() = 0;
	virtual Buffer buffer() = 0;
};

class Section: public Segment {
public:
	virtual ~Section();
	virtual size_t offset() = 0;
	virtual size_t fileSize() = 0;
	virtual flags_t flags() = 0;
};

class Symbol {
public:
	enum type_t {
		NO_TYPE = 0,
		OTHER_TYPE = 1,
		FUNC = 2,
		DATA = 3
	};

	enum bind_t {
		NO_BIND = 0,
		OTHER_BIND = 1,
		LOCAL = 2,
		GLOBAL = 3,
		WEAK = 4
	};

	virtual ~Symbol();
	virtual cstring name() = 0;
	virtual t::uint64 value() = 0;
	virtual t::uint64 size() = 0;
	virtual type_t type() = 0;
	virtual bind_t bind() = 0;
};


class SymbolTable: public HashMap<cstring, Symbol *> {
public:
	virtual ~SymbolTable();
};

class DebugLine;

class File {
public:
	typedef enum {
		no_type,
		program,
		library
	} type_t;

	File(Manager& manager, sys::Path path);
	virtual ~File(void);

	inline sys::Path path(void) const { return _path; }
	inline io::IntFormat format(address_t a) { return gel::format(addressType(), a); }
	inline Manager& manager(void) const { return man; }

	virtual elf::File *toELF();
	virtual elf::File64 *toELF64();

	virtual type_t type(void) = 0;
	virtual bool isBigEndian(void) = 0;
	virtual address_type_t addressType(void) = 0;
	virtual address_t entry(void) = 0;
	virtual int count() = 0;
	virtual Segment *segment(int i) = 0;
	virtual Image *make();
	virtual Image *make(const Parameter& params) = 0;

	virtual int countSections();
	virtual Section *section(int i);
	
	virtual const SymbolTable& symbols() = 0;
	virtual DebugLine *debugLines();
	virtual string machine() const;
	virtual string os() const;
	virtual int elfMachine() const;
	virtual int elfOS() const;
	
protected:
	Manager& man;
private:
	sys::Path _path;
};

io::Output& operator<<(io::Output& out, File::type_t t);

} // gel

#endif /* GELPP_FILE_H_ */
