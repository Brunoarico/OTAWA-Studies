/*
 *	avl::Queue class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2020, IRIT UPS.
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
#ifndef ELM_AVL_QUEUE_H_
#define ELM_AVL_QUEUE_H_

#include <elm/avl/GenTree.h>

namespace elm { namespace avl {

template <class T, class C = elm::Comparator<T>, class A = DefaultAlloc>
class Queue: public GenTree<T, IdAdapter<T>, C, A> {
public:
	typedef T t;
	typedef Queue<T, C> self_t;
	typedef GenTree<T, IdAdapter<T>, C, A> base_t;

	// Queue concept

	inline const T& head() const {
		typename base_t::Node *n = base_t::root();
		ASSERTP(n != nullptr, "empty queue");
		while(n->left() != nullptr) n = n->left();
		return n->data;
	}

	inline T get() {
		ASSERTP(base_t::root() != nullptr, "empty queue");
		typename base_t::Stack s;
		typename base_t::Node *n = static_cast<typename base_t::Node *>(base_t::leftMost(s, base_t::root()));
		T r = n->data;
		base_t::remove(s, n);
		return r;
	}

	inline void put(const T& x) { base_t::add(x); }

	inline void reset() { base_t::clear(); }
};

} }	// elm::avl

#endif /* ELM_AVL_SET_H_ */
