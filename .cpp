/*
 *
 */

#include <otawa/proc/ProcessorPlugin.h>

namespace autogen_files {namespace  {

using namespace elm;
using namespace otawa;

class Plugin: public ProcessorPlugin {
public:
    typedef elm::genstruct::Table<AbstractRegistration * > procs_t;

    Plugin(void): ProcessorPlugin("autogen_files::", Version(1, 0, 0), OTAWA_PROC_VERSION) { }
    virtual procs_t& processors (void) const { return procs_t::EMPTY; };
};

} } 

autogen_files::::Plugin autogen_files__plugin;
ELM_PLUGIN(autogen_files__plugin, OTAWA_PROC_HOOK);

