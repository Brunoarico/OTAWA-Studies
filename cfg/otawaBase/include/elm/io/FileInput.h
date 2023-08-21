/*
 *	FileInput class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2012, IRIT UPS.
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
#ifndef ELM_IO_FILEINPUT_H_
#define ELM_IO_FILEINPUT_H_

#include <elm/io/Input.h>
#include <elm/sys/Path.h>

namespace elm { namespace io {

class FileInput: public Input {
public:
	FileInput(const char *path);
	FileInput(cstring path);
	FileInput(string path);
	FileInput(sys::Path path);
	FileInput(FileInput&& fi);
	~FileInput(void);
private:
	InStream *_in;
};

} }		// elm::io

#endif /* ELM_IO_FILEINPUT_H_ */
