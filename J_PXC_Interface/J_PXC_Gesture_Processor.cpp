#include "J_PXC_Gesture_Processor.h"
//
#include "J_Camera_Manager.h"
//
#include <iostream>

using std::cerr; using std::endl;
namespace jomike{
static J_PXC_Error_Manager s_pxc_error;
static Instance_Pointer<J_Camera_Manager> s_camera_manager;




void J_PXC_Gesture_Processor::add_gesture_notification_callback(Gesture_Callback_t i_callback){
	M_gesture_callbacks.push_back(i_callback);
}



void J_PXC_Gesture_Processor::add_geonode_notification_callback(GeoNode_Callback_t i_callback){
	M_geonode_callbacks.push_back(i_callback);
}

void J_PXC_Gesture_Processor::push_gesture_data(){

	for(auto& f_function_obj : M_gesture_callbacks){
		for(auto& f_gesture : M_gesture_stack){
			f_function_obj(f_gesture);
		}
	}

	M_gesture_stack.resize(0);

}

void J_PXC_Gesture_Processor::process_gestures(){
	assert(M_gesture_module);


	M_gesture_history.push_front(Gesture_Cont_t());

	static const PXC_GeoNode::Label labels[] = {PXC_GeoNode::LABEL_BODY_HAND_PRIMARY
		, PXC_GeoNode::LABEL_BODY_HAND_SECONDARY, PXC_GeoNode::LABEL_BODY_ELBOW_PRIMARY
		, PXC_GeoNode::LABEL_BODY_ELBOW_SECONDARY};

	const int num_geonodes = sizeof(labels) / sizeof(PXC_GeoNode::Label);
	PXC_Gesture_Data gesture_data;
	for(int i = 0; i < num_geonodes; i++){
		for(int b = 0;; b++){
			gesture_data = zero_set<PXC_Gesture_Data>();
			s_pxc_error = M_gesture_module->QueryGestureData(0, labels[i]
				, b, &gesture_data);
			if(pxc_status_item_unavailable()){
				pxc_clear_error();
				break;
			}

			M_gesture_stack.push_back(gesture_data);
			M_gesture_history.front().push_back(gesture_data);
		}
	}

	if(M_gesture_history.front().empty()){
		M_gesture_history.pop_front();
	} /*else{
		M_gesture_history.pop_back();
		}
		*/
	static j_size_t s_last_notification_size = 0;
	if(!(M_gesture_history.size() % 100) 
		&& M_gesture_history.size() != s_last_notification_size){
		cerr << "\nGesture Count: " << M_gesture_history.size();
		s_last_notification_size = M_gesture_history.size();
	}
	if(pxc_status_item_unavailable()){ pxc_clear_error(); }
}


void J_PXC_Gesture_Processor::process_geonodes(){
	static_assert(2 == NUM_GEO_SIDES, "GeoNode index mismatch");
	static_assert(11 == NUM_NODES_PER_GEO_SIDE, "GeoNode nodes per side mismatch");
	M_gesture_module->QueryNodeData(0, PXC_GeoNode::LABEL_BODY_HAND_PRIMARY, 10, M_node_data[0]);
	M_gesture_module->QueryNodeData(0, PXC_GeoNode::LABEL_BODY_HAND_SECONDARY, 10, M_node_data[1]);
	M_gesture_module->QueryNodeData(0, PXC_GeoNode::LABEL_BODY_ELBOW_PRIMARY, &M_node_data[0][10]);
	M_gesture_module->QueryNodeData(0, PXC_GeoNode::LABEL_BODY_ELBOW_SECONDARY, &M_node_data[1][10]);


}

void J_PXC_Gesture_Processor::push_geonodes_data(){
	for(auto& f_function_obj : M_geonode_callbacks){
		f_function_obj(M_node_data[0], M_node_data[1], mirror_status());
	}
}

void J_PXC_Gesture_Processor::derived_finish_processing(){
	process_gestures();
	process_geonodes();
}

void J_PXC_Gesture_Processor::push_data(){
	push_gesture_data();
	push_geonodes_data();
}

J_PXC_Gesture_Processor::J_PXC_Gesture_Processor(J_PXC_Stream_Shared_t i_stream){
	wchar_t k_gesture_name[] = L"Hand/Finger Tracking and Gesture Recognition";
	set_parent(i_stream);
	assert(Image_Format::DEPTH16 == i_stream->format());

	set_to_zero(&M_gesture_description);

	memset(M_gesture_description.friendlyName, 0, sizeof(wchar_t)* 256);
	memcpy(M_gesture_description.friendlyName, k_gesture_name, sizeof(k_gesture_name));

	s_pxc_error = s_camera_manager
		->get_pxc_session()
		.CreateImpl<PXCGesture>(&M_gesture_description, &M_gesture_module);

	NO_PXC_ERROR;

	set_to_zero(&M_gesture_profile_info);

	M_gesture_module->QueryProfile(0, &M_gesture_profile_info);
	const J_PXC_Stream& src_stream = get_src_stream();
	src_stream.device()
		.set_device_capabilities(M_gesture_profile_info.inputs.devCaps);
	M_gesture_profile_info.inputs.streams[0].sizeMax.width
		= M_gesture_profile_info.inputs.streams[0].sizeMin.width
		= src_stream.width();
	M_gesture_profile_info.inputs.streams[0].sizeMax.height
		= M_gesture_profile_info.inputs.streams[0].sizeMin.height
		= src_stream.height();

	M_gesture_profile_info.inputs.streams[0].format = PXCImage::COLOR_FORMAT_DEPTH;

	s_pxc_error = M_gesture_module->SetProfile(&M_gesture_profile_info);
	s_pxc_error = M_gesture_module->QueryProfile(&M_gesture_profile_info);
	NO_PXC_ERROR;
}

void J_PXC_Gesture_Processor::derived_process_image_async(PXC_Image* i_image
	, PXC_SyncPoint* i_syncpoint){

	s_pxc_error = M_gesture_module->ProcessImageAsync(i_image
		, i_syncpoint);

	NO_PXC_ERROR;
}

J_PXC_Gesture_Processor::~J_PXC_Gesture_Processor(){
	M_gesture_module->Release();
}


}



