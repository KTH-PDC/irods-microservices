# example-dynpeps.re - example iRODS rulebase for using the pep_api_data_obj-* dynamic PEPs
#
# Author: Ilari Korhonen, KTH Royal Institute of Technology
# Copyright (C) 2021 KTH Royal Institute of Technology. All rights reserved.


pep_api_data_obj_put_post(*INSTANCE_NAME, *COMM, *DATAOBJINP, *BUFFER, *PORTAL_OPR_OUT)
{
	auditLog("api_data_obj_put_post", *DATAOBJINP);

	*objPath = *DATAOBJINP.obj_path
	doASyncChksumAndRepl(*objPath);
}


pep_api_data_obj_get_post(*INSTANCE_NAME, *COMM, *DATAOBJINP, *BUFFER, *PORTAL_OPR_OUT)
{
	auditLog("api_data_obj_get_post", *DATAOBJINP);
}


pep_api_data_obj_create_post(*INSTANCE_NAME, *COMM, *DATAOBJINP)
{
	auditLog("api_data_obj_create_post", *DATAOBJINP);

	*objPath = *DATAOBJINP.obj_path;
	msiGetOpenDataObjL1Inx(*objPath, *l1descInx);

	msiAddKeyVal(temporaryStorage, str(*l1descInx), *objPath);
}


pep_api_data_obj_open_post(*INSTANCE_NAME, *COMM, *DATAOBJINP)
{
	auditLog("api_data_obj_open_post", *DATAOBJINP);

	*objPath = *DATAOBJINP.obj_path;
	*openFlags = *DATAOBJINP.open_flags;

	if (int(*openFlags) != 0)
	{
		msiGetOpenDataObjL1Inx(*objPath, *l1descInx);
		msiAddKeyVal(temporaryStorage, str(*l1descInx), *objPath);
	}
}


pep_api_data_obj_close_post(*INSTANCE_NAME, *COMM, *DATAOBJCLOSEINP)
{
	auditLog("api_data_obj_close_post", *DATAOBJCLOSEINP);

	*l1descInx = *DATAOBJCLOSEINP.l1descInx;

	if (errorcode(msiGetValByKey(temporaryStorage, str(*l1descInx), *objPath)) == 0)
	{
		if (strlen(*objPath) > 0 && *objPath != "null")
		{
			doASyncChksumAndRepl(*objPath);
			temporaryStorage.*l1descInx = "null";
		}
	}
}
