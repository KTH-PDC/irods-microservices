// libmsiGetOpenDataObjL1Inx.cpp - iRODS microservice to extract open data object L1 index
// Author: Ilari Korhonen, KTH Royal Institute of Technology

// std C++ headers
#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <exception>

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
int msiGetOpenDataObjL1Inx(msParam_t *in, msParam_t *out, ruleExecInfo_t *rei)
{
    // we take one param in
    if (!in)
	return (SYS_INVALID_INPUT_PARAM);

    // and it needs to be a string
    if (strcmp(in->type, STR_MS_T))
	return (SYS_INVALID_INPUT_PARAM);
    
    const char *objPath = (const char*)in->inOutStruct;
    int l1descInx = 0;
    
    for (const l1desc_t &l1 : L1desc)
    {
	// for a valid descriptor, if the path matches...
	if (l1.inuseFlag == FD_INUSE)
	{
	    if (!strcmp(l1.dataObjInp->objPath, objPath))
		l1descInx = &l1 - L1desc;
	}
    }

    // we send out the found index (or 0)
    fillIntInMsParam(out, l1descInx);
    return (0);
}

extern "C" { 
    
    irods::ms_table_entry* plugin_factory()
    {
	// ms table entry for 2 params
	irods::ms_table_entry* msvc = new irods::ms_table_entry(2);
	
	msvc->add_operation<msParam_t*, 
			    msParam_t*, 
			    ruleExecInfo_t*>("msiGetOpenDataObjL1Inx",
					     std::function<int(msParam_t*,
							       msParam_t*,
							       ruleExecInfo_t*)>
					     (msiGetOpenDataObjL1Inx));
	
	return (msvc);
    }
}
