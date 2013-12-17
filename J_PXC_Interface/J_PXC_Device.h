#ifndef J_PXC_DEVICE_H
#define J_PXC_DEVICE_H

#include "J_PXC_Fwd_Decl.h"
#include <ex_array.h>
#include "pxccapture.h"
//
#include <J_Image.h>


J_POINTER_DECL(PXCCapture::Device, PXC_Device)
J_POINTER_DECL(PXCCapture::VideoStream, PXC_Video_Stream)
J_POINTER_DECL(PXCProjection, PXC_Projection)
J_STRUCT_POINTER_DECL(PXCCapture::VideoStream::DataDesc, PXC_Video_Stream_Desc)

typedef PXCCapture::Device::Property PXC_Device_Property;

namespace jomike{
typedef PXCCapture::Device::DeviceCap PXC_Device_Capabilities;
typedef PXCCapture::DeviceInfo PXC_Device_Info;
typedef PXCCapture::Device::Property PXC_Device_Property;

typedef PXCImage::ImageData PXC_ImageData;
typedef PXCCapture::VideoStream::ProfileInfo PXC_Video_Stream_Profile;
typedef PXCCapture::Device::StreamInfo PXC_Stream_Info;



struct Device_Properties{
	Device_Properties(PXC_Device);
	Device_Properties();
	PXCPointF32 M_color_field_of_view;
	PXC_Device_Capabilities device_properties[PXCCapture::Device::DEVCAP_LIMIT];
	PXCPointF32 M_depth_field_of_view;
	PXCPointF32 M_color_principal_point;
	PXCPointF32 M_depth_principal_point;
	PXCPointF32 M_color_focal_length;
	PXCPointF32 M_depth_focal_length;
	pxcF32 M_depth_unit_in_micro_meters;


};

class J_PXC_Device : public std::enable_shared_from_this<J_PXC_Device>{
public:
	J_PXC_Device(PXC_Device, j_uint i_capture_id, j_uint i_device_index, const std::wstring&);
	J_PXC_Stream_Shared_t create_stream(int width, int height, Image_Format);
	void set_device_capabilities(PXC_Device_Capabilities* i_capabilities);
	void set_device_capabilities(PXC_Device_Capabilities* i_capabilities)const;
	PXC_Projection projector();
	void release_device();
	void reset_state();
#ifdef VS_2013
	J_PXC_Device(const J_PXC_Device&) = delete;
	J_PXC_Device& operator=(const J_PXC_Device&) = delete;
#endif // VS_2013
private:
	std::wstring M_name;
	

#ifdef VS_2013
	int M_depth_stream_count = 0;
	int M_color_stream_count = 0;
	PXC_Device M_device = nullptr;
	PXC_Projection M_projector = nullptr;


#else
	int M_depth_stream_count;
	int M_color_stream_count;
	PXC_Device M_device;
	PXC_Projection M_projector;
	void default_initialization();
	J_PXC_Device(const J_PXC_Device&);
	J_PXC_Device& operator=(const J_PXC_Device&);

#endif // VS_2013
	Device_Properties M_device_properties;
	j_uint M_capture_id;
	j_uint M_device_index;

	PXC_Device get_device();
	void init_projection();


};



ex_array<PXC_Video_Stream_Profile> get_video_stream_profiles(PXC_Video_Stream);


PXC_Video_Stream_Profile*
get_closest_profile_match(ex_array<PXC_Video_Stream_Profile>&
, Image_Format, j_uint i_width, j_uint i_height);


bool is_profile_color(const PXC_Video_Stream_Profile& i_profile);

j_dbl frames_per_second(const PXC_Video_Stream_Profile&);


/*
typedef PXCGesture::Alert* PXC_Alert;
typedef PXCGesture::Gesture PXC_Gesture_Data;
typedef PXCGesture::GeoNode PXC_GeoNode;
typedef PXC_GeoNode::Label PXC_Geo_Label;
*/


}

#endif