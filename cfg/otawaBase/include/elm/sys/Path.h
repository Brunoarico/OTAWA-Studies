/*
 *	Path class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2005-8, IRIT UPS.
 *
 *	OTAWA is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	OTAWA is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with OTAWA; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef ELM_SYS_PATH_H
#define ELM_SYS_PATH_H

#include <elm/io/InStream.h>
#include <elm/io/Output.h>
#include <elm/PreIterator.h>
#include <elm/string.h>
#include <elm/sys/SystemException.h>

namespace elm { namespace sys {

// Path class
class Path {
public:
#	if defined(__WIN32) || defined(__WIN64)
		static const char SEPARATOR = '\\';
		static const char PATH_SEPARATOR = ';';
		static inline bool isSeparator(char c) { return c == SEPARATOR || c == '/'; }
#	else
		static const char SEPARATOR = '/';
		static const char PATH_SEPARATOR = ':';
		static inline bool isSeparator(char c) { return c == SEPARATOR; }
#	endif
		static const string BACK_PATH;

	// Constructors
	inline Path(void) { }
	inline Path(const char *path): buf(path) { }
	inline Path(CString path): buf(path) { }
	inline Path(const String& path): buf(path) { }
	inline Path(const Path& path): buf(path.buf) { }
	Path canonical(void) const;
	Path absolute(void) const;
	static void setCurrent(Path& path);
	Path append(Path path) const;
	Path parent(void) const;
	Path setExtension(CString new_extension) const;
	inline Path setExt(cstring ext) const { return setExtension(ext); }
	Path relativeTo(Path base) const;
	Path withoutExt(void) const;

	// Accessors
	inline const String& toString(void) const { return buf; }
	String namePart(void) const;
	sys::Path dirPart(void) const;
	Path basePart(void) const;
	String extension(void) const;
	bool isEmpty(void) const { return buf.isEmpty(); }
	bool isAbsolute(void) const;
	bool isRelative(void) const;
	bool isHomeRelative(void) const;
	inline bool equals(Path& path) const { return buf == path.buf; }
	inline bool subPathOf(const Path& path) const { return buf.startsWith(path.buf); }
	inline bool isPrefixOf(const Path& path) const
		{ return path.buf.startsWith(buf)  && (path.buf.length() == buf.length() || path.buf[buf.length()] == SEPARATOR); }
	inline bool prefixedBy(const Path& path) const { return path.isPrefixOf(*this); }
	static Path current(void);
	static Path home(void);
	static Path temp(void);
	inline const char *asSysString() const { return buf.toCString().chars(); }

	// path testing
	bool exists(void) const;
	bool isFile(void) const;
	bool isDir(void) const;
	bool isReadable(void) const;
	bool isWritable(void) const;
	bool isExecutable(void) const;

	// file handling
	void remove(void);
	void makeDir(void);
	io::InStream *read(void);
	io::OutStream *write(void);
	io::OutStream *append(void);

	// Operator
	inline Path& operator=(const char *str) { buf = str; return *this; }
	inline Path& operator=(CString str) { buf = str; return *this; }
	inline Path& operator=(const String& str) { buf = str; return *this; }
	inline Path& operator=(const Path& path) { buf = path.buf; return *this; }

	inline bool operator==(Path path) const { return equals(path); }
	inline bool operator!=(Path path) const { return !equals(path); }
	inline Path operator/(const Path& path) const { return append(path); }
	inline operator const String& (void) const { return toString(); }
	inline operator bool (void) const  { return buf; }

	// Path iterator
	class PathIter: public PreIterator<PathIter, string> {
		friend class Path;
	public:
		inline PathIter(void): p(0), n(-1) { }
		inline PathIter(string paths): s(paths), p(0), n(-1) { look(); }
		inline bool ended(void) const { return p >= s.length(); }
		inline Path item(void) const { return s.substring(p, n - p); }
		inline void next(void) { p = n; if(p < s.length()) { p++; look(); } }
		inline bool equals(const PathIter& i) const { return s == i.s && p == i.p; }
	private:
		inline PathIter(string paths, int p): s(paths), p(p), n(-1) { look(); }
		void look(void);
		string s;
		int p, n;
	};

	class PathSplit {
	public:
		inline PathSplit(string p): _p(p) { }
		inline PathIter begin(void) const { return PathIter(_p); }
		inline PathIter end(void) const { return PathIter(_p, _p.length()); }
	private:
		string _p;
	};
	inline static PathSplit splitPaths(string paths) { return PathSplit(paths); }

	// directory reading
	class DirIter {
	public:
		typedef cstring t;

		inline DirIter(void): _dir(nullptr) { }
		DirIter(Path path);

		inline bool ended(void) const { return _dir == nullptr; }
		inline cstring item(void) const { return _cur; }
		void next(void);
		inline bool equals(const DirIter& i) const { return _dir == i._dir && _cur == i._cur; }

		inline operator bool() const { return !ended(); }
		inline operator cstring() const { return item(); }
		inline cstring operator*() const { return item(); }
		inline DirIter& operator++() { next(); return *this; }
		inline DirIter operator++(int) { DirIter o = *this; next(); return o; }
		inline bool operator==(const DirIter& i) const { return equals(i); }
		inline bool operator!=(const DirIter& i) const { return !equals(i); }

	private:
		void *_dir;
		cstring _cur;
	};

	class DirReader {
	public:
		inline DirReader(const Path& p): _p(p) { }
		inline DirIter begin(void) { return DirIter(_p); }
		inline DirIter end(void) { return DirIter(); }
	private:
		const Path& _p;
	};

	inline DirReader readDir(void) const { return DirReader(*this); }
	void makeDirs(void) const;

	// deprecated
	inline bool contains(const Path& path) const { return path.subPathOf(*this); }

private:
	int nextSeparator(int start = 0) const;
	int lastSeparator(void) const;
	String buf;
};

inline io::Output& operator<<(io::Output& out, const Path& path)
	{ out << path.toString(); return out; }

}	// system

typedef elm::sys::Path Path;

} // elm

#endif // ELM_SYS_PATH_H
