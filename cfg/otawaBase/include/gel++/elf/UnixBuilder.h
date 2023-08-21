/*
 * GEL++ UnixBuilder class interface
 * Copyright (c) 2016, IRIT- University of Toulouse
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
#ifndef GEL___ELF_UNIXBUILDER_H_
#define GEL___ELF_UNIXBUILDER_H_

#include <elm/data/HashMap.h>
#include <elm/data/VectorQueue.h>
#include <gel++/Image.h>
#include <gel++/elf/File.h>

namespace gel { namespace elf {

class UnixBuilder;

class Unit {
	friend class UnixBuilder;
public:
	static const t::uint32
		SYMBOLIC	= 0x01,
		TEXTREL		= 0x02,
		BIND_NOW	= 0x04;

	Unit(sys::Path name);
	Unit(File *file);
	inline File *file(void) const { return _file; }
	t::uint32 base(void) const { return _base; }
	inline cstring name(void) const { return _soname; }
	inline sys::Path origin() const { return _name.parent(); }

	t::uint32 pltrelsz = 0;
	t::uint32 pltgot = 0;
	t::uint32 hash = 0;
	t::uint32 strtab = 0;
	t::uint32 symtab = 0;
	t::uint32 rela = 0;
	t::uint32 relasz = 0;
	t::uint32 relaent = 0;
	t::uint32 strsz = 0;
	t::uint32 syment = 0;
	t::uint32 init = 0;
	t::uint32 fini = 0;
	t::uint32 rel = 0;
	t::uint32 relsz = 0;
	t::uint32 relent = 0;
	t::uint32 pltrel = 0;
	t::uint32 jmprel = 0;
	t::uint32 flags = 0;
	t::uint32 debug = 0;

protected:
	cstring getString(ImageSegment *s, t::uint32 off);
private:
	t::uint32 load(UnixBuilder& builder, t::uint32 base);
	void link(UnixBuilder& builder);

	sys::Path _name;
	cstring _soname;
	File *_file;
	t::uint32 _base;
	Vector<Unit *> _needed;
	HashMap<cstring, Elf32_Sym *> _map;
	ProgramHeader *_dyn;
	Vector<string> _rpath;
};


class Auxiliary {
public:
	inline Auxiliary(t::uint32 t = 0, t::uint32 v = 0): type(t), val(v) { }
	t::uint32 type;
	t::uint32 val;
};

class UnixParameter: public Parameter {
public:
	UnixParameter(void);
	static UnixParameter null;
	size_t page_size;
	Array<Auxiliary> auxv;
	Array<sys::Path> lib_paths;
	sys::Path sys_root;
	bool is_linux = true;
	bool no_default_path = false;
};

class UnixBuilder: public ImageBuilder {
	friend class Unit;
public:
	UnixBuilder(File *file, const Parameter& param = UnixParameter::null);
	Image *build(void) override;
protected:
	gel::File *retrieve(sys::Path name) override;
private:
	Unit *resolve(cstring name, Unit *unit);
	Unit *get(sys::Path p);
	ImageSegment *buildStack(void);
	void link(File *file, address_t base);
	File *open(sys::Path path);
	string expand(string s, Unit *u);

	elf::File *_prog;
	const UnixParameter *_uparams;
	HashMap<sys::Path, Unit *> _units;
	Image *_im;
	Vector<Unit *> todo;
	Vector<sys::Path> lpaths;
};

} }		// gel::elf

#endif /* GEL___ELF_UNIXBUILDER_H_ */
