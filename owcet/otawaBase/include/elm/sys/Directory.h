/*
 *	Directory class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2005-12, IRIT UPS.
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
#ifndef ELM_SYS_DIRECTORY_H
#define ELM_SYS_DIRECTORY_H

#include <elm/PreIterator.h>
#include <elm/sys/FileItem.h>

namespace elm { namespace sys {

// File class
class Directory: public FileItem {
	friend class FileItem;
	Directory(Path path, ino_t inode);
public:

	static LockPtr<Directory> make(Path path);

	// Children iterator
	class Iter {
	public:
		typedef LockPtr<FileItem> t;

		Iter(LockPtr<Directory> directory);
		~Iter(void);
		bool ended(void) const;
		t item(void) const;
		void next(void);
		bool equals(const Iter& i) const;

		inline operator bool() const { return !ended(); }
		inline operator t() const { return item(); }
		inline t operator*() const { return item(); }
		inline t operator->() const { return item(); }
		inline Iter& operator++() { next(); return *this; }
		inline void operator++(int) { next(); }
		inline bool operator==(const Iter& i) const { return equals(i); }
		inline bool operator!=(const Iter& i) const { return !equals(i); }

	private:
		Path path;
		void *dir;
		LockPtr<FileItem> file;
		void go(void);
	};

	LockPtr<Directory> toDirectory(void) override;
};

} } // elm::system

#endif // ELM_SYS_DIRECTORY_H

