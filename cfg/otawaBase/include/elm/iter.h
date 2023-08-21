/*
 *	Tool classes for iterators.
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2019, IRIT UPS.
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
#ifndef ELM_ITER_H_
#define ELM_ITER_H_

namespace elm {

// PreIterator class
template <class I, class T>
class PreIterator {
public:
	typedef T t;
	typedef T return_t;

	inline bool operator()() const { return !((I *)this)->ended(); }
	inline bool operator!() const { return ((I *)this)->ended(); }

	inline T operator*() const { return ((I *)this)->item(); }
	inline T operator->() const { return ((I *)this)->item(); }

	inline I& operator++() { ((I *)this)->next(); return *(I *)this; }
	inline void operator++(int) { ((I *)this)->next(); }

	inline bool operator==(const I& i) const { return ((I *)this)->equals(i); }
	inline bool operator!=(const I& i) const { return !((I *)this)->equals(i); }

};

// InplacePreIterator class
template <class I, class T>
class InplacePreIterator {
public:
	typedef T t;
	typedef const T& return_t;

	inline bool operator()() const { return !((I *)this)->ended(); }
	inline bool operator!() const { return ((I *)this)->ended(); }

	inline const T& operator*() const { return ((I *)this)->item(); }
	inline const T& operator->() const { return ((I *)this)->item(); }

	inline I& operator++() { ((I *)this)->next(); return *(I *)this; }
	inline void operator++(int) { ((I *)this)->next(); }

	inline bool operator==(const I& i) const { return ((I *)this)->equals(i); }
	inline bool operator!=(const I& i) const { return !((I *)this)->equals(i); }

};

template <class I, class T>
class PreIter {
public:
	typedef T t;
	inline bool operator()() const { return !((I *)this)->ended(); }
	inline bool operator!() const { return ((I *)this)->ended(); }

	inline I& operator++() { ((I *)this)->next(); return *(I *)this; }
	inline void operator++(int) { ((I *)this)->next(); }

	inline bool operator==(const I& i) const { return ((I *)this)->equals(i); }
	inline bool operator!=(const I& i) const { return !((I *)this)->equals(i); }

};

template <class I, class T>
class ConstPreIter {
public:
	typedef const T& return_t;
	inline const T& operator*() const { return ((I *)this)->item(); }
	inline T operator->() const { return ((I *)this)->item(); }
};

template <class I, class T>
class MutPreIter {
public:
	typedef T& return_t;
	inline T& operator*() const { return ((I *)this)->item(); }
	inline T operator->() const { return ((I *)this)->item(); }
};

}	// elm

#endif /* ELM_ITER_H_ */
