/*
 * $Id$
 * Copyright (c) 2006, IRIT-UPS.
 *
 * Meta-programming definitions.
 */
#ifndef ELM_META_H
#define ELM_META_H


namespace elm {

namespace meta {

	template <int x> struct n { enum { _ = x }; };
	struct _true { enum { _ = 1 }; };
	struct _false { enum { _ = 0 }; };

	template <bool c, class T, class E> struct _if: public T {  };
	template <class T, class E> struct _if<false, T, E>: public E {  };

	template <class> struct sfinae_true: _true { };
	template <class T> inline T& declval() { return *static_cast<T *>(nullptr); }

	template <typename U, typename t> struct is_same: _false { };
	template <typename U> struct is_same<U, U>: _true { };

	template <typename T, template <typename X> class C>
	struct is_supported {
		template <class U> static _true c(C<U> *);
		template <class U> static _false c(...);
		enum { _ = decltype(c<T>(0))::_ };
	};

	template<bool B, class T = void> struct enable_if {};
	template<class T> struct enable_if<true, T> { typedef T _; };
}

// deprecated
template <int x> struct _n { enum { _ = x }; };
struct _true { enum { _ = 1 }; };
struct _false { enum { _ = 0 }; };
template <bool c, class T, class E> struct _if: public T {  };
template <class T, class E> struct _if<false, T, E>: public E {  };

}	// elm

#endif // ELM_META_H

