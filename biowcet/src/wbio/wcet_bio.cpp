#include "wcet_bio.h"

using namespace otawa;
using namespace elm::option;

/**
 * @brief Constructor for the WCETCalculatorBio class.
 *
 * @param scriptPath The path to the script file.
 * @param entryFunction The name of the entry function.
 * @param elfPath The path to the ELF file.
 */
WCETCalculatorBio::WCETCalculatorBio(const std::string scriptPath, const std::string entryFunction, const std::string elfPath) {
    this->scriptPath = elm::string(scriptPath.c_str());
    this->entryFunction = elm::string(entryFunction.c_str());
    this->elfPath = elm::string(elfPath.c_str());
}

/**
 * @brief Calculates the Worst Case Execution Time (WCET) for the given script, entry function and ELF file.
 *
 * This function loads the ELF file and runs the script to calculate the WCET for the given entry function.
 * It then converts the Control Flow Graph (CFG) information to a matrix.
 *
 * @throws elm::option::OptionException if the script cannot be found.
 */
void WCETCalculatorBio::calculateWCET() {
    elm::Path path = this->scriptPath;
    elm::string script = this->scriptPath;
    elm::string entry = this->entryFunction;
    PropList props;

    // If the script path is not absolute, try to find it in the share/Otawa/scripts directory
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
            printError("Architecture description file not found");
    }
    else printError("Architecture description file not found");

    // Set the entry function and script path properties
    TASK_ENTRY(props) = entry;
    
    script::PATH(props) = path;
    // Load the ELF file and run the script
    script::Script *scr = new script::Script();
    otawa::WorkSpace *ws = MANAGER.load(this->elfPath, props);
    ws->run(scr, props);

    // Convert CFG information to a matrix
    cfg2Matrix(ws);
    while (!pq.empty()) {
        CfgMatrix c = pq.front();
        pq.pop();
        int maxIter = 3;//pow(c.getSize(), 0.5);
        int antNo = 10;
        float rho = 0.1;
        if (c.getPriority() > 0 && !replaceDependencies(&c)) {
            pq.push(c);
        }
        else{
            c.printCycles();
            ACO aco(c, antNo, 0, maxIter, alpha, beta, rho);
            aco.simulate();
            wcet = aco.getResults();
            cfgMap[c.getMyHashName()] = wcet; 
        }
    }

}



/**
 * @brief Calculates the execution time of a basic block.
 *
 * This function calculates the execution time of a basic block using the Integer Linear Programming (ILP) model.
 * It computes the low time and high times of the block and returns the maximum value between them.
 *
 * @param ws The WorkSpace pointer.
 * @param b The Block pointer.
 * @return The execution time of the basic block.
 */
uint32_t WCETCalculatorBio::blockTime(WorkSpace *ws, Block *b) {
    ilp::System *sys;
    sys = ipet::SYSTEM(ws);
    BasicBlock::basic_preds_t ps;
    b->toBasic()->basicPreds(ps);
    ot::time t2 = 0;

    for (auto p : ps)
        if (p.snd->hasProp(otawa::etime::LTS_TIME)) {
            // compute low time
            int lt = otawa::etime::LTS_TIME(p.snd);
            int lc = sys->valueOf(ipet::VAR(p.snd));
            t2 += lc * lt;

            // compute high times
            for (auto c : otawa::etime::HTS_CONFIG.all(p.snd)) {
                int ht = c.fst;
                int hc = sys->valueOf(c.snd);
                t2 += hc * ht;
            }
        }
    ot::time t1 = ipet::TIME(b);
    if (t2 > 0)
        return t2;
    else if (t1 > 0)
        return t1;
    else
        return 1;
}

/**
 * @brief Converts the Control Flow Graph (CFG) information to a matrix.
 *
 * This function converts the CFG information to a matrix and sets the conversion time for each block.
 * It also sets the function name for each block that is a call.
 *
 * @param ws The WorkSpace pointer.
 */
void WCETCalculatorBio::cfg2Matrix(WorkSpace *ws) {

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
                    ot::time time = blockTime(ws, w);
                    //ot::time time2 = ipet::TIME(w->edgeTo(v));
                    //printf("ipet=s %ld, myMethod = %ld\n", time2, time);
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
                        ot::time time = blockTime(ws, e->sink());
                        //ot::time time2 = ipet::TIME(e);
                        //printf("ipet=s %ld, myMethod = %ld\n", time2, time);
                        cfgM->setLoop(e->source()->index(), v->index(), time);
                        if (MAX_ITERATION(v) > 0) cfgM->setIteration(v->index(), MAX_ITERATION(v));
                    }
                }
            }
        }
        cfgM->setPriority(calls);
        //printf("vFunction: %s, deps: %d\n", cfgM->getMyName().c_str(), calls);
        mySet.insert(*cfgM);

        //printf("Printing size: %ld\n", cfgM->getSize());
        //printf("Printing loops: %d\n", cfgM->getLoops());
        //printf("Printing loopbacks: %d\n", cfgM->getLoopBacks());
        //cfgM->findLoopPaths();

        //cfgM->printCycles();
        // cfgM.printFunctions();
        // cfgM.printIterations();
        //cfgM->printAllLoops();
        // cfgM.printOuts();
        //cfgM.exportCSVs(g->name().asSysString());
        cfgM->exportDots(g->name().asSysString());
    }
    
    for (const CfgMatrix& elem : mySet) pq.push(elem);
    

}

/**
 * @brief Replaces the dependencies in the Control Flow Graph (CFG) matrix with their corresponding function names.
 *
 * This function replaces the dependencies in the CFG matrix with their corresponding function names using the cfgMap.
 * It iterates through the matrix and checks if the value is negative, indicating a function call.
 * If it is a function call, it replaces the value with the corresponding function name from the cfgMap.
 *
 * @param c The CfgMatrix pointer.
 */
bool WCETCalculatorBio::replaceDependencies (CfgMatrix *c) {
    for (int i = 0; i< c->getSize(); i++){
        for (int j = 0; j< c->getSize(); j++){
            int value = c->getCycles(i, j);
            if(value < 0) {//isfunc
                if(cfgMap.count(value) > 0) //issolved
                    c->setConv(i, j, cfgMap[value]);
                else return false;
            }
        }
    }
    return true;

}


/**
 * @brief Returns the Worst Case Execution Time (WCET) calculated by the calculateWCET() function.
 *
 * @return The WCET value.
 */
uint32_t WCETCalculatorBio::getWCET() {
    return wcet;
}