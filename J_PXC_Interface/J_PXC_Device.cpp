#include "J_PXC_Device.h"
//
#include "J_PXC_Stream.h"
//
#include "J_Camera_Manager.h"
//
#include <J_Utile.h>

namespace jomike{
static J_PXC_Error_Manager s_pxc_error;
static Instance_Pointer<J_Camera_Manager> s_camera_manager;

J_PXC_Device::J_PXC_Device(PXC_Device i_device, j_uint i_capture_id, j_uint i_device_index
	, const std::wstring& irk_name)
	:M_name(irk_name), M_capture_id(i_capture_id), M_device_index(i_device_index){
#ifndef VS_2013
	default_initialization();
#endif // !VS_2013

	M_device = i_device;

}

#ifndef VS_2013
void J_PXC_Device::default_initialization(){
	M_depth_stream_count = 0;
	M_color_stream_count = 0;
	M_device = nullptr;
	M_projector = nullptr;
}
#endif //!VS_2013

/*This to be the only function that creates a stream*/
J_PXC_Stream_Shared_t J_PXC_Device::create_stream(int i_width, int i_height
	, Image_Format i_format){


	M_device_properties = Device_Properties(get_device());
	
	PXC_Video_Stream stream;
	PXC_Stream_Info stream_info;
	PXC_Video_Stream_Profile stream_profile;
	PXCCapture::VideoStream::DataDesc::StreamDesc stream_desc;
	for(int stream_index = 0; stream_index < 100; ++stream_index){
		s_pxc_error = get_device()->CreateStream(stream_index, &stream);

		if(pxc_error()){
			pxc_clear_error();
			break;
		}

		ex_array<PXC_Video_Stream_Profile>
			profiles(get_video_stream_profiles(stream));
		PXC_Video_Stream_Profile* profile = get_closest_profile_match(profiles, i_format
			, i_width, i_height);

		
	
	
		if(!profile){
			stream->Release();
			stream = nullptr;
			continue;
		}
		s_pxc_error = stream->SetProfile(profile);
	
		set_to_zero(&stream_info);
		stream->QueryStream(&stream_info);

		if(pxc_error()){
			pxc_clear_error();
			continue;
		}
	
		
		stream->QueryProfile(&stream_profile);
		stream_desc.sizeMin.width = stream_profile.imageInfo.width;
		stream_desc.sizeMin.height = stream_profile.imageInfo.height;
		stream_desc.sizeMax.width = stream_profile.imageInfo.width;
		stream_desc.sizeMax.height = stream_profile.imageInfo.height;
		stream_desc.format = stream_profile.imageInfo.format;
		break;
	}
	if(!stream){
		return J_PXC_Stream_Shared_t();
	}
	J_PXC_Stream_Shared_t new_stream
		= (new J_PXC_Stream(stream, shared_from_this()))->make_shared_ptr();
	new_stream->init();

	if(is_profile_color(new_stream->format())){
		++M_color_stream_count;
	} else{
		++M_depth_stream_count;
	}

	if(M_depth_stream_count && M_color_stream_count){
		init_projection();
	}

	return new_stream;
}

PXC_Device J_PXC_Device::get_device(){
	if(M_device){
		return M_device;
	}
	PXCSession::ImplDesc device_description = zero_set<PXCSession::ImplDesc>();
	device_description.group = PXCSession::IMPL_GROUP_SENSOR;
	device_description.subgroup = PXCSession::IMPL_SUBGROUP_VIDEO_CAPTURE;
	PXCSession::ImplDesc capture_description;
	s_pxc_error
		= s_camera_manager->get_pxc_session()
		.QueryImpl(&device_description, M_capture_id, &capture_description);
	PXC_Capture capture;
	s_pxc_error = s_camera_manager->get_pxc_session()
		.CreateImpl<PXCCapture>(&capture_description, &capture);
	s_pxc_error = capture->CreateDevice(M_device_index, &M_device);
	capture->Release();
	return M_device;
}


void J_PXC_Device::set_device_capabilities(PXC_Device_Capabilities* i_capabilities){
	M_device = M_device ? M_device : get_device();


	for(int i = 0; i < PXCCapture::Device::DEVCAP_LIMIT; i++){
		PXC_Device_Property i_property = i_capabilities[i].label;
		if(!i_property){
			continue;
		}

		i_property = static_cast<PXC_Device_Property>(abs(i_property));
		M_device->QueryProperty(i_property, &i_capabilities[i].value);
	}
}

void J_PXC_Device::set_device_capabilities(PXC_Device_Capabilities* i_capabilities)const{
	assert(M_device);


	for(int i = 0; i < PXCCapture::Device::DEVCAP_LIMIT; i++){
		PXC_Device_Property i_property = i_capabilities[i].label;
		if(!i_property){
			continue;
		}

		i_property = static_cast<PXC_Device_Property>(abs(i_property));
		M_device->QueryProperty(i_property, &i_capabilities[i].value);
	}
}

void J_PXC_Device::init_projection(){
	M_device = get_device();

	PXC_Device_Info device_info;
	M_device->QueryDevice(&device_info);


	J_PXC_RELEASE(M_projector);
	pxcUID uid;
	s_pxc_error
		= M_device->QueryPropertyAsUID(
		PXCCapture::Device::PROPERTY_PROJECTION_SERIALIZABLE, &uid);

	if(pxc_error()){
		pxc_clear_error();
		return;
	}

	/* Recreate the PXCProject instance */
	s_pxc_error = s_camera_manager->get_pxc_session().DynamicCast<PXCMetadata>()
		->CreateSerializable<PXCProjection>(uid, &M_projector);

}

PXC_Projection J_PXC_Device::projector(){
	if(!M_projector){
		init_projection();
	}
	return M_projector;
}

void J_PXC_Device::release_device(){
	J_PXC_RELEASE(M_device);
}

void J_PXC_Device::reset_state(){
	J_PXC_RELEASE(M_projector);
	release_device();
	M_depth_stream_count = M_color_stream_count = 0;
}


}



