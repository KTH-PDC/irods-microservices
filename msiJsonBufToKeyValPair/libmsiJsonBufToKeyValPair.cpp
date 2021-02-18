// std C++ headers
#include <string>
#include <iostream>
#include <vector>

// iRODS headers
#include "apiHeaderAll.h"
#include "msParam.h"
#include "irods_ms_plugin.hpp"


// our microservice entry point function
int msiJsonBufToKeyValPair(msParam_t *in, msParam_t *out, ruleExecInfo_t *rei)
{
    // we take one param in
    if (!in)
	return SYS_INVALID_INPUT_PARAM;
    
    return 0;
}


// the factory function has a pure C interface
extern "C" {
    
    // is called from the dynamic object by symbol name
    irods::ms_table_entry* plugin_factory()
    {
        // allocate a new ms table entry object (for 2 params)
        irods::ms_table_entry* msvc = new irods::ms_table_entry(2);
        
        // we configure the entry point into the ms table
        msvc->add_operation<msParam_t*, msParam_t*, ruleExecInfo_t*>("msiJsonBufToKeyValPair",
                                                                     std::function<int(
                                                                         msParam_t*,
                                                                         msParam_t*,
                                                                         ruleExecInfo_t*)>
                                                                     (msiJsonBufToKeyValPair));
        // the caller adds into the ms table
        return (msvc);
    }
}
