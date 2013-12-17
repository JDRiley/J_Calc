#include "J_PXC_Face_Processor.h"
//
#include "J_Camera_Manager.h"

namespace jomike{
static J_PXC_Error_Manager s_pxc_error;
static Instance_Pointer<J_Camera_Manager> s_camera_manager;






J_PXC_Face_Processor::J_PXC_Face_Processor(J_PXC_Stream_Shared_t i_stream){
#ifndef VS_2013
	void default_initialization();
#endif
	s_camera_manager->get_pxc_session().CreateImpl<J_Face_Module>(&M_j_face_module);

	set_parent(i_stream);
	M_landmark_data.clear();
	M_landmark_data.resize(MAX_FACES);

	s_pxc_error = s_camera_manager->get_pxc_session()
		.CreateImpl<PXCFaceAnalysis>(&M_face_module);

	ex_array<PXCFaceAnalysis::ProfileInfo> face_module_profiles_info;

	for(int i = 0;; i++){
		face_module_profiles_info.push_back(zero_set < PXCFaceAnalysis::ProfileInfo>());
		s_pxc_error = M_face_module->QueryProfile(i, &face_module_profiles_info.back());
		if(pxc_status_item_unavailable()){
			pxc_clear_error();
			face_module_profiles_info.pop_back();
			break;
		}

	}
	M_face_profile_info = face_module_profiles_info.front();
	M_face_profile_info.inputs.streams[0].sizeMax.width
		= M_face_profile_info.inputs.streams[0].sizeMin.width = i_stream->width();
	M_face_profile_info.inputs.streams[0].sizeMax.height
		= M_face_profile_info.inputs.streams[0].sizeMin.height = i_stream->height();
	M_face_input_description = M_face_profile_info.inputs.streams[0];
	get_src_stream().device().set_device_capabilities(M_face_profile_info.inputs.devCaps);
	M_face_profile_info.inputs.streams[0] = M_face_input_description;


	M_face_profile_info.iftracking = true;
	
	s_pxc_error = M_face_module->SetProfile(&M_face_profile_info);

	s_pxc_error = M_face_module->QueryProfile(&M_face_profile_info);

	M_landmark_module = M_face_module->DynamicCast<PXCFaceAnalysis::Landmark>();

	assert(M_landmark_module);

	set_to_zero(&M_landmark_profile);
	s_pxc_error = M_landmark_module->QueryProfile(0, &M_landmark_profile);
	M_landmark_profile.labels = PXCFaceAnalysis::Landmark::LABEL_7POINTS;
	s_pxc_error = M_landmark_module->SetProfile(&M_landmark_profile);
	s_pxc_error = M_landmark_module->QueryProfile(&M_landmark_profile);


	M_face_detection_module = M_face_module->DynamicCast<PXCFaceAnalysis::Detection>();
	assert(M_face_detection_module);

	set_to_zero(&M_face_detection_profile);
	s_pxc_error = M_face_detection_module->QueryProfile(0, &M_face_detection_profile);

	M_face_detection_profile.viewAngles = PXCFaceAnalysis::Detection::VIEW_ANGLE_OMNI;
	M_face_detection_profile.responsiveness = 85;

	s_pxc_error = M_face_detection_module->SetProfile(&M_face_detection_profile);
	s_pxc_error = M_face_detection_module->QueryProfile(&M_face_detection_profile);



	NO_PXC_ERROR;
}

void J_PXC_Face_Processor::derived_process_image_async(PXC_Image* i_image
	, PXC_SyncPoint* i_syncpoint){
	s_pxc_error = M_face_module->ProcessImageAsync(i_image
		, i_syncpoint);
}


void J_PXC_Face_Processor::derived_finish_processing(){
	for(int i = 0; i < MAX_FACES; i++){
		int face_id;
		s_pxc_error = M_face_module->QueryFace(i, &face_id);
		if(pxc_status_item_unavailable()){
			pxc_clear_error();
			M_face_data[i].confidence = 0;
			continue;
		}

		M_face_detection_module->QueryData(face_id, &M_face_data[i]);


		auto& landmarks = M_landmark_data[i];
		landmarks.resize(NUM_LANDMARKS);

		for(int b = 0; b < NUM_LANDMARKS; b++){
			pxcU32 landmark_id = b;

			s_pxc_error = M_landmark_module->QueryLandmarkData(face_id
				, PXCFaceAnalysis::Landmark::LABEL_7POINTS, landmark_id, &landmarks[b]);
			if(pxc_status_item_unavailable()){

				landmarks.pop_back();

#ifdef _DEBUG
				j_size_t size = landmarks.size();
				size *= 1;
#endif
				pxc_clear_error();
				break;
			}
		}
		NO_PXC_ERROR;
	}
}

void J_PXC_Face_Processor::push_data(){
	push_face_data();
}

void J_PXC_Face_Processor::push_face_data()const{
	for(auto& f_function_obj : M_face_data_callbacks){
		f_function_obj(M_face_data, M_landmark_data, mirror_status());
	}

}

void J_PXC_Face_Processor::add_face_notification_callback(Face_Data_Callback_t i_callback){
	M_face_data_callbacks.push_back(i_callback);

}


#ifndef VS_2013
void J_PXC_Face_Processor::default_initialization(){
	M_face_detection_module = nullptr;
	M_face_module = nullptr;
	M_landmark_module = nullptr;
}
#endif //!VS_2013

J_PXC_Face_Processor::~J_PXC_Face_Processor(){
	M_face_module->Release();
}

}



