/*
 * GEL++ main header
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
#ifndef GEL___H_
#define GEL___H_

#include <elm/sys/Path.h>
#include <elm/util/ErrorHandler.h>
#include <elm/io/RandomAccessStream.h>
#include <gel++/Exception.h>
#include <gel++/File.h>
#include <gel++/Image.h>

namespace gel {

using namespace elm;

class File;
namespace elf { class File; }
namespace pecoff { class File; }

class Manager: public ErrorBase {
public:
	inline static File *open(sys::Path path) { return DEFAULT.openFile(path); }
	inline static elf::File *openELF(sys::Path path) { return DEFAULT.openELFFile(path); }

	static Manager DEFAULT;
	File *openFile(sys::Path path);
	elf::File *openELFFile(sys::Path path);
	elf::File *openELFFile(sys::Path path, io::RandomAccessStream *stream);
	pecoff::File *openPECOFFFile(sys::Path path, io::RandomAccessStream *stream);
	
};

}	// gel

#endif /* GEL___H_ */
