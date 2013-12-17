#include "J_Face_Stream_Processor.h"
//
#include "J_PXC_Face_Module.h"
//
#include "J_Camera_Manager.h"

#ifdef _DEBUG
//
#include <iostream>
//
#include <sstream>
//
using std::cerr; using std::endl; using std::ostringstream;
#endif



namespace jomike{

static Instance_Pointer<J_Camera_Manager> s_camera_manager;
static J_PXC_Error_Manager s_pxc_error;

J_Face_Stream_Processor::J_Face_Stream_Processor(J_PXC_Stream_Shared_t i_stream){
	s_camera_manager->get_pxc_session().CreateImpl<J_Face_Module>(&M_j_face_module);
	M_face_data = new J_PXC_Face::J_Face_Data;
}

void J_Face_Stream_Processor::push_data(){
	//cerr << "\nNot Implemented: J_Face_Stream_Processor::push_data()";
	//assert(!"Not Implemented");
}

void J_Face_Stream_Processor::derived_process_image_async(PXC_Image* i_image
	, PXC_SyncPoint* i_syncpoint){
	s_pxc_error = M_j_face_module->ProcessImageAsync(i_image, M_face_data,i_syncpoint);

}

J_Face_Stream_Processor::~J_Face_Stream_Processor(){
	J_PXC_RELEASE(M_j_face_module);
}

void J_Face_Stream_Processor::derived_finish_processing(){
	//cerr << "\nNot Implemented: J_Face_Stream_Processor::derived_finish_processing()";
}

}



