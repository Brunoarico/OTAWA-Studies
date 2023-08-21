/*
 *	With and WithPtr class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2023, IRIT UPS.
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
#ifndef ELM_UTIL_WITH_H
#define ELM_UTIL_WITH_H

namespace elm {

template <class T>
class With {
public:
	With(): _p(nullptr) {}
	With(T& p):_p(&p) { _p->enter(); }
	With(const With& w): _p(w._p) {  w._p = nullptr; }
	~With() { if(_p != nullptr) _p->leave(); }
	inline T& operator*() const { return *_p; }
	inline operator T *() const { return _p; }
	//inline With<T>& operator=(const With& w) { _p = w._p; w._p = nullptr; }
private:
	mutable T *_p;
};
template <class T> inline With<T> with(T& p) { return With<T>(p); }

template <class T>
class WithPtr {
public:
	WithPtr(): _p(nullptr) {}
	WithPtr(T *p):_p(p) { }
	WithPtr(const WithPtr& w): _p(w._p) {  w._p = nullptr; }
	~WithPtr() { if(_p != nullptr) delete _p; }
	inline T *operator->() const { return _p; }
	inline T& operator*() const { return *_p; }
	inline operator T *() const { return _p; }
private:
	mutable T *_p;
};
template <class T> inline WithPtr<T> with_ptr(T *p) { return WithPtr<T>(p); }

}	// elm

#endif	// ELM_UTIL_WITH_H
