#include "J_Camera_Manager.h"
//
#include <J_OpenGl.h>
//
#include <string>
//
#include <sstream>
//
#include <j_physical_constants.h>
//
#include "J_PXC_Device.h"
//
#include "J_PXC_Gesture_Processor.h"
using std::string; using std::wstring; using std::ostringstream;
//
#include <array>
using std::array;
//
#include<thread>
//
#include <chrono>
namespace this_thread = std::this_thread;
namespace chrono = std::chrono;
//
#include <limits>
#include "J_PXC_Module.h"
//
#include "J_PXC_Module.h"
//IO Facilities
#include <iostream>
#include "J_PXC_Depth_Stream_Processor.h"
#include "J_PXC_Color_Stream_Processor.h"
#include "J_Face_Stream_Processor.h"
using std::wostringstream; using std::fill;
using std::cerr; using std::endl; using std::wcerr;
using std::unique_ptr; using std::copy; using std::tan;
const int A_HIGH_NUMBER = 20000;
const int MAX_DEVICES = 50; using std::all_of;
using std::fill; using std::min;
using std::numeric_limits;
using std::max; using std::map;
using std::dynamic_pointer_cast;
using std::reverse;


namespace jomike{


static Instance_Pointer<Contexts_Handler> s_contexts;



static J_PXC_Error_Manager s_pxc_error;

template <typename Num_t>
static j_float to_radians(Num_t i_val){ return static_cast<j_float>(PI*i_val / 180.0f); }

J_Camera_Manager& J_Camera_Manager::get_instance(){
	static J_Camera_Manager camera_manager;
	return camera_manager;
}\

//Constructors---------------------------------------------------------------------------------------
J_Camera_Manager::J_Camera_Manager(){
#ifndef VS_2013
	default_initialization();
#endif //!VS_2013
	s_pxc_error = PXCSession_Create(&M_pxc_session);
	
	
	s_pxc_error = load_j_pxc_modules(M_pxc_session);

	NO_PXC_ERROR;
	init_device_list();
}

#ifndef VS_2013

void J_Camera_Manager::default_initialization(){
	M_mirror_status = false;
}

#endif //!VS_2013

//Notify---------------------------------------------------------------


PXC_Gesture_Data J_Camera_Manager::pop_gesture(){
	if (M_gestures.empty()){
		return zero_set<PXC_Gesture_Data>();
	}

	PXC_Gesture_Data gesture = M_gestures.back();
	M_gestures.pop_back();
	return gesture;
}

//View Management---------------------------------------------------------------------------------------

void J_Camera_Manager::update(){
	

	if(pxc_exec_timeout()){
		pxc_clear_error();
		return;
	}

	

	if(M_depth_stream && M_depth_stream->ready_for_processing()){
		M_depth_stream->finish_stream_processing();
		M_depth_stream->start_stream_processing();
		M_depth_stream->push_stream_data();
		M_gesture_fps_counter.count_frame();
	}
	
	if(M_color_stream && M_color_stream->ready_for_processing()){
		M_color_stream->finish_stream_processing();
		M_color_stream->push_stream_data();
		M_color_stream->start_stream_processing();
		
	}

	

}

void J_Camera_Manager::clear_devices(){

	M_devices_by_name.clear();
	M_device.reset();
}

void J_Camera_Manager::init_device_list(){

	PXCSession::ImplDesc device_description = zero_set<PXCSession::ImplDesc>();
	device_description.group = PXCSession::IMPL_GROUP_SENSOR;
	device_description.subgroup = PXCSession::IMPL_SUBGROUP_VIDEO_CAPTURE;

	for(int module_ix = 0;;++module_ix){
		PXCSession::ImplDesc capture_description;
		s_pxc_error = M_pxc_session->QueryImpl(&device_description, module_ix, &capture_description);

		if(pxc_status_item_unavailable()){
			pxc_clear_error();
			break;
		}

		if(pxc_error()){
			pxc_clear_error();
			continue;
		}

		PXCCapture* capture;

		s_pxc_error = M_pxc_session->CreateImpl<PXCCapture>(&capture_description, &capture);
		if(pxc_error()){
			pxc_clear_error();
			continue;
		}

		find_devices_for_module(module_ix, capture);
		capture->Release();
		break;
	}

}


void J_Camera_Manager::find_devices_for_module(int i_capture_id, PXCCapture* i_capture){
	pxcStatus error = PXC_STATUS_NO_ERROR;

	for(int i=0;i < MAX_DEVICES;i++){
		PXCCapture::DeviceInfo device_info = zero_set<PXCCapture::DeviceInfo>();
		PXC_Device device = nullptr;
		s_pxc_error = i_capture->CreateDevice(i, &device);

		if(pxc_status_item_unavailable()){
			J_PXC_RELEASE(device);
			pxc_clear_error();
			break;
		}

		s_pxc_error = device->QueryDevice(&device_info);




		if(pxc_error()){
			pxc_clear_error();
			J_PXC_RELEASE(device);
			continue; 
		}
		wostringstream wostr;
		wostr << ' ' << M_devices_by_name.size() << ":\t";
		
		assert(!error);

		wstring detail_string(wostr.str());

		detail_string +=  device_info.name;

		

		assert(!M_devices_by_name.count(detail_string));

		M_devices_by_name[detail_string]
			= J_PXC_Device_Shared_t(new J_PXC_Device(device, i_capture_id, i, detail_string));

	}

}

J_Camera_Error::operator bool()const{
	return str().empty();
}

void J_Camera_Manager::set_stream(wstring i_device_name){
	auto device_pos = M_devices_by_name.find(i_device_name);
	assert(M_devices_by_name.end() != device_pos);
	J_PXC_Device_Shared_t old_device = M_device;
	J_PXC_Device_Shared_t new_device = device_pos->second;
	
	assert(new_device);
	set_device(new_device);

	
	M_depth_stream = M_device->create_stream(320, 240, Image_Format::DEPTH16);
#ifdef _DEBUG
	M_color_stream = M_device->create_stream(640, 480, Image_Format::BGR24_UBYTE);
#else
	M_color_stream = M_device->create_stream(1280, 720, Image_Format::BGR24_UBYTE);
#endif
	M_device->release_device();
	if(!M_depth_stream || !M_color_stream){
		M_depth_stream.reset();
		M_color_stream.reset();
		set_device(old_device);

		return;
	}
	assert(M_depth_stream);
	assert(M_color_stream);
	M_depth_stream_processor
		= J_PXC_Depth_Stream_Depth_Processor_Shared_t
		(new J_PXC_Depth_Stream_Depth_Processor(M_depth_stream));

	M_gesture_processor
 		= J_PXC_Gesture_Processor_Shared_t(new J_PXC_Gesture_Processor(M_depth_stream));

	M_depth_stream->add_stream_processor(M_depth_stream_processor);
	M_depth_stream->add_stream_processor(M_gesture_processor);

	M_face_processor
		= J_PXC_Face_Processor_Shared_t(new J_PXC_Face_Processor(M_color_stream));


	M_color_stream_processor
		= J_PXC_Color_Stream_Processor_Shared_t
		(new J_PXC_Color_Stream_Processor(M_color_stream));

	M_j_face_stream_processor
		= J_Face_Stream_Processor_Shared_t
		(new J_Face_Stream_Processor(M_color_stream));

	M_color_stream->add_stream_processor(M_color_stream_processor);
	M_color_stream->add_stream_processor(M_face_processor);
	M_color_stream->add_stream_processor(M_j_face_stream_processor);


	M_depth_stream->start_stream_processing();
	M_color_stream->start_stream_processing();


}

void J_Camera_Manager::set_device(J_PXC_Device_Shared_t i_device){
	if(!i_device){
		return;
	}

	clear_device();
	
	M_device = i_device;

}

void J_Camera_Manager::clear_device(){
	if(M_device){
		M_device->reset_state();
	}


	M_gesture_processor.reset();
	M_face_processor.reset();
	M_color_stream_processor.reset();
	M_depth_stream_processor.reset();

	M_depth_stream.reset();
	M_color_stream.reset();
	M_device.reset();
}

j_float J_Camera_Manager::get_aspect_ratio()const{
	if(!M_color_stream){
		return 3.0f / 4.0f;
	}
	auto width = M_color_stream->width();
	auto height = M_color_stream->height();
	return static_cast<j_float>(width) / height;
}

J_Camera_Manager::~J_Camera_Manager(){
	if(M_color_stream){
		M_color_stream->clear_stream_processes();
	}
	if(M_depth_stream){
		M_depth_stream->clear_stream_processes();
	}

}

j_dbl J_Camera_Manager::get_gesture_fps()const{ return M_gesture_fps_counter.fps(); }

ex_array<wstring> J_Camera_Manager::get_device_names()const{
	ex_array<wstring> device_names;
	for(auto& f_device_pair : M_devices_by_name){
		device_names.push_back(f_device_pair.first);
	}
	return device_names;
}


void J_Camera_Manager::poll_events(){
	update();

}

J_PXC_Gesture_Processor_Shared_t J_Camera_Manager::get_gesture_processor(){
	assert(M_gesture_processor);
	return M_gesture_processor;
}

J_PXC_Face_Processor_Shared_t J_Camera_Manager::get_face_processor(){
	return M_face_processor;
}

J_PXC_Color_Stream_Processor_Shared_t J_Camera_Manager::get_color_stream_processor(){
	return M_color_stream_processor;
}

J_PXC_Depth_Stream_Depth_Processor_Shared_t J_Camera_Manager::get_depth_stream_depth_processor(){
	return M_depth_stream_processor;
}

void J_Camera_Manager::clear(){
	*this = J_Camera_Manager();
}

PXCSession& J_Camera_Manager::get_pxc_session(){
	return *(get_instance().M_pxc_session);
}

const J_PXC_Stream& J_Camera_Manager::get_depth_stream()const{
	return *M_depth_stream;
}

const J_PXC_Stream& J_Camera_Manager::get_color_stream()const{
	return *M_color_stream;
}

bool J_Camera_Manager::gesture_detection_status()const{
	return bool(M_gesture_processor);
}

void J_Camera_Manager::set_mirror_status(bool i_status){
	M_mirror_status = i_status;
	if(M_depth_stream){
		M_depth_stream->set_stream_mirror_status(M_mirror_status);
	}
	if(M_color_stream){
		M_color_stream->set_stream_mirror_status(M_mirror_status);
	}

}

bool J_Camera_Manager::mirror_status()const{
	return M_mirror_status;
}

void J_Camera_Manager::notify_end(){
	J_PXC_Error_Manager::clear();
	clear_device();
}

//Destruction---------------------------------------------------------------------------------------


//
//
//
//void J_Camera_Manager::set_device(J_PXC_Device_Shared_t i_device){
//	clear_device();
//	assert(i_device);
//	M_device = i_device;
//
//	/*
//	M_device_properties = Device_Properties(M_device);
//
//
//
//	PXCCapture::VideoStream::DataDesc stream_desc
//		= zero_set<PXCCapture::VideoStream::DataDesc>();
//
//	stream_desc.streams[0].format = PXCImage::COLOR_FORMAT_RGB24;
//	PXC_Video_Stream_Profile* profile = nullptr;
//	ex_array<PXC_Video_Stream_Profile> all_profiles;
//	ex_array<PXC_Stream_Info> stream_infos;
//	PXC_Video_Stream color_stream =nullptr;
//	for (int stream_index = 0; stream_index < 100; ++stream_index){
//		s_pxc_error = M_device->CreateStream(stream_index, &M_color_stream);
//
//		if (pxc_error()){ 
//			pxc_clear_error();
//			break;
//		}
//
//		ex_array<PXC_Video_Stream_Profile>
//			profiles(get_video_stream_profiles(M_color_stream));
//		all_profiles.insert(all_profiles.end(), profiles.begin(), profiles.end());
//		all_profiles.push_back(zero_set<PXC_Video_Stream_Profile>());
//		profile = get_closest_profile_match(profiles, Stream_Formats::COLOR
//			, 640, 480);
//
//		PXC_Stream_Info stream_info;
//		M_color_stream->QueryStream(&stream_info);
//		stream_infos.push_back(stream_info);
//		if (!profile){
//			M_color_stream->Release();
//			M_color_stream = nullptr;
//			continue;
//		}
//		s_pxc_error = M_color_stream->SetProfile(profile);
//		M_image_profile_info = *profile;
//		set_to_zero(&M_color_stream_info);
//		M_color_stream->QueryStream(&M_color_stream_info);
//		
//		if (pxc_error()){ 
//			pxc_clear_error();
//			continue; 
//		}
//		color_stream = M_color_stream;
//		PXC_Video_Stream_Profile stream_profile;
//		
//		M_color_stream->QueryProfile(&stream_profile);
//		stream_desc.streams[0].sizeMin.width = stream_profile.imageInfo.width;
//		stream_desc.streams[0].sizeMin.height = stream_profile.imageInfo.height;
//		stream_desc.streams[0].sizeMax.width = stream_profile.imageInfo.width;
//		stream_desc.streams[0].sizeMax.height = stream_profile.imageInfo.height;
//		stream_desc.streams[0].format = stream_profile.imageInfo.format;
//		M_stream_description = stream_desc.streams[0];
//		//break;
//	}
//	all_profiles.size();
//	M_color_stream = color_stream;
//	*/
//}
//
//
//void J_Camera_Manager::set_gesture_detection_status(bool i_status){
//	wchar_t k_gesture_name[] = L"Hand/Finger Tracking and Gesture Recognition";
//
//	if (M_gesture_detection_status == i_status){
//		return;
//	}
//	if (M_gesture_detection_status){
//		reset_gesture();
//		return;
//	}
//	assert(!M_depth_stream_read_status);
//
//	M_gesture_detection_status = true;
//	
//	assert(!M_gesture_module);
//	assert(!M_sync_points[GESTURE]);
//	PXC_Impl_Description gesture_description = zero_set<PXC_Impl_Description>();
//
//	memset(gesture_description.friendlyName, 0, sizeof(wchar_t)* 256);
//	memcpy(gesture_description.friendlyName, k_gesture_name, sizeof(k_gesture_name));
//
//	s_pxc_error = get_pxc_session().CreateImpl<PXCGesture>(&gesture_description, &M_gesture_module);
//
//	NO_PXC_ERROR;
//
//
//
//	s_pxc_error = M_device->CreateStream(2, &M_depth_stream);
//
//	if (pxc_error()){
//		pxc_clear_error();
//		M_depth_stream = nullptr;
//		reset_gesture();
//		return;
//	}
//
//	
//	ex_array<PXC_Video_Stream_Profile> profiles(
//		get_video_stream_profiles(M_depth_stream));
//
//	if (profiles.size() != 8){
//		reset_gesture();
//		return;
//	}
//
//	M_depth_profile_info = profiles[4];
//	M_depth_vertex_profile_info = profiles[5];
//
//	s_pxc_error = M_depth_stream->SetProfile(&M_depth_profile_info);
//	NO_PXC_ERROR;
//	set_to_zero(&M_depth_stream_info);
//	set_to_zero(&M_depth_stream_profile_info);
//	M_depth_stream->QueryStream(&M_depth_stream_info);
//	M_depth_stream->QueryProfile(&M_depth_stream_profile_info);
//
//
//	
//	set_to_zero(&M_gesture_profile_info);
//	M_gesture_module->QueryProfile(&M_gesture_profile_info);
//	set_device_capabilities(M_gesture_profile_info.inputs.devCaps);
//	M_gesture_profile_info.inputs.streams[0].format = PXCImage::COLOR_FORMAT_DEPTH;
//	M_gesture_profile_info.inputs.streams[0].options = 0;
//	s_pxc_error = M_gesture_module->SetProfile(&M_gesture_profile_info);
//	s_pxc_error = M_gesture_module->QueryProfile(&M_gesture_profile_info);
//	set_device_capabilities(M_gesture_profile_info.inputs.devCaps);
//	s_pxc_error = M_gesture_module->SetProfile(&M_gesture_profile_info);
//	NO_PXC_ERROR;
//
//	M_depth_stream_read_status = true;
//	start_depth_stream_read();
//
//	start_gesture_detection();
//
//	init_projection();
//
//}
//
//void J_Camera_Manager::set_face_detection_status(bool i_status){
//	if (M_face_detection_status == i_status){
//		return;
//	}
//	if (M_face_detection_status){
//		reset_face_module();
//		return;
//	}
//	M_landmark_data.clear();
//	M_landmark_data.resize(MAX_FACES);
//	M_face_detection_status = true;
//
//	assert(!M_face_module);
//	assert(!M_sync_points[FACE]);
//
//	s_pxc_error = M_pxc_session->CreateImpl<PXCFaceAnalysis>(&M_face_module);
//
//	ex_array<PXCFaceAnalysis::ProfileInfo> face_module_profiles_info;
//	for(int i = 0;; i++){
//		face_module_profiles_info.push_back(zero_set < PXCFaceAnalysis::ProfileInfo>());
//		s_pxc_error = M_face_module->QueryProfile(i, &face_module_profiles_info.back());
//		if(pxc_status_item_unavailable()){
//			pxc_clear_error();
//			face_module_profiles_info.pop_back();
//			break;
//		}
//
//	}
//	auto info = face_module_profiles_info.front();
//	M_stream_description.format = info.inputs.streams[0].format;
//	set_device_capabilities(info.inputs.devCaps);
//	info.inputs.streams[0] = M_stream_description;
//
//	info.iftracking = true;
//	s_pxc_error = M_face_module->SetProfile(&info);
//
//	s_pxc_error = M_face_module->QueryProfile(&info);
//
//	M_landmark_module = M_face_module->DynamicCast<PXCFaceAnalysis::Landmark>();
//
//	assert(M_landmark_module);
//	
//	set_to_zero(&M_landmark_profile);
//	s_pxc_error = M_landmark_module->QueryProfile(0, &M_landmark_profile);
//	M_landmark_profile.labels = PXCFaceAnalysis::Landmark::LABEL_7POINTS;
//	s_pxc_error = M_landmark_module->SetProfile(&M_landmark_profile);
//	s_pxc_error = M_landmark_module->QueryProfile(&M_landmark_profile);
//	
//	
//	M_face_detection_module = M_face_module->DynamicCast<PXCFaceAnalysis::Detection>();
//	assert(M_face_detection_module);
//
//	set_to_zero(&M_face_detection_profile);
//	s_pxc_error = M_face_detection_module->QueryProfile(0,&M_face_detection_profile);
//
//	M_face_detection_profile.viewAngles = PXCFaceAnalysis::Detection::VIEW_ANGLE_OMNI;
//	M_face_detection_profile.responsiveness = 85;
//	
//	s_pxc_error = M_face_detection_module->SetProfile(&M_face_detection_profile);
//	s_pxc_error = M_face_detection_module->QueryProfile(&M_face_detection_profile);
//
//
//
//	NO_PXC_ERROR;
//	start_stream();
//	start_face_detection();
//}
//
//
//
//
//void J_Camera_Manager::start_stream(){
//	M_stream_read_status = true;
//	assert(has_stream());
//	start_color_stream_read();
//}
//
//bool J_Camera_Manager::has_device()const{ return M_device; }
//
//bool J_Camera_Manager::has_stream()const{ return M_color_stream; }
//
//
//void J_Camera_Manager::map_color_to_depth_stream(){
//	assert(!"The method or operation is not implemented.");
//}
//
//
//void J_Camera_Manager::start_gesture_detection(){
//	if(!gesture_detection_status()){
//		return;
//	}
//	assert(M_sync_points[DEPTH]);
//	assert(!M_sync_points[GESTURE]);
//
//	
//}
//
//bool J_Camera_Manager::finish_gesture_stream_read(){
//	if (!gesture_detection_status()){
//		return false;
//	}
//
//
//	NO_PXC_ERROR;
//
//	M_gesture_fps_counter.count_frame();
//
//	
//	process_geonodes();
//	process_gestures();
//
//	push_geonodes_data();
//	push_gesture_data();
//
//	return true;
//}
//
//
//
//void J_Camera_Manager::project_depth_to_color_stream(){
//
//	static ex_array<PXCPointF32> color_poses;
//
//	static ex_array<PXCPoint3DF32> depth_poses;
//
//
//
//	const int x_max = depth_image_width();
//	const int y_max = depth_image_height();
//	const int depth_pixels = y_max*x_max;
//
//	color_poses.resize(depth_pixels);
//	depth_poses.resize(depth_pixels);
//
//
//	PXC_ImageData depth_vertex_data;
//	s_pxc_error = M_depth_image->AcquireAccess(PXCImage::ACCESS_READ
//		, PXCImage::COLOR_FORMAT_DEPTH, &depth_vertex_data);
//
//	NO_PXC_ERROR;
//
//	int plane_index = 0;
//	int16_t* depth_buffer_ptr
//		= reinterpret_cast<int16_t*>((depth_vertex_data.pitches[plane_index] >= 0)
//		? depth_vertex_data.planes[plane_index] :
//		depth_vertex_data.planes[plane_index]
//		+ depth_vertex_data.pitches[plane_index] * depth_image_width());
//
//
//	PXCPoint3DF32 pos;
//
//	for(int i = 0, index = 0; i < y_max; i++){
//		pos.y = static_cast<j_float>(i);
//		for(int b = 0; b < x_max; b++, index++){
//			pos.x = static_cast<j_float>(b);
//			pos.z = depth_buffer_ptr[index];
//			depth_poses[index] = pos;
//		}
//	}
//
//	assert(depth_pixels == depth_poses.size());
//
//	
//	//init_projection();
//	s_pxc_error = M_projector
//		->MapDepthToColorCoordinates(depth_pixels, depth_poses.data(), color_poses.data());
//
//	static ex_array<j_ubyte> s_depth_image_buffer;
//	static const j_float DEPTH_TO_COLOR_RATIO = 1.0f;
//
//
//
//	s_depth_image_buffer.swap(M_depth_stream_buffer);
//	M_depth_stream_buffer.resize(depth_pixels*NUM_DEPTH_CHANNELS);
//	fill(M_depth_stream_buffer.begin(), M_depth_stream_buffer.end(), J_UBYTE_ZERO);
//	
//	
//	for(int i = 0 ; i < depth_pixels; i++){
//		int x_pos = static_cast<int>(color_poses[i].x)/2;
//		int y_pos = static_cast<int>(color_poses[i].y)/2;
//
//		if(depth_buffer_ptr[i] > 1500){
//			continue;
//		}
//		if(!between_inclusive(x_pos, 0, x_max - 1)){
//			continue;
//		}
//		if(!between_inclusive(y_pos, 0, y_max - 1)){
//			continue;
//		}
//		int dest_index = NUM_DEPTH_CHANNELS*(y_pos*x_max + x_pos);
//
//		auto start_pos = s_depth_image_buffer.begin() + i*NUM_DEPTH_CHANNELS;
//		copy(start_pos, start_pos + NUM_DEPTH_CHANNELS
//			, M_depth_stream_buffer.begin() + dest_index);
//	}
//	
//}
//
//void J_Camera_Manager::map_depth_to_color_stream(){
//	assert(!"This is the old mapping function");
//	/*
//		Here we shall map the depth image to the color stream 
//	*/
//	static ex_array<j_ubyte> s_depth_image_buffer;
//	static const j_float DEPTH_TO_COLOR_RATIO = 1.0f;
//	const int depth_pixels = depth_image_height()*depth_image_width();
//	//const int color_pixels = image_height()*image_width();
//	
//	s_depth_image_buffer.swap(M_depth_stream_buffer);
//	M_depth_stream_buffer.resize(depth_pixels*NUM_DEPTH_CHANNELS);
//	fill(M_depth_stream_buffer.begin(), M_depth_stream_buffer.end(), J_UBYTE_ZERO);
//
//	PXC_ImageData depth_vertex_data;
//	s_pxc_error = M_depth_image->AcquireAccess(PXCImage::ACCESS_READ
//		, PXCImage::COLOR_FORMAT_DEPTH, &depth_vertex_data);
//	
//	NO_PXC_ERROR;
//
//	int plane_index = 0;
//	int16_t* image_buffer_ptr 
//		= reinterpret_cast<int16_t*>((depth_vertex_data.pitches[plane_index] >= 0)
//		? depth_vertex_data.planes[plane_index] :
//		depth_vertex_data.planes[plane_index]
//		+ depth_vertex_data.pitches[plane_index] * depth_image_width());
//
//
//	for(int i = 0; i < depth_pixels; i++){
//		bool stop = false;
//
//		Depth_Vertex_t depth_vertex;
//		j_short z_pos = image_buffer_ptr[i];
//		if((M_device_properties.M_depth_principal_point.x == i % (depth_image_width()))
//			&& (M_device_properties.M_depth_principal_point.y
//			== (i / (depth_image_width())))){
//			stop = true;
//		}
//
//		if(z_pos > 3000){
//			continue;
//		}
//		j_float x_radians 
//			= to_radians(M_device_properties.M_depth_field_of_view.x / 2.0f);
//		
//		const float max_x_val = z_pos*tan(x_radians);
//
//		if(i%depth_image_width() == 310){
//			stop = true;
//		}
//
//
//		j_float x_ratio 
//			= static_cast<j_float>(i % depth_image_width() - depth_image_width()/2)
//			/ (depth_image_width()/2);
//
//		depth_vertex.x = static_cast<j_short>(x_ratio*max_x_val);
//		
//
//		j_float y_radians 
//			= to_radians(M_device_properties.M_depth_field_of_view.y / 2.0f);
//
//		const float max_y_val = z_pos*tan(y_radians);
//
//		j_float y_ratio 
//			= static_cast<j_float>(i / depth_image_width() - depth_image_height() / 2)
//			/ (depth_image_height() / 2);
//
//		depth_vertex.y = static_cast<j_short>(y_ratio*max_y_val);
//		depth_vertex.z = z_pos;
//		
//		int destination_x_pos = color_stream_x_pixel(depth_vertex);
//		int destination_y_pos = color_stream_y_pixel(depth_vertex);
//
//		if(!between_inclusive(destination_x_pos, 0, depth_image_width()-1)){
//			continue;
//		}
//		if(!between_inclusive(destination_y_pos, 0, depth_image_height()-1)){
//			continue;
//		}
//
//		int dest_index = (depth_image_width()*destination_y_pos + destination_x_pos)
//			*NUM_DEPTH_CHANNELS;
//
//		int src_index = i*NUM_DEPTH_CHANNELS;
//		M_depth_stream_buffer[dest_index] += s_depth_image_buffer[src_index];
//		M_depth_stream_buffer[dest_index+1] += s_depth_image_buffer[src_index+1];
//		M_depth_stream_buffer[dest_index+2] += s_depth_image_buffer[src_index+2];
//		M_depth_stream_buffer[dest_index+3] += s_depth_image_buffer[src_index+3];
//	}
//
//	M_depth_image->ReleaseAccess(&depth_vertex_data);
//	NO_PXC_ERROR;
//}
//
//int J_Camera_Manager::color_stream_x_pixel(Depth_Vertex_t i_depth_vertex){
//	j_float radians = to_radians(M_device_properties.M_color_field_of_view.x / 2.0f);
//
//	const float max_x_val = i_depth_vertex.z*tan(radians);
//
//	static const short s_x_offset = 26;
//	i_depth_vertex.x += s_x_offset;
//	j_float ratio = i_depth_vertex.x / max_x_val;
//
//	int half_res = depth_image_width() / 2;
//	return half_res + static_cast<int>(ratio*half_res);
//
//}
//
//int J_Camera_Manager::color_stream_y_pixel(Depth_Vertex_t i_depth_vertex){
//	j_float radians = to_radians(M_device_properties.M_color_field_of_view.y / 2.0f);
//
//	const float max_y_val = i_depth_vertex.z*tan(radians);
//
//	j_float ratio = i_depth_vertex.y / max_y_val;
//
//	int half_res = depth_image_height() / 2;
//	return half_res + static_cast<int>(ratio*half_res);
//}
//
//
//bool J_Camera_Manager::face_detection_status()const{ return M_face_detection_status; }
//
//bool J_Camera_Manager::gesture_detection_status()const{
//	return M_gesture_detection_status;
//}
//
//
//void J_Camera_Manager::reset_face_module(){
//	assert(M_face_module);
//	if (M_sync_points[FACE]){
//		M_sync_points[FACE]->Synchronize(2000000u);
//	}
//	J_PXC_RELEASE(M_sync_points[FACE]);
//
//	J_PXC_RELEASE(M_face_module);
//	M_face_module = nullptr;
//	M_face_detection_status = false;
//	return;
//}
//
//void J_Camera_Manager::reset_gesture(){
//	if (M_sync_points[GESTURE]){
//		M_sync_points[GESTURE]->Synchronize(2000000u);
//	};
//
//	J_PXC_RELEASE(M_depth_stream);
//
//	M_gesture_detection_status
//		= M_depth_stream_read_status = false;
//
//	J_PXC_RELEASE(M_sync_points[GESTURE]);
//	J_PXC_RELEASE(M_sync_points[DEPTH]);
//	J_PXC_RELEASE(M_gesture_module);
//	J_PXC_RELEASE(M_depth_image);
//	cerr << "\nGesture Reset!";
//
//	return;
//}
//
//void J_Camera_Manager::destroy(){
//	
//	clear_device();
//	J_PXC_RELEASE(M_accelerator);
//
//
//
//
//
//	
//	for(auto f_device : M_devices_by_name){
//		J_PXC_RELEASE(f_device.second);
//	}
//
//	M_devices_by_name.clear();
//
//	J_PXC_RELEASE(M_pxc_session);
//
//	J_PXC_Error_Manager::clear();
//}
//
//PXCSession& J_Camera_Manager::get_pxc_session(){
//	return *(get_instance().M_pxc_session);
//}
//
//
//int J_Camera_Manager::image_width(){
//	return M_image_profile_info.imageInfo.width;
//}
//
//int J_Camera_Manager::image_height(){
//	return M_image_profile_info.imageInfo.height;
//}
//
//int J_Camera_Manager::depth_image_width(){
//	return M_depth_profile_info.imageInfo.width;
//}
//
//int J_Camera_Manager::depth_image_height(){
//	return M_depth_profile_info.imageInfo.height;
//}
//
///*
//
//j_float J_Camera_Manager::depth_color_stream_fov_x_ratio()const{
//	//assert(M_device);
//
//
//	j_float color_x = M_device_properties.M_color_field_of_view.x;
//	j_float depth_x = M_device_properties.M_depth_field_of_view.x;
//
//	return depth_x/color_x;
//
//}
//
//j_float J_Camera_Manager::depth_color_stream_fov_y_ratio()const{
//	//assert(M_device);
//
//
//	j_float color_y = M_device_properties.M_color_field_of_view.y;
//	j_float depth_y = M_device_properties.M_depth_field_of_view.y;
//
//	return depth_y / color_y;
//}
//
//
//void J_Camera_Manager::put_line_in_center_of_color_stream(const int NUM_COLOR_STREAM_CHANNELS){
//	for(int i = 0; i < image_width()*image_height(); i++){
//
//		j_float x_position
//			= static_cast<j_float>(i%image_width())
//			/ image_width();
//
//		if(abs(x_position - 0.5f) < 0.002f){
//			j_ubyte* cur_pixel = &M_color_stream_buffer[NUM_COLOR_STREAM_CHANNELS*i];
//
//			fill(cur_pixel, cur_pixel + 3, numeric_limits<j_ubyte>::max());
//		}
//	}
//}
//
//*/


}

