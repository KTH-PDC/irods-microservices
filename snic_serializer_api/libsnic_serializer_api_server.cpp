// libsnic_serializer_api_server.cpp - iRODS API endpoint (dummy) to load new serializers into the server
// Author: Ilari Korhonen, KTH Royal Institute of Technology

// std C++ headers
#include <string>

// boost headers
#include "boost/lexical_cast.hpp"

// iRODS headers
#include "apiHandler.hpp"
#include "irods_server_api_call.hpp"
#include "irods_re_serialization.hpp"

// iRODS type openedDataObjInp_t
#include "dataObjInpOut.h"


// we only do the server-side stuff
#ifdef RODS_SERVER

// define API number (my phone number)
#define SNIC_SERIALIZER_APN 87907817

// packing info
#define OpenedDataObjInp_PI "int _this; str _that[64];"


// serializer callback for a pointer
static irods::error serialize_openedDataObjInp_ptr(boost::any param,
						   irods::re_serialization::serialized_parameter_t &out)
{
    return (SUCCESS());
}

// serializer callback for a handle
static irods::error serialize_openedDataObjInp_ptr_ptr(boost::any param,
						       irods::re_serialization::serialized_parameter_t &out) 
{
    return (SUCCESS());
}

// dummy API endpoint implementation
int rs_snic_serializer_dummy(rsComm_t *comm, openedDataObjInp_t *in_param, int *out)
{
    int status = 0;

    rodsLog(LOG_NOTICE, "SNIC Serializer API - dummy endpoint invoked!");
    *out = status;

    return (status);
}

// calling convention from template expansion
int call_openedDataObjInp_t_int(irods::api_entry *api, rsComm_t *comm,
				openedDataObjInp_t *in, int *out)
{
    return api->call_handler<openedDataObjInp_t*,int*>(comm, in, out);
}

extern "C" {

    // factory for API table entry
    irods::api_entry* plugin_factory(const std::string &inst_name,
				     const std::string &context)
    {
	irods::apidef_t dummy_api = {SNIC_SERIALIZER_APN, RODS_API_VERSION,
				     NO_USER_AUTH, NO_USER_AUTH,
				     "openedDataObjInp_PI", 0, "INT_PI", 0,
				     std::function<int(rsComm_t*,openedDataObjInp_t*,int*)>(rs_snic_serializer_dummy),
				     "api_snic_serializer_dummy", 0, (funcPtr)call_openedDataObjInp_t_int};
	
	irods::api_entry *ptr = NULL;
	return (ptr);
    }

}; // extern "C"

#endif // RODS_SERVER
