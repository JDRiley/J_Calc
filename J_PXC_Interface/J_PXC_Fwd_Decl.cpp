#include "J_PXC_Fwd_Decl.h"




#include "pxcsmartptr.h"
#include "pxcgesture.h"
#include "pxcaccelerator.h"
#include "pxcface.h"
#include "J_PXC_Gesture_Processor.h"

#include "J_PXC_Device.h"

using std::sort; using std::move; using std::wstring;
using std::find_if;
namespace jomike{

pxcStatus s_pxc_status = PXC_STATUS_NO_ERROR;

void pxc_clear_error(){s_pxc_status = PXC_STATUS_NO_ERROR;}

void no_error_exec(pxcStatus i_status){
	s_pxc_status = i_status;
	
	NO_PXC_ERROR;
}

ex_array<pxcStatus> J_PXC_Error_Manager::Ms_errors 
	= ex_array<pxcStatus>(1, PXC_STATUS_NO_ERROR);


pxcStatus& J_PXC_Error_Manager::error(){return s_pxc_status;}


static J_PXC_Error_Manager s_pxc_error;

J_PXC_Error_Manager::J_PXC_Error_Manager():M_pxc_error(s_pxc_status){
}

bool pxc_status_item_unavailable(){return PXC_STATUS_ITEM_UNAVAILABLE == s_pxc_status;}

pxcStatus& J_PXC_Error_Manager::operator=(pxcStatus i_status){
	if(i_status){
		Ms_errors.push_back(i_status);
	}
	if(pxc_status_clear()){
		s_pxc_status = i_status;
	}
	else{
		assert(!"PXC ERROR NOT CLEARED");
	}
	

	return s_pxc_status;
}

void J_PXC_Error_Manager::clear(){
	Ms_errors.clear();
}

Device_Properties::Device_Properties(){}
Device_Properties::Device_Properties(PXC_Device i_device){
	s_pxc_error = i_device->QueryPropertyAsPoint(PXCCapture::Device::PROPERTY_COLOR_FIELD_OF_VIEW
		, &M_color_field_of_view);
	
	pxc_clear_error();
	
	s_pxc_error = i_device->QueryPropertyAsPoint(PXCCapture::Device::PROPERTY_DEPTH_FIELD_OF_VIEW
		, &M_depth_field_of_view);
	pxc_clear_error();
	
	s_pxc_error = i_device->QueryPropertyAsPoint(PXCCapture::Device::PROPERTY_COLOR_PRINCIPAL_POINT
		, &M_color_principal_point);
	pxc_clear_error();

	s_pxc_error = i_device->QueryPropertyAsPoint(PXCCapture::Device::PROPERTY_DEPTH_PRINCIPAL_POINT
		, &M_depth_principal_point);
	pxc_clear_error();

	s_pxc_error = i_device->QueryPropertyAsPoint(PXCCapture::Device::PROPERTY_DEPTH_FOCAL_LENGTH
		, &M_color_focal_length);
	pxc_clear_error();

	s_pxc_error = i_device->QueryPropertyAsPoint(PXCCapture::Device::PROPERTY_DEPTH_FOCAL_LENGTH
		, &M_depth_focal_length);
	pxc_clear_error();

	s_pxc_error = i_device->QueryProperty(PXCCapture::Device::PROPERTY_DEPTH_UNIT
		, &M_depth_unit_in_micro_meters);
	pxc_clear_error();

	device_properties[0].label = PXCCapture::Device::PROPERTY_COLOR_EXPOSURE;
	device_properties[1].label = PXCCapture::Device::PROPERTY_COLOR_BRIGHTNESS;
	device_properties[2].label = PXCCapture::Device::PROPERTY_COLOR_CONTRAST;
	device_properties[3].label = PXCCapture::Device::PROPERTY_COLOR_SATURATION;
	device_properties[4].label = PXCCapture::Device::PROPERTY_COLOR_HUE;
	device_properties[5].label = PXCCapture::Device::PROPERTY_COLOR_GAMMA;
	device_properties[6].label = PXCCapture::Device::PROPERTY_COLOR_WHITE_BALANCE;
	device_properties[7].label = PXCCapture::Device::PROPERTY_COLOR_SHARPNESS;
	device_properties[8].label = PXCCapture::Device::PROPERTY_COLOR_BACK_LIGHT_COMPENSATION;
	device_properties[9].label = PXCCapture::Device::PROPERTY_COLOR_GAIN;
	device_properties[10].label = PXCCapture::Device::PROPERTY_AUDIO_MIX_LEVEL;

	device_properties[11].label = PXCCapture::Device::PROPERTY_DEPTH_SATURATION_VALUE;
	device_properties[12].label = PXCCapture::Device::PROPERTY_DEPTH_LOW_CONFIDENCE_VALUE;
	device_properties[13].label = PXCCapture::Device::PROPERTY_DEPTH_CONFIDENCE_THRESHOLD;
	device_properties[14].label = PXCCapture::Device::PROPERTY_DEPTH_SMOOTHING;
	device_properties[15].label = PXCCapture::Device::PROPERTY_DEPTH_UNIT;
	

	/* Two value properties */
	device_properties[16].label = PXCCapture::Device::PROPERTY_COLOR_FIELD_OF_VIEW;
	device_properties[18].label = PXCCapture::Device::PROPERTY_COLOR_SENSOR_RANGE;
	device_properties[20].label = PXCCapture::Device::PROPERTY_COLOR_FOCAL_LENGTH;
	device_properties[22].label = PXCCapture::Device::PROPERTY_COLOR_PRINCIPAL_POINT;

	device_properties[24].label = PXCCapture::Device::PROPERTY_DEPTH_FIELD_OF_VIEW;
	device_properties[26].label = PXCCapture::Device::PROPERTY_DEPTH_SENSOR_RANGE;
	device_properties[28].label = PXCCapture::Device::PROPERTY_DEPTH_FOCAL_LENGTH;
	device_properties[30].label = PXCCapture::Device::PROPERTY_DEPTH_PRINCIPAL_POINT;


	/* Misc. */
	device_properties[32].label = PXCCapture::Device::PROPERTY_ACCELEROMETER_READING;   // three values
	device_properties[35].label = PXCCapture::Device::PROPERTY_PROJECTION_SERIALIZABLE;
	if(j_true){
		return;
	}
	/* Customized properties */
	device_properties[26].label = PXCCapture::Device::PROPERTY_CUSTOMIZED;
	for(int i = 0; i < 16; i++){
		s_pxc_error = i_device->QueryProperty(device_properties[i].label
			, &device_properties[i].value);
		
		if(pxc_status_item_unavailable()){
			device_properties[i].label = static_cast<PXC_Device_Property>(0);
			device_properties[i].value = 0;
			pxc_clear_error();
		}
	}
	for(int i = 16; i < 32; i++){
		PXCPointF32 point;
		s_pxc_error = i_device->QueryPropertyAsPoint(device_properties[i].label
			, &point);
		device_properties[i].value = point.x;
		device_properties[i + 1].label 
			= static_cast<PXC_Device_Property>(device_properties[i].label + 1);
		device_properties[i + 1].value = point.y;
		++i;
		if(pxc_status_item_unavailable()){
			device_properties[i].label = static_cast<PXC_Device_Property>(0);
			device_properties[i].value = 0;
			pxc_clear_error();
		}
	}
	int devcap_limit = PXCCapture::Device::DEVCAP_LIMIT;
	for(int i = 32; i < PXCCapture::Device::DEVCAP_LIMIT; i++){
		device_properties[i].label = static_cast<PXC_Device_Property>(0);
		device_properties[i].value = 0;
	}
	auto ptr = std::remove_if(device_properties, device_properties + devcap_limit
		, [](PXC_Device_Capabilities y_caps){return y_caps.label == 0; });
	PXC_Device_Capabilities blank_device_cap;
	blank_device_cap.label = static_cast<PXC_Device_Property>(0);
	blank_device_cap.value =  0.0f;
	std::fill(ptr, device_properties + devcap_limit, blank_device_cap);
}

pxcStatus& pxc_status(){return s_pxc_status;}

bool no_pxc_error(){return s_pxc_status >= PXC_STATUS_NO_ERROR;}

bool pxc_error(){return s_pxc_status < PXC_STATUS_NO_ERROR;}

bool pxc_warning(){return s_pxc_status > PXC_STATUS_NO_ERROR;}

bool pxc_status_clear(){return PXC_STATUS_NO_ERROR == s_pxc_status;}

bool pxc_status_waiting_execution(){ return PXC_STATUS_EXEC_INPROGRESS == s_pxc_status; }

bool pxc_exec_timeout(){ return PXC_STATUS_EXEC_TIMEOUT == s_pxc_status; }

ex_array<PXC_Video_Stream_Profile> get_video_stream_profiles(PXC_Video_Stream i_stream){
	assert(i_stream);
	ex_array<PXC_Video_Stream_Profile> profiles;

	for(int profile_index = 0; ; ++profile_index){
		PXCCapture::VideoStream::ProfileInfo profile_info;
		pxc_status() = i_stream->QueryProfile(profile_index, &profile_info);

		if(pxc_error()){
			pxc_clear_error();
			break;
		}

		profiles.push_back(profile_info);
	}
	return move(profiles);
}

bool is_profile_color(const PXC_Video_Stream_Profile& irk_profile){
	switch(irk_profile.imageInfo.format){
	case PXCImage::COLOR_FORMAT_YUY2:
	case PXCImage::COLOR_FORMAT_NV12:
	case PXCImage::COLOR_FORMAT_RGB32:
	case PXCImage::COLOR_FORMAT_RGB24:
		return true;
	default:
		return false;
	}
}

bool is_profile_color(const Image_Format& i_format){
	switch(i_format){
	case Image_Format::DEPTH16:
		return false;
	default:
		return true;
	}
}


PXC_Video_Stream_Profile* get_closest_profile_match(ex_array<PXC_Video_Stream_Profile>&
													irk_profiles, Image_Format i_format, j_uint
													i_width, j_uint i_height){
	if(irk_profiles.empty()){
		return nullptr;
	}

	ex_array<PXC_Video_Stream_Profile*> candidate_profiles;
	for(auto& f_profile : irk_profiles){
		if((is_profile_color(i_format))
			!= is_profile_color(f_profile)){
			continue;
		}

		if(f_profile.imageInfo.width != i_width){
			continue;
		}

		if(f_profile.imageInfo.height != i_height){
			continue;
		}


		candidate_profiles.push_back(&f_profile);
	}
	sort(candidate_profiles.begin(), candidate_profiles.end()
		,[](const PXC_Video_Stream_Profile* y_left, 
		const PXC_Video_Stream_Profile* y_right){
		return (frames_per_second(*y_left) > frames_per_second(*y_right));
	});

	if(!candidate_profiles.empty()){
		return candidate_profiles[0];
		/*auto cand_profile
			 = find_if(candidate_profiles.begin(), candidate_profiles.end()
			 , [](PXC_Video_Stream_Profile* y_profile){
			 return PXCImage::COLOR_FORMAT_RGB24 == y_profile->imageInfo.format;
			 });
			 return candidate_profiles.end() == cand_profile ?
			 candidate_profiles[0] : *cand_profile;*/
	}
	
	return nullptr;
}



j_dbl frames_per_second(const PXC_Video_Stream_Profile& irk_profile){
	return static_cast<j_dbl>(irk_profile.frameRateMax.numerator)
		/static_cast<j_dbl>(irk_profile.frameRateMax.denominator);
}


wstring get_body_name(PXC_Geo_Label i_label){
	switch(i_label & PXCGesture::GeoNode::LABEL_MASK_BODY){		
	case PXCGesture::GeoNode::LABEL_BODY_HAND_PRIMARY:
	case PXCGesture::GeoNode::LABEL_BODY_HAND_SECONDARY:
		return L"hand";
	case PXCGesture::GeoNode::LABEL_FINGER_THUMB:
		return L"thumb_tip";
	case PXCGesture::GeoNode::LABEL_FINGER_RING:
		return L"ring_tip";
	case PXCGesture::GeoNode::LABEL_FINGER_INDEX:
		return L"index_tip";
	case PXCGesture::GeoNode::LABEL_FINGER_PINKY:
		return L"pinky_tip";
	case PXCGesture::GeoNode::LABEL_FINGER_MIDDLE:
		return L"middle_tip";
	case PXCGesture::GeoNode::LABEL_BODY_ELBOW_PRIMARY:
	case PXCGesture::GeoNode::LABEL_BODY_ELBOW_SECONDARY:
		return L"elbow";
	default:
		return L"undefined";
	}
}

wstring get_gesture_name(PXC_Geo_Label i_label){
	switch (i_label){
	case PXC_Gesture_Data::LABEL_NAV_SWIPE_LEFT:
		return L"swipe_left";
	case PXC_Gesture_Data::LABEL_NAV_SWIPE_RIGHT:
		return L"swipe_right";
	case PXC_Gesture_Data::LABEL_NAV_SWIPE_DOWN:
		return L"swipe_down";
	case PXC_Gesture_Data::LABEL_NAV_SWIPE_UP:
		return L"swipe_up";
	case PXC_Gesture_Data::LABEL_POSE_BIG5:
		return L"big_five";
	case PXC_Gesture_Data::LABEL_POSE_PEACE:
		return L"peace_sign";
	case PXC_Gesture_Data::LABEL_POSE_THUMB_DOWN:
		return L"thumb_down";
	case PXC_Gesture_Data::LABEL_POSE_THUMB_UP:
		return L"thumb_up";
	case PXC_Gesture_Data::LABEL_HAND_CIRCLE:
		return L"hand_circle";
	case PXC_Gesture_Data::LABEL_HAND_WAVE:
		return L"hand_wave";
	default:
		return L"undefined";
	}
}

wstring get_side(PXC_Geo_Label i_label){
	switch (i_label){
	case PXCGesture::GeoNode::LABEL_BODY_ELBOW_PRIMARY:
	case PXCGesture::GeoNode::LABEL_BODY_HAND_PRIMARY:
		return L"primary";
	case PXCGesture::GeoNode::LABEL_BODY_HAND_SECONDARY:
	case PXCGesture::GeoNode::LABEL_BODY_ELBOW_SECONDARY:
		return L"secondary";
	default:
		return L"undefined";
	}
}



}