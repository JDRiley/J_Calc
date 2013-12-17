#ifndef J_PXC_FACE_MODULE_H
#define J_PXC_FACE_MODULE_H


#include "J_PXC_Fwd_Decl.h"
//
#include "pxcbase.h"
#include "j_pxc_face.h"
#include "service/pxcsessionservice.h"
#include "service/pxcschedulerservice.h"
#include "pxcaccelerator.h"
#include "pxcsession.h"


J_POINTER_DECL(PXCScheduler::SyncPoint, PXC_SyncPoint)
J_POINTER_DECL(PXCScheduler, PXC_Scheduler)
J_POINTER_DECL(PXCSchedulerService, PXC_Scheduler_Service)

namespace jomike{

namespace J_PXC_Face{
struct J_Face_Data{

};
}



class J_Face_Module_Processing_Callback;

class J_Face_Module :public PXCBase{
public:
	PXC_CUID_OVERWRITE(PXC_UID('J', 'F', 'P', 'M'));

	virtual pxcStatus PXCAPI ProcessImageAsync(PXC_Image*, J_PXC_Face::J_Face_Data* i_data, PXC_SyncPoint*) = 0;


private:
};

class J_Face_Module_Impl :public PXCBaseImpl<J_Face_Module> {
public:
	J_Face_Module_Impl(PXC_Session i_session);
	pxcStatus PXCAPI ProcessImageAsync(PXC_Image*, J_PXC_Face::J_Face_Data* i_data, PXC_SyncPoint*)override;
	void do_process();

	void update_syncpoint_finished();


	static pxcStatus PXCAPI CreateInstance(PXCSession*, PXCScheduler*, PXCAccelerator*
		, PXCSessionService::DLLExportTable*, pxcUID cuid, PXCBase **instance);
private:
	PXC_Face_Module M_face_module;
	PXC_Scheduler M_scheduler;
	PXC_Scheduler_Service M_scheduler_service;
	J_Face_Module_Processing_Callback* M_processing_callback;
	PXC_Image M_image;
	J_PXC_Face::J_Face_Data* M_output_data;
	PXC_SyncPoint M_syncpoint;
	PXCSchedulerService::SyncPointService* M_syncpoint_service;
#ifdef VS_2013
	bool M_ready_flag = true;
#else
	bool M_ready_flag;
#endif
};

extern PXCSessionService::DLLExportTable  g_j_pxc_face_dll_table;




}

#endif //J_PXC_FACE_MODULE_H

