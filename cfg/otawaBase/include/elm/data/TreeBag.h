/*
 *	TreeSet class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2004-07, IRIT UPS.
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
#ifndef ELM_DATA_TREEBAG_H
#define ELM_DATA_TREEBAG_H

#include <elm/utility.h>
#include <elm/PreIterator.h>
#include <elm/data/Vector.h>
#include <elm/data/VectorQueue.h>
#include <elm/inhstruct/BinTree.h>
#include <elm/compare.h>
#include <elm/adapter.h>

namespace elm {

template <class T, class C = Comparator<T>, class A = DefaultAlloc >
class TreeBag: public C, public A {
private:

	class Node: public inhstruct::BinTree::Node {
	public:
		inline Node(const T& value): val(value) { }
		T val;
		inline void *operator new(size_t size, TreeBag<T, C, A> *t)
			{ return t->A::allocate(size); }
		inline void free(TreeBag<T, C, A> *t)
			{ this->~Node(); t->A::free(this); }
	};

public:

	// Methods
	inline TreeBag() { }
	inline TreeBag(const TreeBag<T, C>& t) { copy(t); }
	inline ~TreeBag(void) { clear(); }
	const C& comparator() const { return *this; }
	C& comparator() { return *this; }
	A& allocator() { return *this; }

	// Collection concept
	inline int count(void) const { return root.count(); }
	inline bool contains(const T& x) const { return find(x) != nullptr; }
	inline bool isEmpty(void) const { return root.isEmpty(); }
 	inline operator bool(void) const { return !isEmpty(); }

 	template <class CC> bool containsAll(const CC& c) const
 		{ for(const auto x: c) if(!contains(x)) return false; return true; }

	class Iter: public PreIterator<Iter, const T&> {
		friend class TreeBag;
		inline Node *_(inhstruct::BinTree::Node *n) { return static_cast<Node *>(n); }
	public:
		inline Iter(const TreeBag& tree)
			{ if(tree.root.root()) downLeft(_(tree.root.root())); }
		bool ended(void) const { return !s; }
		void next(void)
			{	if(s.top()->right()) downLeft(_(s.top()->right()));
				else { Node *n = s.pop(); if(s && n == s.top()->right()) upRight(n); } }
		const T& item(void) const { return s.top()->val; }
		inline bool equals(const Iter& i) const { return s == i.s; }

	private:
		inline Iter() { }
		inline void downLeft(Node *n)
			{ s.push(n); while(s.top()->left()) s.push(_(s.top()->left())); }
		inline void upRight(Node *n)
			{ while(s && s.top()->right() == n) n = s.pop(); }
		Vector<Node *> s;
	};

	inline Iter begin() const { return Iter(*this); }
	inline Iter end() const { return Iter(); }

	inline bool equals(const TreeBag<T, C>& t) const {
		Iter i(*this), j(t);
		while(i() && j()) {
			if(comparator().doComparator(*i, *j) != 0)
				return false;
			i++, j++;
		}
		return !i() && !j();
	}
	inline bool operator==(const TreeBag<T, C>& t) const { return equals(t); }
	inline bool operator!=(const TreeBag<T, C>& t) const { return !equals(t); }

	// MutableCollection concept
	void clear(void)  {
		if(isEmpty())
			return;
		VectorQueue<Node *> todo;
		todo.put((Node *)root.root());
		while(todo) {
			Node *node = todo.get();
			if(node->left())
				todo.put((Node *)node->left());
			if(node->right())
				todo.put((Node *)node->right());
			node->free(this);
		}
	}


	void add(const T &x) {
		Node *node = (Node *)root.root();
		Node *new_node = new(this) Node(x);
		if(!node)
			root.setRoot(new_node);
		else
			while(node) {
				int cmp = C::doCompare(x, node->val);
				if(cmp >= 0) {
					if(!node->right()) {
						node->insertRight(new_node);
						break;
					}
					else
						node = (Node *)node->right();
				}
				else {
					if(!node->left()) {
						node->insertLeft(new_node);
						break;
					}
					else
						node = (Node *)node->left();
				}
			}
	}

	template <class CC> void addAll (const CC &c)
		{ for(const auto x: c) add(x); }
	void remove(const T& x) {
		Node *node = (Node *)root.root(), *parent = 0;
		while(true) {
			ASSERT(node);
			int cmp = C::compare(x, node->val);
			if(cmp == 0)
				break;
			parent = node;
			if(cmp > 0)
				node = (Node *)node->right();
			else
				node = (Node *)node->left();
		}
		Node *left = (Node *)node->left(), *right = (Node *)node->right();
		node->free(this);
		if(!left)
			replace(parent, node, right);
		else if(!right)
			replace(parent, node, left);
		else {
			replace(parent, node, left);
			for(node = left; node->right(); node = (Node *)node->right());
			node->insertRight(right);
		}
	}

	template <class CC> void removeAll(const CC &c)
		{ for(const auto x: c) remove(x); }
	inline void remove(const Iter &iter) { remove(*iter); }

	void copy(const TreeBag<T, C>& t) {
		// TODO improve it!
		clear();
		addAll(t);
	}

	const T *find(const T& x) const {
		Node *node = (Node *)root.root();
		while(node) {
			int cmp = C::compare(x, node->val);
			if(cmp == 0)
				return &node->val;
			else if(cmp > 0)
				node = (Node *)node->right();
			else
				node = (Node *)node->left();
		}
		return nullptr;
	}

private:
	inhstruct::BinTree root;

	void replace(Node *parent, Node *old, Node *_new)  {
		if(!parent)
			root.setRoot(_new);
		else if(parent->left() == old)
			parent->insertLeft(_new);
		else
			parent->insertRight(_new);
	}
};

} // elm

#endif // ELM_GENSTRUCT_SORTEDBINTREE_H
