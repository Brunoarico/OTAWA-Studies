/*
 * GEL++ base definitions
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
#ifndef INCLUDE_GEL___BASE_H_
#define INCLUDE_GEL___BASE_H_

#include <elm/array.h>
#include <elm/types.h>
#include "Exception.h"

namespace gel {

using namespace elm;

typedef t::uint64 address_t;
typedef t::uint64 size_t;
typedef t::uint64 offset_t;

typedef struct range_t {
	inline range_t(address_t a): _addr(a), _size(1) { }
	inline range_t(address_t a, size_t s): _addr(a), _size(s) { }

	inline address_t base(void) const { return _addr; }
	inline size_t size(void) const { return _size; }
	inline address_t top(void) const { return _addr + _size; }
	inline address_t last(void) const { return top() - 1; }
	inline bool contains(address_t a) const { return _addr <= a && a <= last(); }
	inline bool operator&(address_t a) const { return contains(a); }

	inline range_t operator+(offset_t off) const { return range_t(_addr + off, _size); }
	inline range_t operator-(offset_t off) const { return range_t(_addr - off, _size); }
	inline range_t operator+(int off) const { return range_t(_addr + off, _size); }
	inline range_t operator-(int off) const { return range_t(_addr - off, _size); }

	address_t _addr;
	size_t _size;
} range_t;

typedef enum {
	address_8,
	address_16,
	address_32,
	address_64
} address_type_t;

io::IntFormat format(address_type_t t, address_t a);
inline io::Output& operator<<(io::Output& out, const range_t& r)
	{ out << format(address_64, r.base()) << ':' << format(address_64, r.size()); return out; }

class Decoder {
public:
	virtual ~Decoder(void);
	virtual void fix(t::uint16& w) = 0;
	virtual void fix(t::int16& w) = 0;
	virtual void fix(t::uint32& w) = 0;
	virtual void fix(t::int32& w) = 0;
	virtual void fix(t::uint64& w) = 0;
	virtual void fix(t::int64& w) = 0;

	virtual void unfix(t::uint16& w) = 0;
	virtual void unfix(t::int16& w) = 0;
	virtual void unfix(t::uint32& w) = 0;
	virtual void unfix(t::int32& w) = 0;
	virtual void unfix(t::uint64& w) = 0;
	virtual void unfix(t::int64& w) = 0;
};

class Buffer {
public:
	inline Buffer(void): d(0), b(0), sz(0) { }
	inline Buffer(Decoder *decoder, t::uint8 *buffer, size_t size)
		: d(decoder), b(buffer), sz(size) { }
	inline Buffer(Decoder *decoder, const void *buffer, size_t size)
		: d(decoder), b((t::uint8 *)buffer), sz(size) { }

	inline const t::uint8 *bytes(void) const { return b; }
	inline t::uint8 *bytes(void) { return b; }
	inline size_t size(void) const { return sz; }
	static Buffer null;
	inline bool isNull(void) const { return !b; }
	inline bool equals(const Buffer& buf) const { return b == buf.b && sz == buf.sz; }
	inline Decoder *decoder(void) const { return d; }

	inline const t::uint8 *at(offset_t offset) const
		{ ASSERT(offset < sz); return b + offset; }
	inline void get(offset_t off, t::uint8& r) const
		{ ASSERT(off + sizeof(t::uint8) <= sz); r = *(t::uint8 *)(b + off); }
	inline void get(offset_t off, t::int8& r) const
		{ ASSERT(off + sizeof(t::int8) <= sz); r = *(t::int8 *)(b + off); }
	inline void get(offset_t off, t::uint16& r) const
		{ ASSERT(off + sizeof(t::uint16) <= sz); d->fix(r = *(t::uint16 *)(b + off)); }
	inline void get(offset_t off, t::int16& r) const
		{ ASSERT(off + sizeof(t::int16) <= sz); d->fix(r = *(t::int16 *)(b + off)); }
	inline void get(offset_t off, t::uint32& r) const
		{ ASSERT(off + sizeof(t::uint32) <= sz); d->fix(r = *(t::uint32 *)(b + off)); }
	inline void get(offset_t off, t::int32& r) const
		{ ASSERT(off + sizeof(t::int32) <= sz); d->fix(r = *(t::int32 *)(b + off)); }
	inline void get(offset_t off, t::uint64& r) const
		{ ASSERT(off + sizeof(t::uint64) <= sz); d->fix(r = *(t::uint64 *)(b + off)); }
	inline void get(offset_t off, t::int64& r) const
		{ ASSERT(off + sizeof(t::int64) <= sz); d->fix(r = *(t::int64 *)(b + off)); }
	inline void get(offset_t off, cstring& s)
		{ ASSERT(off < sz); s = cstring((const char *)(b + off)); }
	inline void get(offset_t off, string& s)
		{ ASSERT(off < sz); s = string((const char *)(b + off)); }

	inline void set(offset_t off, t::uint8 v)
		{ ASSERT(off + sizeof(t::uint8) <= sz); *static_cast<t::uint8 *>(b + off) = v; }
	inline void set(offset_t off, t::int8 v)
		{ ASSERT(off + sizeof(t::int8) <= sz); *(t::int8 *)(b + off) = v; }
	inline void set(offset_t off, t::uint16 v)
		{ ASSERT(off + sizeof(t::uint16) <= sz); *(t::uint16 *)(b + off) = v; }
	inline void set(offset_t off, t::int16 v)
		{ ASSERT(off + sizeof(t::int16) <= sz); *(t::int16 *)(b + off) = v; }
	inline void set(offset_t off, t::uint32 v)
		{ ASSERT(off + sizeof(t::uint32) <= sz); *(t::uint32 *)(b + off) = v; }
	inline void set(offset_t off, t::int32 v)
		{ ASSERT(off + sizeof(t::int32) <= sz); *(t::int32 *)(b + off) = v; }
	inline void set(offset_t off, t::uint64 v)
		{ ASSERT(off + sizeof(t::uint64) <= sz); *(t::uint64 *)(b + off) = v; }
	inline void set(offset_t off, t::int64 v)
		{ ASSERT(off + sizeof(t::int64) <= sz); *(t::int64 *)(b + off) = v; }
	inline void set(offset_t off, cstring s)
		{ size_t ss = s.length() + 1; ASSERT(off + ss <= sz); array::copy(b + off, (const t::uint8 *)(s.chars()), ss); }
	inline void set(offset_t off, string s)
		{ size_t ss = s.length() + 1; ASSERT(off + ss <= sz); array::copy(b + off, (const t::uint8 *)(s.chars()), ss - 1); *(b + off + ss - 1) = '\0'; }

	inline operator bool(void) const { return !isNull(); }
	inline bool operator==(const Buffer& b) const { return equals(b); }
	inline bool operator!=(const Buffer& b) const { return !equals(b); }

private:
	Decoder *d;
	t::uint8 *b;
	size_t sz;
};
io::Output& operator<<(io::Output& out, const Buffer& buf);

class Cursor {
public:
	inline Cursor(void): off(0) { }
	inline Cursor(const Buffer& b): off(0), buf(b) { }
	inline Decoder *decoder(void) const { return buf.decoder(); }

	inline bool ended(void) const { return off >= buf.size(); }
	inline operator bool(void) const { return !ended(); }
	inline bool avail(size_t s) const { return off + s <= buf.size(); }
	inline const t::uint8 *here(void) const { return buf.at(off); }
	inline offset_t offset(void) const { return off; }
	inline void finish(void) { off = buf.size(); }
	inline bool equals(const Cursor& c) const { return off == c.off && buf.equals(c.buf); }
	inline size_t size() const { return buf.size(); }

	inline bool skip(size_t s) { if(!avail(s)) return false; off += s; return true; }
	inline bool move(offset_t o) { if(o > buf.size()) return false; off = o; return true; }

	inline bool read(t::uint8& v)
		{ if(!avail(sizeof(t::uint8))) return false; buf.get(off, v); off += sizeof(t::uint8); return true; }
	inline bool read(t::int8& v)
		{ if(!avail(sizeof(t::int8))) return false; buf.get(off, v); off += sizeof(t::int8); return true; }
	inline bool read(t::uint16& v)
		{ if(!avail(sizeof(t::uint16))) return false; buf.get(off, v); off += sizeof(t::uint16); return true; }
	inline bool read(t::int16& v)
		{ if(!avail(sizeof(t::int16))) return false; buf.get(off, v); off += sizeof(t::int16); return true; }
	inline bool read(t::uint32& v)
		{ if(!avail(sizeof(t::uint32))) return false; buf.get(off, v); off += sizeof(t::uint32); return true; }
	inline bool read(t::int32& v)
		{ if(!avail(sizeof(t::int32))) return false; buf.get(off, v); off += sizeof(t::int32); return true; }
	inline bool read(t::uint64& v)
		{ if(!avail(sizeof(t::uint64))) return false; buf.get(off, v); off += sizeof(t::uint64); return true; }
	inline bool read(t::int64& v)
		{ if(!avail(sizeof(t::int64))) return false; buf.get(off, v); off += sizeof(t::int64); return true; }
	bool read(cstring& s);
	inline bool read(string& s) { cstring r; read(r); s = string(r); return true; }
	bool read(size_t size, const t::uint8 *& buf);

	inline bool write(t::uint8 v)
		{ if(!avail(sizeof(t::uint8))) return false; buf.set(off, v); off += sizeof(t::uint8); return true; }
	inline bool write(t::int8 v)
		{ if(!avail(sizeof(t::int8))) return false; buf.set(off, v); off += sizeof(t::int8); return true; }
	inline bool write(t::uint16 v)
		{ if(!avail(sizeof(t::uint16))) return false; buf.set(off, v); off += sizeof(t::uint16); return true; }
	inline bool write(t::int16 v)
		{ if(!avail(sizeof(t::int16))) return false; buf.set(off, v); off += sizeof(t::int16); return true; }
	inline bool write(t::uint32 v)
		{ if(!avail(sizeof(t::uint32))) return false; buf.set(off, v); off += sizeof(t::uint32); return true; }
	inline bool write(t::int32 v)
		{ if(!avail(sizeof(t::int32))) return false; buf.set(off, v); off += sizeof(t::int32); return true; }
	inline bool write(t::uint64 v)
		{ if(!avail(sizeof(t::uint64))) return false; buf.set(off, v); off += sizeof(t::uint64); return true; }
	inline bool write(t::int64 v)
		{ if(!avail(sizeof(t::int64))) return false; buf.set(off, v); off += sizeof(t::int64); return true; }
	inline bool write(cstring s)
		{ if(!avail(s.length() + 1)) return false; buf.set(off, s); off += s.length(); return true; }
	inline bool write(string s)
		{ if(!avail(s.length() + 1)) return false; buf.set(off, s); off += s.length(); return true; }

private:
	offset_t off;
	Buffer buf;
};

} // gel

#endif /* INCLUDE_GEL___BASE_H_ */
