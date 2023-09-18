#include "elm/option/StringList.h"
#include "elm/sys/System.h"

#include "otawa/app/Application.h"
#include "otawa/cfg/features.h"
#include "otawa/ilp/System.h"

#include "otawa/events/features.h"
#include "otawa/hard/Processor.h"
#include "otawa/ilp.h"
#include "otawa/ipet/features.h"
#include "otawa/stats/BBStatCollector.h"

#include "otawa/etime/features.h"
#include "otawa/ipet/WCETComputation.h"

#include "otawa/ipet/BasicObjectFunctionBuilder.h"
#include "otawa/script/Script.h"
#include "otawa/stats/StatInfo.h"
#include "otawa/util/BBRatioDisplayer.h"
#include "otawa/flowfact/FlowFactLoader.h"

#include "CfgMatrix.h"
#include <unordered_set>
#include <vector>

using namespace otawa;
using namespace elm::option;


class Summer: public StatCollector::Collector {
public:
	Summer(void): _init(false), _sum(0), _max(0), _min(0), _cnt(0) { }
	virtual ~Summer(void) { }
	inline int sum(void) const { return _sum; }
	inline int max(void) const { return _max; }
	inline int min(void) const { return _min; }
	inline int count(void) const { return _cnt; }
	virtual void enter(const ContextualStep &step) { }
	virtual void leave(void) { }
	virtual void collect(const Address &address, t::uint32 size, int value, const ContextualPath& ctx) {
		_cnt++;
		_sum += value;
		if(!_init) {
			_init = true;
			_max = value;
			_min = value;
		}
		else {
			if(value > _max)
				_max = value;
			if(value < _min)
				_min = value;
		}

	}
private:
	bool _init;
	int _sum, _max, _min, _cnt;
};

class OWCET: public Application {
public:
	OWCET(void): Application(
		"owcet",
		Version(1, 2, 0),
		"Compute the WCET of task using a processor script (.osx)."
		"H. Cassé <casse@irit.fr>",
		"Copyright (c) IRIT - UPS <casse@irit.fr>"
	),
	params			(ListOption<string>		::Make(*this).cmd("-p")			.cmd("--param").description("parameter passed to the script").argDescription("IDENTIFIER=VALUE")),
	script			(ValueOption<string>	::Make(*this).cmd("-s")			.cmd("--script").description("script used to compute WCET").argDescription("PATH")),
	display_stats	(SwitchOption			::Make(*this).cmd("-S")			.cmd("--display-stats").description("display statistics"))
	{ }

protected:
	virtual void work (const string &entry, PropList &props) {
		// set statistics option
		if(display_stats)
			Processor::COLLECT_STATS(props) = true;

		// any script
		if(!script)
			throw option::OptionException("a script must be given !");

		// fulfill the parameters
		for(int i = 0; i < params.count(); i++) {
			string param = params[i];
			int idx = param.indexOf('=');
			if(idx < 0)
				cerr << "WARNING: argument " << param << " is malformed: VARIABLE=VALUE\n";
			else
				script::PARAM(props).add(pair(param.substring(0, idx), param.substring(idx + 1)));
		}

		Path path = *script;
		if(!path.exists() && !path.isAbsolute()) {
			Path file = *script;
			if(file.extension() != "osx")
				file = file.setExtension("osx");
			bool found = false;
			string paths = MANAGER.buildPaths("../../share/Otawa/scripts", "");
			for(Path::PathIter p(paths); p(); p++) {
				path = Path(*p) / file;
				if(isVerbose())
					cerr << "INFO: looking script in directory " << *p << io::endl;
				if(path.exists()) {
					found = true;
					break;
				}
			}
			if(!found)
				throw elm::option::OptionException(_ << "cannot find script " << *script);
		}
		if(isVerbose())
			cerr << "INFO: using script from " << path << io::endl;

		TASK_ENTRY(props) = entry;
		script::PATH(props) = path;
		
		script::Script *scr = new script::Script();
		workspace()->run(scr, props);

		// display the result
		ot::time wcet = ipet::WCET(workspace());
		if(wcet == -1)
			cerr << "ERROR: no WCET computed (see errors above)." << io::endl;
		else if(scr->version() == 1)
			cout << "WCET[" << entry << "] = " << wcet << " cycles\n";

		// display statistics
		if(wcet >= 0 && display_stats) {
			bool found = false;

			// traverse all stats
			for(StatInfo::Iter stat(workspace()); stat(); stat++) {
				found = true;
				Summer s;
				stat->collect(s);
				int t = stat->total();

				// non-summable stat
				cerr << stat->name() << ": ";
				if(t)
					cerr << "total=" << t << ", ";
				cerr << " avg=" << float(s.sum()) / s.count()
					 << ", max=" << s.max()
					 << ", min=" << s.min() << io::endl;
			}
			// no state message
			if(!found)
				cerr << "No statistics to display.\n";
		}

		cfg2Matrix();
	}

