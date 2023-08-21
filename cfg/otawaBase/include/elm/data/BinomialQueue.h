/*
 *	BinomialQueue class
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2021, IRIT UPS.
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
#ifndef INCLUDE_ELM_DATA_BINOMIALQUEUE_H_
#define INCLUDE_ELM_DATA_BINOMIALQUEUE_H_

#include <elm/assert.h>
#include <elm/compare.h>
#include <elm/data/custom.h>

namespace elm {

template <class T, class C = Comparator<T>, class A = DefaultAlloc>
class BinomialQueue: public C, public A {
	const static unsigned NOT_IN_HEAP = type_info<unsigned>::max;

	class Node {
	public:
		Node(const T& xx)
			: next(nullptr), child(nullptr),
			degree(NOT_IN_HEAP), x(xx) { }
		Node *next, *child;
		unsigned degree;
		T x;
	};

public:
	BinomialQueue(const C& c = single<C>(), const A& a = single<A>())
		: C(c), A(a), _head(nullptr), _min(nullptr) { }

	inline bool isEmpty() const { return _head == nullptr && _min == nullptr; }

	const T& head() const {
		Node *prev;
		Node *node = min(prev);
		return node->x;
	}

	T get() {
		Node *p;
		Node *n = min(p);
		if(p != nullptr)
			p->next = n->next;
		else
			_head = n->next;
		join(reverse(n->child));
		auto x = n->x;
		A::free(n);
		return x;
	}

	void put(const T& x) {
		auto n = new(A::allocate(sizeof(Node))) Node(x);
		n->degree = 0;
		if(_min != nullptr && higher(n->x, _min->x)) {
			auto m = _min;
			m->child = nullptr;
			m->next = nullptr;
			m->degree = 0;
			join(m);
			_min = n;
		}
		else {
			join(n);
		}
	}

#	ifdef TEST_BINOMIAL_QUEUE
		void dump(io::Output& out);
		void dump_rec(io::Output& out, Node *node);
#	endif
private:

	inline bool higher(const T& x, const T& y) const { return C::doCompare(x, y) <= 0; }

	inline void link(Node *root, Node *child) {
		child->next = root->child;
		root->child = child;
		root->degree++;
	}

	Node *min(Node*& prev) {
		ASSERTP(_head != nullptr, "no head in empty queue");
		prev = nullptr;
		Node *n = _head, *p = _head, *c = _head->next;
		while(c != nullptr) {
			if(higher(c->x, n->x)) {
				n = c;
				prev = p;
			}
			p = c;
			c = c->next;
		}
		return n;
	}

	Node *merge(Node *a, Node *b) {
		Node *h = nullptr, **p = &h;
		while(a != nullptr && b != nullptr) {
			if(a->degree < b->degree) {
				*p = a;
				a = a->next;
			}
			else {
				*p = b;
				b = b->next;
			}
			p = &(*p)->next;
		}
		if(a != nullptr)
			*p = a;
		else
			*p = b;
		return h;
	}

	void join(Node *h2) {
		if(h2 == nullptr)
			return;
		if(_head == nullptr) {
			_head = h2;
			return;
		}
		Node *h1 = merge(_head, h2);
		Node *p = nullptr, *x = h1, *n = x->next;
		while(n != nullptr) {
			if(x->degree != n->degree || (n->next != nullptr && n->next->degree == x->degree)) {
				p = x;
				x = n;
			}
			else if(higher(x->x, n->x)) {
				x->next = n->next;
				link(x, n);
			}
			else {
				if(p != nullptr)
					p->next = n;
				else
					h1 = n;
				link(n, x);
				x = n;
			}
			n = x->next;
		}
		_head = h1;
	}

	Node *reverse(Node *h) {
		if(h == nullptr)
			return nullptr;
		Node *t = nullptr;
		while(h->next != nullptr) {
			auto n = h->next;
			h->next = t;
			t = h;
			h = n;
		}
		h->next = t;
		return h;
	}

	Node *_head, *_min;
};

}	// elm

#endif /* INCLUDE_ELM_DATA_BINOMIALQUEUE_H_ */

