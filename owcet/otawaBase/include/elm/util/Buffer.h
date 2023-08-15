/*
 *	Buffer class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2018, IRIT UPS.
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
#ifndef ELM_UTIL_BUFFER_H_
#define ELM_UTIL_BUFFER_H_

#include <elm/array.h>
#include <elm/types.h>
#include <elm/util/Exception.h>

namespace elm {

class BufferException: public Exception { };

class Buffer {
public:
	static const t::size default_size = 4096;

	inline Buffer(t::size size = default_size):
		_size(size), _buf(new char[size]), _free(true) { }
	inline Buffer(char *buf, t::size size)
		: _size(size), _buf(buf), _free(false) { }
	inline ~Buffer() { if(_free) delete [] _buf; }

	inline t::size size() const { return _size; }

	inline char operator[](t::size i) const
		{ check(i); return _buf[i]; }
	inline char& operator[](t::size i)
		{ check(i); return _buf[i]; }

	inline void copyTo(void *p, t::size size, t::size offset = 0) const
		{ check(offset + size - 1); array::copy(static_cast<char *>(p), _buf + offset, _size);  }
	inline void copyFrom(void *p, t::size size, t::size offset = 0)
		{ check(offset + size - 1); array::copy(_buf + offset, static_cast<char *>(p), _size);  }

	template <class F>
	inline void read(F f, t::size size, t::size offset = 0) const
		{ check(offset + size - 1); f(static_cast<const char *>(_buf + offset), size); }
	template <class F>
	inline void write(F f, t::size size, t::size offset = 0)
		{ check(offset + size - 1); f(_buf + offset, size); }

private:
	inline void check(t::size i) const { if(i > _size) throw BufferException(); }

	t::size _size;
	char *_buf;
	bool _free;
};

}	// elm

#endif /* ELM_UTIL_BUFFER_H_ */
