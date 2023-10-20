#include "wcet_bio.h"


using namespace otawa;
using namespace elm::option;

WCETCalculatorBio::WCETCalculatorBio(const elm::string scriptPath, const elm::string entryFunction, const elm::string elfPath) {
        this->scriptPath = scriptPath;
        this->entryFunction = entryFunction;
        this->elfPath = elfPath;
}

void WCETCalculatorBio::calculateWCET() {
        elm::Path path = this->scriptPath;
        elm::string script = this->scriptPath;
        elm::string entry = this->entryFunction;
        PropList props;

        if (!path.exists() && !path.isAbsolute()) {
            elm::Path file = script;
            if (file.extension() != "osx")
                file = file.setExtension("osx");
            bool found = false;
            elm::string paths = MANAGER.buildPaths("../../share/Otawa/scripts", "");
            for (elm::Path::PathIter p(paths); p(); p++) {
                path = elm::Path(*p) / file;
                if (path.exists()) {
                    found = true;
                    break;
                }
            }
            if (!found)
                throw elm::option::OptionException(_ << "cannot find script " << script);
        }
        TASK_ENTRY(props) = entry;
        script::PATH(props) = path;
        script::Script *scr = new script::Script();
        otawa::WorkSpace *ws = MANAGER.load(this->elfPath, props);
        ws->run(scr, props);
        cfg2Matrix(ws);
}

uint32_t WCETCalculatorBio::getWCET() {
        return -1;
}

uint32_t WCETCalculatorBio::blockTime(WorkSpace *ws, Block *b) {
    ilp::System *sys;	
    sys = ipet::SYSTEM(ws);
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


void WCETCalculatorBio::cfg2Matrix(WorkSpace *ws) {

    string dir = ws->workDir();
    std::string s = dir.asSysString();
    size_t lastSlashPos = s.find_last_of('/');
    std::string result = s.substr(0, lastSlashPos);
    
    
    for(auto g: **otawa::INVOLVED_CFGS(ws)) {
        CfgMatrix cfgM;
        
        cout << "Function " << g << io::endl;

        for(auto v: *g) {

            if(v->isCall()) {
                SynthBlock *sb = v->toSynth();
                cout << "Bloco: " << v->isSynth() << sb->callee()->name() << io::endl;
                cfgM.setFuncName(v->index(), sb->callee()->name().asSysString());
            }

            for(auto w: SUCCS(v)) {

                if(w->isBasic()){
                    ot::time time = blockTime(ws, w);
                    if(time > 0) cfgM.setConv(v->index(), w->index(), time);
                    else cfgM.setConv(v->index(), w->index(), 1);
                }
                else if(w->isCall()) {
                    std::string funcn = w->toSynth()->callee()->name().asSysString();
                    cfgM.setConv(v->index(), w->index(), cfgM.getBlockNameHash(w->index()));
                }
            }

            if(LOOP_HEADER(v)) {
                std::unordered_set<int> exclusionSet;
                for(auto e: ENTRY_EDGES(v)) {	
                    exclusionSet.insert(e->source()->index());
                }
                for(auto e: BACK_EDGES(v)){
                    if(exclusionSet.find(e->source()->index()) == exclusionSet.end()){
                        ot::time time = blockTime(ws, e->sink());
                        cfgM.setLoop(e->source()->index(), v->index(), time);
                        if(MAX_ITERATION(v) > 0) cfgM.setIteration(v->index(), MAX_ITERATION(v));
                    }
                }
            }
        }

        cfgM.printCycles();
        //cfgM.printFunctions();
        //cfgM.printIterations();
        //cfgM.print_all_cycles();
        //cfgM.printOuts();
        //cfgM.exportCSVs(g->name().asSysString());
        //cfgM.exportDots(g->name().asSysString());
    }

}