	ot::time blockTime(Block *b) {
		ilp::System *sys;	
		sys = ipet::SYSTEM(workspace());
		BasicBlock::basic_preds_t ps;
		b->toBasic()->basicPreds(ps);
		ot::time t2 = 0;

		for(auto p: ps)
			if(p.snd->hasProp(otawa::etime::LTS_TIME)) {
				
				// compute low time
				int lt = otawa::etime::LTS_TIME(p.snd);
				int lc = sys->valueOf(ipet::VAR(p.snd));
				t2 += lc * lt;

				// compute high times
				for(auto c: otawa::etime::HTS_CONFIG.all(p.snd)) {
					int ht = c.fst;
					int hc = sys->valueOf(c.snd);
					t2 += hc * ht;
				}
			}
		ot::time t1 =  ipet::TIME(b);
		if (t2 > 0) return t2;
		else if (t1 > 0) return t1;
		else return 1;
	}


	void cfg2Matrix() {

		string dir = workspace()->workDir();
		std::string s = dir.asSysString();
		size_t lastSlashPos = s.find_last_of('/');
		std::string result = s.substr(0, lastSlashPos);
		
		for(auto g: **otawa::INVOLVED_CFGS(workspace())) {
			CfgMatrix cfgM;
			
			cout << "Function " << g << io::endl;

			for(auto v: *g) {
				//cout << "Bloco: " << v->index() << " ";
				
				/*if(v->isBasic()){	
					cout << "T: " << blockTime(v) << io::endl;
				}*/

				/*if(v->isEntry()){
					cout << "Bloco: " << "Entrada" << " ";
					for(auto w: SUCCS(v)) cout << "Bloco: " << v-> << " ";

				}
				else if(v->isExit()) {
					cout << "Bloco: " << "Saida" << " ";
				}*/

				for(auto w: SUCCS(v)) {
					if(w->isBasic()){
						ot::time time = blockTime(w);
						if(time > 0) cfgM.addConv(v->index(), w->index(), time, w->address().offset());
						else cfgM.addConv(v->index(), w->index(), 1, 0);
					}
				}

				if(LOOP_HEADER(v)) {
					std::unordered_set<int> exclusionSet;
					for(auto e: ENTRY_EDGES(v)) {	
						exclusionSet.insert(e->source()->index());
					}
					for(auto e: BACK_EDGES(v)){
						if(exclusionSet.find(e->source()->index()) == exclusionSet.end()){
							ot::time time = blockTime(e->sink());
							cfgM.addLoop(e->source()->index(), v->index(), time, v->address().offset());
							if(MAX_ITERATION(v) > 0) cfgM.addObrigatoryPass(v->index(), MAX_ITERATION(v));
						}
					}
				}
			}

			cfgM.printCycles();
			cfgM.printIterations();
			cfgM.print_all_cycles();
			//cfgM.printOuts();
			cfgM.exportCSVs();
			cfgM.exportDots();
		}

	}



private:
	ListOption<string> params;
	ValueOption<string> script;
	SwitchOption display_stats;
	string bin, task;
};

OTAWA_RUN(OWCET);
