#include "J_PXC_Module.h"

#include "J_PXC_Fwd_Decl.h"

#include "pxcsession.h"
//
#include "service/pxcsessionservice.h"
//
#include "J_PXC_Face_Module.h"


namespace jomike{
static J_PXC_Error_Manager s_pxc_error;

pxcStatus load_j_pxc_modules(PXC_Session i_session){
	static bool s_modules_loaded = false;
	if(s_modules_loaded){
		return PXC_STATUS_NO_ERROR;
	}

	pxcStatus error = i_session->DynamicCast<PXCSessionService>()->LoadImpl(&g_j_pxc_face_dll_table);
	s_pxc_error = error;
	NO_PXC_ERROR;

	s_modules_loaded = true;
	return error;
}


PXCSessionService::DLLExportTable g_j_pxc_face_dll_table = {
	0,
	J_Face_Module_Impl::CreateInstance,
	PXCSessionService::SUID_DLL_EXPORT_TABLE,
	{
		PXCSession::IMPL_GROUP_USER, /* group */
		PXCSession::IMPL_SUBGROUP_FACE_ANALYSIS, /* subgroup */
		0, /* algorithm */
		PXC_UID('J', 'F', 'P', 'M'), /* iuid */
		{1, 0}, /* version */
		PXCAccelerator::ACCEL_TYPE_CPU, /* acceleration */
		100, /* merit */
		0x1000, /* vendor */
		{J_Face_Module::CUID, 0, 0, 0},        /* exposed interfaces */
		L"J_Face_Processing_Module",
		{0} /* reserved */
	}
};



}



