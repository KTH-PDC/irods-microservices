// libmsiGetOpenDataObjPath.cpp - iRODS microservice to get opened data object path
// Author: Ilari Korhonen, KTH Royal Institute of Technology

// std C++ headers
#include <cstring>

// iRODS headers
#include "apiHeaderAll.h"
#include "msParam.h"
#include "irods_ms_plugin.hpp"
#include "irods_re_structs.hpp"
#include "rodsErrorTable.h"
#include "irods_error.hpp"
#include "objDesc.hpp"
#include "fileOpr.hpp"

// persistent L1 object descriptor table
extern l1desc_t L1desc[NUM_L1_DESC];


// microservice entry point
int msiGetOpenDataObjPath(msParam_t *in, msParam_t *out, ruleExecInfo_t *rei)
{
    int l1descInx = 0;

    // we need an input param
    if (!in)
	return (SYS_INVALID_INPUT_PARAM);
    
    // and it needs to be a positive integer
    if (strcmp(in->type, INT_MS_T) || (l1descInx = parseMspForPosInt(in)) < 0)
	return (SYS_INVALID_INPUT_PARAM);

    // proceed, if sanity
    if (l1descInx < NUM_L1_DESC - 1)
    {
	l1desc_t *descPtr = L1desc + l1descInx;

	// for a valid descriptor there is an object path
	if (descPtr->inuseFlag == FD_INUSE)
            fillStrInMsParam(out, (const char*)descPtr->dataObjInp->objPath);

	else
	    return (SYS_API_INPUT_ERR);
    }

    else
	return (SYS_INVALID_INPUT_PARAM);

    return (0);
}

extern "C" { 
    
    irods::ms_table_entry* plugin_factory()
    {
	// ms table entry for 2 params
	irods::ms_table_entry* msvc = new irods::ms_table_entry(2);
	
	msvc->add_operation<msParam_t*, 
			    msParam_t*, 
			    ruleExecInfo_t*>("msitemplate",
					     std::function<int(msParam_t*,
							       msParam_t*,
							       ruleExecInfo_t*)>
					     (msiGetOpenDataObjPath));
	
	return (msvc);
    }
}
