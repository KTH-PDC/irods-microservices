// libsnic_serializer_api_server.cpp - iRODS API endpoint (dummy) to load new serializers into the server
// Author: Ilari Korhonen, KTH Royal Institute of Technology

// std C++ headers
#include <string>

// iRODS headers
#include "apiHandler.hpp"
#include "rodsPackInstruct.h"
#include "irods_server_api_call.hpp"
#include "irods_re_serialization.hpp"

// iRODS type openedDataObjInp_t
#include "dataObjInpOut.h"

// iRODS type bytesBuf_t
#include "rodsDef.h"

// we only do the server-side stuff
#ifdef RODS_SERVER

// define API number (my phone number)
#define SNIC_SERIALIZER_APN 87907817

// take out an iRODS namespace
namespace rs = irods::re_serialization;


// serializer callback for an openedDataObjInp_t pointer
static irods::error serialize_openedDataObjInp_ptr(boost::any param,
						   rs::serialized_parameter_t &out)
{
    try {
        openedDataObjInp_t *ptr = boost::any_cast<openedDataObjInp_t*>(param);

	// for a valid ptr, we serialize the immediate properties + keyValPair
        if (ptr)
	{
            out["l1descInx"] = std::to_string(ptr->l1descInx);
            out["len"] = std::to_string(ptr->len);
	    out["whence"] = std::to_string(ptr->whence);
	    out["oprType"] = std::to_string(ptr->oprType);
	    out["offset"] = std::to_string(ptr->offset);
	    out["bytesWritten"] = std::to_string(ptr->bytesWritten);

	    rs::serialize_parameter(ptr->condInput, out);
        }

        else
            out["null_value"] = "null_value";
    }
    catch (std::exception &e) {
        return ERROR(INVALID_ANY_CAST, "failed to cast openedDataObjInp_t ptr");
    }

    return (SUCCESS());
}

// serializer callback for a double pointer of openedDataObjInp_t
static irods::error serialize_openedDataObjInp_ptr_ptr(boost::any param,
						       rs::serialized_parameter_t &out) 
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
	return (ERROR(INVALID_ANY_CAST, "failed to cast openedDataObjInp_t ptr ptr"));
    }

    return (SUCCESS());
}

// serializer callback for a pointer of bytesBuf_t
static irods::error serialize_bytesBuf_ptr(boost::any param,
                                           rs::serialized_parameter_t &out)
{
    try {
        bytesBuf_t *ptr = boost::any_cast<bytesBuf_t*>(param);

	// for a valid ptr, we serialize the length and content
        if (ptr)
	{
            out["len"] = std::to_string(ptr->len);
            out["buf"] = std::string((char*)ptr->buf);
        }
        else {
            out["null_value"] = "null_value";
        }
    }
    catch (std::exception &e) {
	return (ERROR(INVALID_ANY_CAST, "failed to cast bytesBuf_t ptr"));
    }

    return SUCCESS();
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

	rs::add_operation(typeid(openedDataObjInp_t*), serialize_openedDataObjInp_ptr);
	rs::add_operation(typeid(openedDataObjInp_t**), serialize_openedDataObjInp_ptr_ptr);

        rs::add_operation(typeid(bytesBuf_t*), serialize_bytesBuf_ptr);

	dummy_ptr->in_pack_key = "OpenedDataObjInp_PI";
        dummy_ptr->in_pack_value = OpenedDataObjInp_PI;

        dummy_ptr->out_pack_key = "INT_PI";
        dummy_ptr->out_pack_value = INT_PI;

	return (dummy_ptr);
    }

}; // extern "C"

#endif // RODS_SERVER
