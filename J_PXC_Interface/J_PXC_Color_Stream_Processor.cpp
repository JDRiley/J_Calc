#include "J_PXC_Color_Stream_Processor.h"
//
#include "J_PXC_Device.h"
//
#include "J_Camera_Manager.h"
namespace jomike{
static J_PXC_Error_Manager s_pxc_error;
static Instance_Pointer<J_Camera_Manager> s_camera_manager;


void J_PXC_Color_Stream_Processor::push_data(){
	push_color_stream();
}

void J_PXC_Color_Stream_Processor::derived_process_image_async(PXC_Image* i_image, PXC_SyncPoint*){
	M_image = i_image;
}

void J_PXC_Color_Stream_Processor::derived_finish_processing(){
	auto image = *M_image;
	PXCImage::ImageInfo image_info;
	image->QueryInfo(&image_info);

	PXC_ImageData image_data;
	s_pxc_error = image->AcquireAccess(PXCImage::ACCESS_READ
		, PXCImage::COLOR_FORMAT_RGB24, &image_data);

	NO_PXC_ERROR;

	pxcBYTE* image_buffer_ptr = (image_data.pitches[0] >= 0) ? image_data.planes[0]
		: image_data.planes[0] + image_data.pitches[0] * M_height;




	const int NUM_COLOR_STREAM_CHANNELS = 3;
	M_stream_buffer.resize(M_height*M_width* NUM_COLOR_STREAM_CHANNELS);

	if(!image_buffer_ptr){
		return;
	}

	M_stream_buffer.assign(image_buffer_ptr
		, image_buffer_ptr + M_stream_buffer.size());

	if(mirror_status()){
		reverse_color_stream(NUM_COLOR_STREAM_CHANNELS);
	}

	image->ReleaseAccess(&image_data);
}

void J_PXC_Color_Stream_Processor::push_color_stream(){

	Image_Format format = mirror_status() ?
		Image_Format::RGB24_UBYTE : Image_Format::BGR24_UBYTE;
	for(auto& f_function_obj : M_color_stream_callbacks){
		f_function_obj(M_stream_buffer.data()
			, format, M_width, M_height);
	}
}
void J_PXC_Color_Stream_Processor::reverse_color_stream(const int NUM_COLOR_STREAM_CHANNELS){
	const int height = M_height;
	auto start_pos = M_stream_buffer.begin();
	int offset_val = M_width*NUM_COLOR_STREAM_CHANNELS;
	for(int i = 0; i < height; i++){
		auto end_pos = start_pos + offset_val;
		reverse(start_pos, end_pos);
		start_pos = end_pos;

	}
}



void J_PXC_Color_Stream_Processor
	::add_color_stream_notification_callback(Color_Stream_Callback_t i_callback){
		M_color_stream_callbacks.push_back(i_callback);
}

J_PXC_Color_Stream_Processor::J_PXC_Color_Stream_Processor(J_PXC_Stream_Shared_t i_stream){
#ifndef VS_2013
	default_initialization();
#endif //!VS_2013
	M_width = i_stream->width();
	M_height = i_stream->height();
	set_parent(i_stream);
}

#ifndef VS_2013
void J_PXC_Color_Stream_Processor::default_initialization(){
	M_image = nullptr;
}
#endif //!VS_2013


void J_PXC_Color_Stream_Processor::alert_mirror_status_change(){
	if(M_stream_buffer.empty()){
		return;
	}
	reverse_color_stream(3);
	push_data();
}

J_PXC_Color_Stream_Processor::~J_PXC_Color_Stream_Processor(){
	M_image = nullptr;
}




}



