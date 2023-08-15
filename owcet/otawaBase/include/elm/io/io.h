/*
 * $Id$
 * Copyright (c) 2004, Alfheim Corporation.
 *
 * io.h -- formatted IO classes interfaces.
 */
#ifndef ELM_IO_IO_H
#define ELM_IO_IO_H

#include <elm/sys/SystemIO.h>
#include <elm/io/IOException.h>
#include <elm/io/Output.h>
#include <elm/io/Input.h>
#include <elm/io/FileInput.h>
#include <elm/io/StringInput.h>
#include <elm/io/FileOutput.h>

namespace elm {

// Standard environment
extern io::Output cout;
extern io::Output cerr;
extern io::Input cin;

namespace io {
	inline FileInput read(sys::Path path) { return FileInput(path); }
	inline StringInput read(const char *s) { return StringInput(s); }
	inline StringInput read(const cstring& s) { return StringInput(s); }
	inline StringInput read(const string& s) { return StringInput(s); }
	inline FileOutput write(sys::Path path) { return FileOutput(path); }
	inline FileOutput append(sys::Path path) { return FileOutput(path, true); }
} // io

} // elm

#endif	// ELM_IO_IO_H
