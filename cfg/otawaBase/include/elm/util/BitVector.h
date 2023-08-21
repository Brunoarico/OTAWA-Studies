/*
 *	BitVector class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2005-17, IRIT UPS.
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
#ifndef ELM_UTIL_BIT_VECTOR_H
#define ELM_UTIL_BIT_VECTOR_H

#include <elm/assert.h>
#include <elm/io.h>
#include <elm/PreIterator.h>

namespace elm {

// BitVector class
class BitVector {
	typedef t::uint8 word_t;
public:
	inline BitVector(void): bits(nullptr), _size(0) { }
	BitVector(int size, bool set = false);
	BitVector(const BitVector& vec);
	BitVector(const BitVector& vec, int new_size);
	inline ~BitVector(void) { if(bits) delete [] bits; }
	inline int size(void) const { return _size; }

	inline bool bit(int i) const {
		ASSERTP(i < _size, "index out of bounds");
		return (bits[windex(i)] & (1 << bindex(i))) != 0;
	}

	inline bool isEmpty(void) const {
		mask();
		for(int i = 0; i < wcount(); i++)
			if(bits[i]) return false;
		return true;
	}

	bool includes(const BitVector& vec) const;
	bool includesStrictly(const BitVector &vec) const;
	bool equals(const BitVector& vec) const;
	int countBits(void) const;
	void resize(int new_size);
	bool meets(const BitVector& bv);
	
	inline void set(int index) const
		{ ASSERTP(index < _size, "index out of bounds"); bits[windex(index)] |= word_t(1) << bindex(index); }
	inline void set(int index, bool value) const
		{ ASSERTP(index < _size, "index out of bounds"); if(value) set(index); else clear(index); }
	inline void clear(int index) const
		{ ASSERTP(index < _size, "index out of bounds"); bits[windex(index)] &= ~(word_t(1) << bindex(index)); }
	
	void copy(const BitVector& bits) const;
	void clear(void);
	void set(void);

	void applyNot(void);
	void applyOr(const BitVector& vec);
	void applyAnd(const BitVector& vec);
	void applyReset(const BitVector& vec);
#ifdef EXPERIMENTAL
	inline void shiftLeft(int n = 1) { doShiftLeft(n, bits); }
	inline void shiftRight(int n = 1) { doShiftRight(n, bits); }
	inline void rotateLeft(int n = 1)  { doRotateLeft(n, bits); }
	inline void rotateRight(int n = 1) { doRotateRight(n, bits); }
#endif
	
	BitVector makeNot(void) const;
	BitVector makeOr(const BitVector& vec) const;
	BitVector makeAnd(const BitVector& vec) const;
	BitVector makeReset(const BitVector& vec) const;
#ifdef EXPERIMENTAL
	inline BitVector makeShiftLeft(int n = 1) const { BitVector r(size()); doShiftLeft(n, r.bits); return r; }
	inline BitVector makeShiftRight(int n = 1) const { BitVector r(size()); doShiftRight(n, r.bits); return r; }
	inline BitVector makeRotateLeft(int n = 1) const { BitVector r(size()); doRotateLeft(n, r.bits); return r; }
	inline BitVector makeRotateRight(int n = 1) const { BitVector r(size()); doRotateRight(n, r.bits); return r; }
#endif
	
	void print(io::Output& out) const;
	
	// useful operations
	int countOnes(void) const;
	inline int countZeroes(void) const { return countBits() - countOnes(); }

	class Iter {
	public:
		inline Iter(const BitVector& v): bvec(v), i(0) { }
		inline bool ended() const { return i >= bvec.size(); }
		inline bool item() const { return bvec.bit(i); }
		inline void next() { i++; }
		inline bool equals(const Iter& it) const { return i == it.i; }

		inline bool operator()() const { return !ended(); }
		inline bool operator*() const { return item(); }
		inline Iter& operator++() { next(); return *this; }
		inline Iter operator++(int) { Iter o = *this; next(); return o; }
		inline bool operator==(const Iter& it) const { return equals(it); }
		inline bool operator!=(const Iter& it) const { return !equals(it); }

	protected:
		const BitVector& bvec;
		int i;
	};

	// OneIterator iter
	class OneIterator: public Iter {
	public:
		inline OneIterator(const BitVector& bit_vector, int ii = 0): Iter(bit_vector) { i = ii - 1; next(); }
		inline int item() const  { return i; }
		inline void next() { do i++; while(i < bvec.size() && !bvec.bit(i)); }
		inline int operator*() const { return item(); }
		inline Iter& operator++() { next(); return *this; }
		inline Iter operator++(int) { Iter o = *this; next(); return o; }
	};
	inline OneIterator begin() const { return OneIterator(*this); }
	inline OneIterator end() const { return OneIterator(*this, size()); }

	// ZeroIterator iter
	class ZeroIterator: public Iter {
	public:
		inline ZeroIterator(const BitVector& bit_vector): Iter(bit_vector) { i = -1; next(); }
		inline int item(void) const  { return i; }
		inline void next(void) { do i++; while(i < bvec.size() && bvec.bit(i)); }
		inline int operator*() const { return item(); }
		inline Iter& operator++() { next(); return *this; }
		inline Iter operator++(int) { Iter o = *this; next(); return o; }
	};

	// Ref delegate
	class Ref {
	public:
		inline Ref(BitVector& v, int i): _v(v), _i(i) { }
		inline bool get(void) const { return _v.bit(_i); }
		inline void set(void) { _v.set(_i); }
		inline void clear(void) { _v.clear(_i); }
		inline void set(bool b) { if(b) set(); else clear(); }

		inline operator bool(void) const { return get(); }
		inline Ref& operator=(bool b) { set(b); return *this;}
	private:
		BitVector& _v;
		int _i;
	};

	// Operators
	inline operator bool(void) const 						{ return !isEmpty(); }
	inline bool operator[](int index) const	 				{ return bit(index); }
	inline Ref operator[](int i)							{ return Ref(*this, i); }
	inline BitVector operator~(void) const 					{ return makeNot(); }
	inline BitVector operator|(const BitVector& vec) const	{ return makeOr(vec); }
	inline BitVector operator&(const BitVector& vec) const	{ return makeAnd(vec); }
	inline BitVector operator+(const BitVector& vec) const	{ return makeOr(vec); }
	inline BitVector operator-(const BitVector& vec) const	{ return makeReset(vec); }
#ifdef EXPERIMENTAL
	inline BitVector operator<<(int n) const				{ return makeShiftLeft(n); }
	inline BitVector operator>>(int n) const				{ return makeShiftRight(n); }
#endif
	BitVector& operator=(const BitVector& vec);
	inline BitVector& operator|=(const BitVector& vec)		{ applyOr(vec); return *this; }
	inline BitVector& operator&=(const BitVector& vec)		{ applyAnd(vec); return *this; }
	inline BitVector& operator+=(const BitVector& vec)		{ applyOr(vec); return *this; }
	inline BitVector& operator-=(const BitVector& vec)		{ applyReset(vec); return *this; }
#ifdef EXPERIMENTAL
	inline BitVector& operator<<=(int d)					{ shiftLeft(d); return *this; }
	inline BitVector& operator>>=(int d)					{ shiftRight(d); return *this; }
#endif
	inline bool operator==(const BitVector& vec) const		{ return equals(vec); }
	inline bool operator!=(const BitVector& vec) const		{ return !equals(vec); }
	inline bool operator<(const BitVector& vec) const		{ return vec.includesStrictly(*this); }
	inline bool operator<=(const BitVector& vec) const		{ return vec.includes(*this); }
	inline bool operator>(const BitVector& vec) const		{ return includesStrictly(vec); }
	inline bool operator>=(const BitVector& vec) const		{ return includes(vec); }

	inline t::size __size(void) const { return sizeof(*this) + wcount() * sizeof(word_t); }

private:
	word_t *bits;
	int _size;

	inline int wsize(void) const { return sizeof(word_t) << 3; }
	inline int wshift(void) const { return type_info<word_t>::shift + 3; }
	inline int inWords(int s) const { return (s + wsize() - 1) >> wshift(); }
	inline int wcount(void) const { return inWords(_size); }
	inline int windex(int index) const { return index >> wshift(); }
	inline int bindex(int index) const { return index & (wsize() - 1); }

	inline void mask(word_t *bits) const {
		word_t mask = word_t(-1) >> (wsize() - bindex(_size));
		if(mask) bits[wcount() - 1] &= mask;
	}
	inline void mask(void) const { mask(bits); }
#ifdef EXPERIMENTAL
	void doShiftLeft(int n, word_t *tbits) const;
	void doShiftRight(int n, word_t *tbits) const;
	void doRotateLeft(int n, word_t *tbits) const;
	void doRotateRight(int n, word_t *tbits) const;
#endif
};

inline io::Output& operator<<(io::Output& out, const BitVector& bvec)
	{ bvec.print(out); return out; }

} // elm

#endif	// ELM_UTIL_BIT_VECTOR_H
