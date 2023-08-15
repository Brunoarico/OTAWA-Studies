/*
 *	test facilities interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2005, IRIT UPS.
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
#ifndef ELM_UTIL_TEST_H
#define ELM_UTIL_TEST_H

#include <elm/string.h>
#include <elm/io.h>
#include <elm/util/Initializer.h>
#include <elm/data/List.h>

namespace elm {

// TestCase class
class TestCase {
	CString _name;
	int tests;
	int errors;
public:
	TestCase(CString name);
	void initialize(void);
	virtual ~TestCase(void);
	inline cstring name(void) { return _name; }
	void test(CString file, int line, CString text);
	void failed(void);
	void succeeded(void);
	void check(CString file, int line, CString text, bool result);
	bool require(CString file, int line, CString text, bool result);

	template <class T, class U> inline void check_equal(CString file, int line,
	CString text, const T& result, const U& reference) {
		bool test = result == reference;
		check(file, line, text, test);
		if(!test)
			cout << '\t' << result << " != " << reference << "\n";
	}

	void prepare(void);
	void complete(void);
	void perform(void);
	inline bool isSuccessful(void) const { return errors == 0; }
	inline bool hasFailed(void) const { return errors != 0; }
	inline bool isFullPath() const { return full_path; }
	inline void setFullPath(bool x) { full_path = x; }
protected:
	virtual void execute(void);
	TestCase& __case;
	bool full_path;
};

class TestSet: private Initializer<TestCase> {
public:
	static TestSet def;

	void perform(void);

	class Iterator: public List<TestCase *>::Iter {
	public:
		inline Iterator(const TestSet& set): List<TestCase *>::Iter(set.cases) { }
	};

private:
	friend class TestCase;
	void add(TestCase *tcase);
	List<TestCase *> cases;
};


// Macros
//#define ELM_CHECK_MAKE(name, actions) class name##Test: public { name##Test(void)
#define ELM_CHECK_BEGIN(name)	{ elm::TestCase __case(name); __case.prepare();
#define ELM_CHECK(tst)			__case.check(__FILE__, __LINE__, #tst, tst)
#define ELM_CHECK_MSG(msg, res)	__case.check(__FILE__, __LINE__, msg, res)
#define ELM_CHECK_END 			__case.complete(); }
#define ELM_CHECK_RETURN 		__case.complete(); if(__case.isSuccessful()) return 0; else return 1; }
#define ELM_REQUIRE(tst, action)	if(!__case.require(__FILE__, __LINE__, #tst, tst)) action
#define ELM_CHECK_EQUAL(res, ref)	__case.check_equal(__FILE__, __LINE__, #res " == " #ref, res, ref)
#define ELM_CHECK_EXCEPTION(exn, stat)	{ __case.test(__FILE__, __LINE__, #stat); \
	try { stat; __case.failed(); } catch(const exn&) { __case.succeeded(); } }
#define ELM_FAIL_ON_EXCEPTION(exn, stat) { __case.test(__FILE__, __LINE__, #stat); \
	try { stat; __case.succeeded(); } \
	catch(exn& e) { __case.failed(); cerr << "exception = " << e.message() << elm::io::endl; } }
#define ELM_TEST_BEGIN(name) \
	static class name##Test: public elm::TestCase { \
	public: \
		name##Test(void): elm::TestCase(#name) { } \
	protected: \
		virtual void execute(void) {
#define ELM_TEST_END \
		} \
	} __test;

// shortcuts
#ifndef ELM_NO_SHORTCUT
#	define CHECK_BEGIN(name) ELM_CHECK_BEGIN(name)
#	define CHECK(tst) ELM_CHECK(tst)
#	define CHECK_MSG(msg, res) ELM_CHECK_MSG(msg, res)
#	define REQUIRE(tst, action) ELM_REQUIRE(tst, action)
#	define CHECK_EQUAL(res, ref) ELM_CHECK_EQUAL(res, ref)
#	define CHECK_END ELM_CHECK_END
#	define CHECK_EXCEPTION(exn, stat) ELM_CHECK_EXCEPTION(exn, stat)
#	define FAIL_ON_EXCEPTION(exn, stat) ELM_FAIL_ON_EXCEPTION(exn, stat)
#	define TEST_BEGIN(name) ELM_TEST_BEGIN(name)
#	define TEST_END	 ELM_TEST_END
#	define CHECK_RETURN	ELM_CHECK_RETURN
#endif

} // elm

#endif // ELM_UTIL_TEST_H
