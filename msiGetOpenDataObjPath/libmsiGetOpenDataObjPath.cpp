// libmsiGetOpenDataObjPath.cpp - iRODS microservice to get opened data object path
// Author: Ilari Korhonen, KTH Royal Institute of Technology

// std C++ headers
#include <string>
#include <cstring>
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
int msiGetOpenDataObjPath(msParam_t *in, msParam_t *out, ruleExecInfo_t *rei)
{
    // we need an input param
    if (!in)
	return (SYS_INVALID_INPUT_PARAM);
    
    // and it needs to be a keyvalpair 
    if (strcmp(in->type, KeyValPair_MS_T))
	return (SYS_INVALID_INPUT_PARAM);
    
    char *val = getValByKey((keyValPair_t*)in->inOutStruct, "l1descInx");

    if (val == nullptr)
        return UNMATCHED_KEY_OR_INDEX;

    try {
	int l1descInx = std::stoi(val);
	l1desc_t *descPtr = L1desc + l1descInx;

	// for a valid descriptor there is an object path
	if (descPtr->inuseFlag == FD_INUSE)
	{
	    std::string objPath = descPtr->dataObjInp->objPath;
	    fillStrInMsParam(out, objPath.c_str());
	}

	else
	    return (SYS_INTERNAL_NULL_INPUT_ERR);
    }
    catch (std::exception &e)
    {
	return (SYS_UNKNOWN_ERROR);
    }

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
