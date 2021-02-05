// libsnic_serializer_api_server.cpp - iRODS API endpoint (dummy) to load new serializers into the server
// Author: Ilari Korhonen, KTH Royal Institute of Technology

// std C++ headers
#include <string>

// boost headers
#include "boost/lexical_cast.hpp"

// iRODS headers
#include "apiHandler.hpp"
#include "rodsPackInstruct.h"
#include "irods_server_api_call.hpp"
#include "irods_re_serialization.hpp"

// iRODS type openedDataObjInp_t
#include "dataObjInpOut.h"


// we only do the server-side stuff
#ifdef RODS_SERVER

// define API number (my phone number)
#define SNIC_SERIALIZER_APN 87907817


// serializer callback for a pointer
static irods::error serialize_openedDataObjInp_ptr(boost::any param,
						   irods::re_serialization::serialized_parameter_t &out)
{
    try {
        openedDataObjInp_t *ptr = boost::any_cast<openedDataObjInp_t*>(param);

	// for a valid ptr, we serialize all but the keyValPair
        if (ptr)
	{
            out["l1descInx"] = boost::lexical_cast<std::string>(ptr->l1descInx);
            out["len"] = boost::lexical_cast<std::string>(ptr->len);
	    out["whence"] = boost::lexical_cast<std::string>(ptr->whence);
	    out["oprType"] = boost::lexical_cast<std::string>(ptr->oprType);
	    out["offset"] = boost::lexical_cast<std::string>(ptr->offset);
	    out["bytesWritten"] = boost::lexical_cast<std::string>(ptr->bytesWritten);
        }

        else
            out["null_value"] = "null_value";
    }
    catch (std::exception &e) {
        return ERROR(INVALID_ANY_CAST, "failed to cast openedDataObjInp_t pointer");
    }

    return (SUCCESS());
}

// serializer callback for a handle
static irods::error serialize_openedDataObjInp_ptr_ptr(boost::any param,
						       irods::re_serialization::serialized_parameter_t &out) 
{
    try {
	openedDataObjInp_t **ptr = boost::any_cast<openedDataObjInp_t**>(param);

	// if we can, dereference and serialize
	if (ptr && *ptr)
	    serialize_openedDataObjInp_ptr(*ptr, out);

	else
	    out["null_value"] = "null_value";
    }
    catch (std::exception &e) {
	return (ERROR(INVALID_ANY_CAST, "failed to cast openedDataObjInp_t handle"));
    }

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
	
	irods::api_entry *dummy_ptr = new irods::api_entry(dummy_api);

	irods::re_serialization::add_operation(typeid(openedDataObjInp_t*), serialize_openedDataObjInp_ptr);
	irods::re_serialization::add_operation(typeid(openedDataObjInp_t**), serialize_openedDataObjInp_ptr_ptr);

	dummy_ptr->in_pack_key = "OpenedDataObjInp_PI";
        dummy_ptr->in_pack_value = OpenedDataObjInp_PI;

        dummy_ptr->out_pack_key = "INT_PI";
        dummy_ptr->out_pack_value = INT_PI;

	return (dummy_ptr);
    }

}; // extern "C"

#endif // RODS_SERVER
