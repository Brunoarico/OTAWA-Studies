/*
 *	Range class
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2016, IRIT UPS.
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
#ifndef ELM_DATA_RANGE_H_
#define ELM_DATA_RANGE_H_

#include "../equiv.h"

namespace elm {

template <class I>
class Range {
public:
	typedef typename I::t t;

	inline Range(const I& begin, const I& end)
		: _begin(begin), _end(end) { }

	class Iter: public PreIterator<Iter, t> {
	public:
		typedef Range collec_t;
		inline Iter(const Range& collection): c(collection), i(c._begin) { }
		inline Iter(const Range& collection, const I& ii): c(collection), i(ii) { }
		inline bool ended(void) const { return i.equals(c._end); }
		inline typename I::return_t item(void) const { return i.item(); }
		inline void next(void) { i.next(); }
		inline bool equals(const Iter& ii) const { return i.equals(ii.i); }
	private:
		const Range& c;
		I i;
	};

	inline Iter begin() const { return Iter(*this, _begin); }
	inline Iter end() const { return Iter(*this, _end); }

	inline bool isEmpty(void) const { return _begin.equals(_end); }
	inline int count(void) const
		{ int c = 0; for(Iter i(*this); i(); i++) c++; return c; }
	inline operator bool(void) const { return !isEmpty(); }
	inline bool contains(const t& v)
		{ for(Iter i(*this); i(); i++) if(Equiv<t>::equals(*i, v)) return true; return false; }
	template <class C> inline bool containsAll(const C& c)
	 	 { for(typename C::Iter i(c); i; i++) if(!contains(*i)) return false; return true; }

private:
	I _begin, _end;
};

template <class I>
Range<I> range(const I& begin, const I& end) { return Range<I>(begin, end); }

}	// elm

#endif /* ELM_DATA_RANGE_H_ */
