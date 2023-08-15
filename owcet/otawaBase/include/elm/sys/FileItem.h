/*
 *	FileItem class interface
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
#ifndef ELM_SYS_FILE_ITEM_H
#define ELM_SYS_FILE_ITEM_H

#include <sys/stat.h>	// TODO find a way to remove
#include <elm/util/LockPtr.h>
#include <elm/sys/Path.h>
#include <elm/sys/SystemException.h>

namespace elm { namespace sys {

// External
class File;
class Directory;

// FileItem class
class FileItem: public Lock {
	friend class LockPtr<FileItem>;
public:
	static LockPtr<FileItem> get(Path path);
	
	virtual LockPtr<File> toFile();
	virtual LockPtr<Directory> toDirectory();
	String name();
	Path& path();
	bool isReadable();
	bool isWritable();
	bool isDeletable();

protected:
	Directory *parent;
	Path _path;
	ino_t ino;
	FileItem(Path path, ino_t inode);
	virtual ~FileItem();
};

} } // elm::system

#endif // ELM_SYS_FILE_ITEM_H
