#include "cfgGen.h"

using namespace otawa;
using namespace elm::option;

cfgGen::cfgGen(const std::string scriptPath, const std::string entryFunction, const std::string elfPath) {
        this->scriptPath = elm::string(scriptPath.c_str());
        this->entryFunction = elm::string(entryFunction.c_str());
        this->elfPath = elm::string(elfPath.c_str());
        
}

/**
 * [HRG01]
 * @brief Generates the workspace.
 *
 * This function generates the workspace using otawa from the architecture and the C file inserted in the input
 *
 */
otawa::WorkSpace *cfgGen::workspaceGenerator() {
    elm::Path path = this->scriptPath;
    elm::string script = this->scriptPath;
    elm::string entry = this->entryFunction;
    PropList props;

    //[HRI08]
    if (!path.exists() && !path.isAbsolute()) {
        elm::Path file = script;
        if (file.extension() != "osx")
            file = file.setExtension("osx");
        bool found = false;
        elm::string paths = elm::string(OSX_PATH);
        for (elm::Path::PathIter p(paths); p(); p++) {
            path = elm::Path(*p) / file;
            if (path.exists()) {
                found = true;
                break;
            }
        }
        if (!found)
            printError("Architecture description file not found");
    }
    else printError("Architecture description file not found");
    TASK_ENTRY(props) = entry;
    script::PATH(props) = path;
    script::Script *scr = new script::Script();
    ws = MANAGER.load(this->elfPath, props);
    ws->run(scr, props);
    return ws;
}

/**
 * [HRG01]
 * @brief Calculates the execution time of a basic block.
 *
 * This function calculates the execution time of a basic block.
 * It computes the high times of the block.
 *
 * @param b The Block pointer.
 * @return The execution time of the basic block.
 */
uint32_t cfgGen::blockTime(otawa::Block *b) {
    ilp::System *sys;
    sys = ipet::SYSTEM(ws);
    BasicBlock::basic_preds_t ps;
    b->toBasic()->basicPreds(ps);

    ot::time t2 = 0;
    ot::time t1;
    std::vector<int> time;

    for (auto p : ps){
        if(!b->isCall())
            if (p.snd->hasProp(otawa::etime::LTS_TIME)) {
                // compute low time
                int lt = otawa::etime::LTS_TIME(p.snd);
                //if (t2 > lt) t2 = lt;
                time.push_back(lt);
            }
    }
    int sum = std::accumulate(time.begin(), time.end(), 0);

    t2 = sum / time.size();
    t1 = ipet::TIME(b);

    if (t2 > 0)
        return t2;
    else if (t1 > 0)
        return t1;
    else
        return 1;
}

/**
 * [HRG04]
 * [HRG05]
 * @brief Converts the Control Flow Graph (CFG) information to a matrix.
 *
 * This function converts the CFG information to a matrix and sets the conversion time for each block.
 * It also sets the function name for each block that is a call.
 *
 */
std::set<CfgMatrix> cfgGen::cfg2Matrix() {

    std::set<CfgMatrix> mySet;
    string dir = ws->workDir();
    std::string s = dir.asSysString();
    size_t lastSlashPos = s.find_last_of('/');

    std::string result = s.substr(0, lastSlashPos);
    for (auto g : **otawa::INVOLVED_CFGS(ws)) {
        CfgMatrix *cfgM = new CfgMatrix();
        cfgM->setMyFunc(g->name().asSysString());
        int calls = 0;

        for (auto v : *g) {
            if (v->isCall()) {
                SynthBlock *sb = v->toSynth();
                cfgM->setFuncCallName(v->index(), sb->callee()->name().asSysString());
            }

            for (auto w : SUCCS(v)) {
                if (w->isBasic()) {
                    ot::time time = blockTime(w);
                    if (time > 0)
                        cfgM->setConv(v->index(), w->index(), time);
                    else
                        cfgM->setConv(v->index(), w->index(), 1);
                } 
                else if (w->isCall()) {
                    calls++;
                    std::string funcn = w->toSynth()->callee()->name().asSysString();
                    cfgM->setFuncCallName(w->index(), funcn);
                    cfgM->setConv(v->index(), w->index(), cfgM->getBlockNameHash(w->index()));
                }
            }

            if (LOOP_HEADER(v)) {
                
                std::unordered_set<int> exclusionSet;
                for (auto e : ENTRY_EDGES(v)) {
                    exclusionSet.insert(e->source()->index());
                }
                for (auto e : BACK_EDGES(v)) {
                    if (exclusionSet.find(e->source()->index()) == exclusionSet.end()) {
                        ot::time time = blockTime(e->sink());
                        cfgM->setLoop(e->source()->index(), v->index(), time);
                        if (MAX_ITERATION(v) > 0) cfgM->setIteration(v->index(), MAX_ITERATION(v));
                    }
                }
            }
        }
        
        cfgM->setPriority(calls);
        mySet.insert(*cfgM);
        cfgM->exportDots(g->name().asSysString());
    }
    
    return mySet;
}