#include "J_PXC_Face_Module.h"
//
#include "service/pxcschedulerservice.h"
//
#include "pxcscheduler.h"

namespace jomike{

static J_PXC_Error_Manager s_pxc_error;


class J_Face_Module_Processing_Callback : public PXCSchedulerService::Callback{
public:
	J_Face_Module_Processing_Callback(J_Face_Module_Impl* const i_ptr);
	void PXCAPI Run(pxcStatus)override;
	void PXCAPI Release()override;
	void* PXCAPI DynamicCast(pxcUID cuid)override;
private:
	J_Face_Module_Impl* M_module;
};

J_Face_Module_Processing_Callback::J_Face_Module_Processing_Callback(J_Face_Module_Impl* const i_ptr){
	M_module = i_ptr;
}

void PXCAPI J_Face_Module_Processing_Callback::Release(){
	free(this);
}

void PXCAPI J_Face_Module_Processing_Callback::Run(pxcStatus i_status){
	s_pxc_error = i_status;
	NO_PXC_ERROR;
	M_module->do_process();
}

void* PXCAPI J_Face_Module_Processing_Callback::DynamicCast(pxcUID ){
	return nullptr;
}

pxcStatus PXCAPI J_Face_Module_Impl::CreateInstance(PXCSession* i_session
	, PXCScheduler*, PXCAccelerator*, PXCSessionService::DLLExportTable*
	, pxcUID , PXCBase **i_instance){
	*i_instance = new J_Face_Module_Impl(i_session);

	return PXC_STATUS_NO_ERROR;
}

J_Face_Module_Impl::J_Face_Module_Impl(PXC_Session i_session){
	s_pxc_error = i_session->CreateImpl<PXCFaceAnalysis>(&M_face_module); 
	s_pxc_error = i_session->CreateScheduler(&M_scheduler);
	M_scheduler_service = M_scheduler->DynamicCast<PXCSchedulerService>();
	assert(M_scheduler_service);
}

pxcStatus PXCAPI J_Face_Module_Impl::ProcessImageAsync(PXC_Image* i_image, J_PXC_Face::J_Face_Data* i_data, PXC_SyncPoint* i_syncpoint){
	s_pxc_error = M_scheduler_service->CreateSyncPoint(0, nullptr, i_syncpoint);

	M_processing_callback = M_processing_callback ? M_processing_callback : new J_Face_Module_Processing_Callback(this);
	M_ready_flag = false;
	M_output_data = i_data;
	M_syncpoint = *i_syncpoint;
	auto syncpoint_service = M_syncpoint->DynamicCast<PXCSchedulerService::SyncPointService>();

	void* outputs[] = {M_output_data};
	s_pxc_error = M_scheduler_service->MarkOutputs(1, outputs, PXC_STATUS_EXEC_INPROGRESS);
	syncpoint_service->AddRef();
	s_pxc_error = syncpoint_service->SignalSyncPoint(PXC_STATUS_EXEC_INPROGRESS);

	
	void * inputs[] = {i_image};
	s_pxc_error = M_scheduler_service->RequestInputs(1, inputs, M_processing_callback);
	NO_PXC_ERROR;
	return PXC_STATUS_NO_ERROR;
}

void J_Face_Module_Impl::do_process(){
	update_syncpoint_finished();
}

void J_Face_Module_Impl::update_syncpoint_finished(){
	void* outputs[] = {M_output_data};
	int num_outputs = sizeof(outputs) / sizeof(void*);
	s_pxc_error = M_scheduler_service->MarkOutputs(num_outputs, outputs, PXC_STATUS_NO_ERROR);
	auto syncpoint_service = M_syncpoint->DynamicCast<PXCSchedulerService::SyncPointService>();
	s_pxc_error = syncpoint_service->SignalSyncPoint(PXC_STATUS_NO_ERROR);
	syncpoint_service->Release();
	NO_PXC_ERROR;
}


}



