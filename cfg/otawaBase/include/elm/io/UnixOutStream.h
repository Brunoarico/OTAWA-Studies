/*
 * $Id$
 * Copyright (c) 2004-06, IRIT - UPS.
 *
 * elm/io/OutStream.h -- interface for OutStream class.
 */
#ifndef ELM_IO_UNIX_OUTSTREAM_H
#define ELM_IO_UNIX_OUTSTREAM_H

#include <elm/string/CString.h>
#include <elm/io/OutStream.h>

namespace elm { namespace io {

// UnixOutStream class
class UnixOutStream: public OutStream {
public:
	UnixOutStream(int fd);
	~UnixOutStream();
	inline int fd(void) const { return _fd; };
	int write(const char *buffer, int size) override;
	int flush() override;
	CString lastErrorMessage() override;
	bool supportsANSI() const override;
protected:
	int _fd;
};

} } // elm::io

#endif	// ELM_IO_UNIX_OUTSTREAM_H
