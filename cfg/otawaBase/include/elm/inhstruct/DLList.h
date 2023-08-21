/*
 *	inhstruct::BiDiList class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2004-16, IRIT UPS.
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
#ifndef ELM_INHSTRUCT_DLLIST_H
#define ELM_INHSTRUCT_DLLIST_H

#include <elm/assert.h>

namespace elm { namespace inhstruct {

// DLList class
class DLList;
class DLNode {
	friend class DLList;
	DLNode *nxt, *prv;
public:
	inline DLNode *next(void) const { return nxt; }
	inline DLNode *previous(void) const { return prv; }
	inline bool atBegin(void) const { return prv == 0; }
	inline bool atEnd(void) const { return nxt == 0; }
	
	inline void replace(DLNode *node) {
		ASSERTP(node, "null node for replacement");
		nxt->prv = node; node->nxt = nxt;
		prv->nxt = node; node->prv = prv;
	}

	inline void insertAfter(DLNode *node) {
		ASSERTP(node, "null node to insert");
		nxt->prv = node; node->nxt = nxt;
		nxt = node; node->prv = this;
	}

	inline void insertBefore(DLNode *node) {
		ASSERTP(node, "null node to insert");
		prv->nxt = node; node->prv = prv;
		prv = node; node->nxt = this;
	}

	inline void remove(void)
		{ prv->nxt = nxt; nxt->prv = prv; }
	inline void removeNext(void)
		{ ASSERTP(!nxt->atEnd(), "no next node"); nxt->remove(); }
	inline void removePrevious(void)
		{ ASSERTP(!prv->atBegin(), "no previous node"); prv->remove(); }
};


// DLList class
class DLList {
	mutable DLNode hd, tl;
public:
	inline DLList(void) {
		hd.nxt = &tl; hd.prv = 0;
		tl.prv = &hd; tl.nxt = 0;
	}

	inline DLList(DLList& list) {
		hd.prv = 0;
		tl.nxt = 0;
		if(list.isEmpty()) {
			hd.nxt = &tl;
			tl.prv = &hd;
		}
		else {
			hd.nxt = list.hd.nxt;
			list.hd.nxt = 0;
			hd.nxt->prv = &hd;
			tl.prv = list.tl.prv;
			list.tl.prv = 0;
			tl.prv->nxt = &tl;

		}
	}

	inline DLNode *first(void) const { return hd.nxt; }
	inline DLNode *last(void) const { return tl.prv; }
	inline bool isEmpty(void) const { return hd.nxt == &tl; }
	inline DLNode *head(void) const { return &hd; }
	inline DLNode *tail(void) const { return &tl; }

	inline int count(void) const {
		int cnt = 0;
		for(DLNode *cur = hd.nxt; cur != &tl; cur =cur->nxt)
			cnt++;
		return cnt;
	}

	inline void addFirst(DLNode *node)
		{ ASSERTP(node, "null node added"); hd.insertAfter(node); }
	inline void addLast(DLNode *node)
		{ ASSERTP(node, "null node added"); tl.insertBefore(node); }
	inline void removeFirst(void)
		{ ASSERTP(!isEmpty(), "list empty"); hd.removeNext(); }
	inline void removeLast(void)
		{ ASSERTP(!isEmpty(), "list empty"); tl.removePrevious(); }
};


} } // elm::inhstruct

#endif	// ELM_INHSTRUCT_DLLIST_H
