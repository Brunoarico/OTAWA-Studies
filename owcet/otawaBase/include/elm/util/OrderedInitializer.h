/*
 *	OrderedInitializer class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2015, IRIT UPS.
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
#ifndef ELM_ORDERED_INITIALIZER_H
#define ELM_ORDERED_INITIALIZER_H

namespace elm {

// OrderedInitializer class
template <class T>
class OrderedInitializer {
	typedef struct node_t {
		struct node_t *next;
		T *object;
		inline node_t(T *_object, struct node_t *_next)
		: object(_object), next(_next) { }
	} node_t;
	
	static node_t *list;
	static bool initialized;
public:
	OrderedInitializer(bool start = true);
	~OrderedInitializer(void);
	void record(T *object);
	void startup(void);
};

// Statics
template <class T> typename Initializer<T>::node_t *Initializer<T>::list = 0;
template <class T> bool Initializer<T>::initialized = false;

// Initializer<T>::Initializer
template <class T>
Initializer<T>::Initializer(bool start) {
	if(start)
		startup();
}

// Initializer<T>::~Initializer
template <class T>
Initializer<T>::~Initializer(void) {
	for(node_t *node = list, *next; node; node = next) {
		next = node->next;
		delete node;
	}
}

// Initializer<T>::record
template <class T>
void Initializer<T>::record(T *object) {
	if(initialized)
		object->initialize();
	else {
		list = new node_t(object, list);
	}
}

// Initializer<T>::startup()
template <class T>
void Initializer<T>::startup(void) {
	if(!initialized) {
		initialized = true;
		for(node_t *node = list, *next; node; node = next) {
			next = node->next;
			node->object->initialize();
			delete node;
		}
		list = 0;
	}
}

} // elm

#endif // ELM_INITIALIZER_H

