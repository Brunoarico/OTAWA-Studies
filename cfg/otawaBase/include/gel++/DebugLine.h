/*
 * GEL++ DebugLine class interface
 * Copyright (c) 2020, IRIT- université de Toulouse
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
#ifndef GELPP_DEBUG_LINE_H
#define GELPP_DEBUG_LINE_H

#include <elm/data/List.h>
#include <elm/data/FragTable.h>
#include <elm/data/HashMap.h>
#include <gel++.h>

namespace gel {

using namespace elm;

class DebugLine {
public:

	class CompilationUnit;
	class LineNumber;

	class File {
		friend class CompilationUnit;
		friend class DebugLine;
	public:
		inline File(): _date(0), _size(0) { }
		inline File(sys::Path path, t::uint64 date = 0, size_t size = 0):
			_path(path), _date(date), _size(size) { }

		inline const sys::Path& path() const { return _path; }
		inline t::uint64 date() const { return _date; }
		inline size_t size() const { return _size; }
		inline const List<CompilationUnit *>& units() const { return _units; }
		void find(int line, Vector<Pair<address_t, address_t> >& addrs) const;

	private:
		sys::Path _path;
		t::uint64 _date;
		size_t _size;
		List<CompilationUnit *> _units;
	};

	class LineNumber {
	public:
		static const t::uint32
			IS_STMT			= 1 << 0,
			BASIC_BLOCK		= 1 << 1,
			PROLOGUE_END	= 1 << 2,
			EPILOGUE_BEGIN	= 1 << 3;

		inline LineNumber()
			: _file(nullptr), _line(0), _col(0), _flags(0), _addr(0), _isa(0),
			  _disc(0), _opi(0) { }

		LineNumber(address_t addr, File *file, int line, int col = 0,
		t::uint32 flags = 0, t::uint8 isa = 0, t::uint8 desc = 0, t::uint8 opi = 0);

		inline File *file() const { return _file; }
		inline int line() const { return _line; }
		inline int col() const { return _col; }
		inline t::uint32 flags() const { return _flags; }
		inline address_t addr() const { return _addr; }
		inline t::uint8 isa() const { return _isa; }
		inline t::uint8 discriminator() const { return _disc; }
		inline t::uint8 op_index() const { return _opi; }

	private:
		File *_file;
		int _line, _col;
		t::uint32 _flags;
		address_t _addr;
		t::uint8 _isa, _disc, _opi;
	};

	class CompilationUnit {
		friend class DebugLine;
	public:
		const FragTable<LineNumber>& lines() const { return _lines; }
		const Vector<File *>& files() const { return _files; }
		void add(const LineNumber& num);
		void add(File *file);
		address_t baseAddress() const;
		address_t topAddress() const;
		inline size_t size() const { return topAddress() - baseAddress(); }
		const LineNumber *lineAt(address_t addr) const;
	private:
		Vector<File *> _files;
		FragTable<LineNumber> _lines;
	};

	DebugLine(gel::File *file);

	inline const HashMap<sys::Path, File *>& files() const { return _files; }
	inline const FragTable<CompilationUnit *>& units() const { return _cus; }
	inline gel::File& program() const { return prog; }
	const LineNumber *lineAt(address_t addr) const;
	
protected:
	virtual ~DebugLine();
	void add(CompilationUnit *cu);
	void add(File *file);
	gel::File& prog;
private:
	FragTable<CompilationUnit *> _cus;
	HashMap<sys::Path, File *> _files;
};

}	// gel

#endif	// GELPP_DEBUG_LINE_H



