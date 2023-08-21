/*
 *	Path class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2005-12, IRIT UPS.
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
#ifndef ELM_SYS_PLUGGER_H
#define ELM_SYS_PLUGGER_H

#include <elm/macros.h>
#include <elm/PreIterator.h>
#include <elm/sys/Plugin.h>
#include <elm/data/Range.h>
#include <elm/data/Vector.h>
#include <elm/sys/Directory.h>
#include <elm/sys/Path.h>
#include <elm/util/ErrorHandler.h>

namespace elm { namespace sys {

// Plugger class
class Plugger: public ErrorBase {
	friend class Plugin;
	friend class PluginManager;
public:
	typedef enum error_t {
		OK = 0,
		NO_PLUGIN,
		NO_HOOK,
		BAD_VERSION,
		NO_MAGIC,
		BAD_PLUGIN,
		MISSING_DEP
	} error_t;

public:
	Plugger(CString hook, const Version& plugger_version, String paths = "*");
	virtual ~Plugger(void);
	void addPath(String path);
	void removePath(String path);
	void resetPaths(void);
	Plugin *plug(const string& path);
	Plugin *plugFile(sys::Path path);
	inline String hook(void) const { return _hook; }
	string getLastError(void);
	inline bool quiet(void) const { return _quiet; }
	inline void setQuiet(bool quiet) { _quiet = quiet; }
	bool isPlugged(string name) const;

	// deprecated
	virtual void onError(String message);
	virtual void onWarning(String message);
	error_t lastError(void);
	String lastErrorMessage(void);

	// Iterator class
	class Iter: public PreIterator<Iter, String> {
	public:
		Iter(Plugger& _plugger, bool end = false);
		~Iter(void);
		bool ended(void) const;
		String item(void) const;
		void next(void);
		Plugin *plug(void) const;
		Path path(void) const;
	private:
		Plugger& plugger;
		Vector<Plugin *>& statics;
		int i, c;
		int _path;
		Directory::Iter *file;
		void go(void);
	};

	// iterator access
	inline const Vector<string>& paths() const { return _paths; }
	inline Range<Iter> available() { return range(Iter(*this), Iter(*this, true)); }
	inline const Vector<Plugin *>& plugged() const { return plugins; }

	// deprecated
	class PathIterator: public Vector<string>::Iter {
	public:
		inline PathIterator(const Plugger& plugger): Vector<string>::Iter(plugger._paths) { }
		inline PathIterator(const PathIterator& iter): Vector<string>::Iter(iter) { }
	};

private:

	static Vector<Plugger *> pluggers;
	CString _hook;
	Version per_vers;
	Vector<Plugin *> plugins;
	Vector<String> _paths;
	error_t err;
	bool _quiet;
	static void leave(Plugin *plugin);
	Plugin *plug(Plugin *plugin, void *handle);
	inline Vector<Plugin *>& statics(void) { return Plugin::static_plugins; }
	void onError(error_level_t level, const string& message);
	Plugin *lookELD(const Path& path, error_t& err, Vector<Plugin *>& deps);

	// portability functions
	static void *link(sys::Path lib);
	static void unlink(void *handle);
	static void *lookSymbol(void *handle, cstring hook);
	static void *lookLibrary(sys::Path lib, Vector<string> rpath);
	static string error(void);
};

} } // elm::sys

#endif // ELM_SYS_PLUGGER_H
