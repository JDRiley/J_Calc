#include "J_PXC_Depth_Stream_Processor.h"
//
#include "J_Camera_Manager.h"

namespace jomike{
static J_PXC_Error_Manager s_pxc_error;
static Instance_Pointer<J_Camera_Manager> s_camera_manager;
void J_PXC_Depth_Stream_Image_Processor::derived_finish_processing(){
	process_depth_image();
}

void J_PXC_Depth_Stream_Image_Processor::process_depth_image(){
	auto image = *M_image;

	PXC_ImageData depth_data;
	s_pxc_error = image->AcquireAccess(PXCImage::ACCESS_READ
		, PXCImage::COLOR_FORMAT_RGB32, &depth_data);

	NO_PXC_ERROR;
	int plane_index = 0;
	pxcBYTE* image_buffer_ptr = (depth_data.pitches[plane_index] >= 0)
		? depth_data.planes[plane_index] :
		depth_data.planes[plane_index]
		+ depth_data.pitches[plane_index] * M_width;

	int num_pixels = M_width * M_height;
	const int NUM_BUFFER_CHANNELS = 4;
	M_depth_stream_buffer.resize(num_pixels * NUM_BUFFER_CHANNELS);

	static_assert(NUM_BUFFER_CHANNELS == 4, "Channel Snaffu");

	for(int i = 0; i < num_pixels; i++){
		M_depth_stream_buffer[NUM_BUFFER_CHANNELS * i] = image_buffer_ptr[4 * i] / 2;
		M_depth_stream_buffer[NUM_BUFFER_CHANNELS * i + 1] = image_buffer_ptr[4 * i + 1] / 2;
		M_depth_stream_buffer[NUM_BUFFER_CHANNELS * i + 2] = image_buffer_ptr[4 * i + 2];
		M_depth_stream_buffer[NUM_BUFFER_CHANNELS * i + 3]
			= static_cast<j_ubyte>(M_alpha*image_buffer_ptr[4 * i + 3]);

		//j_float x_position
		//	= static_cast<j_float>(i % (depth_image_width()))
		//	/ depth_image_width();

		//if(abs(x_position - 0.5f) < 0.0005f){
		//	j_ubyte* cur_pixel = &M_depth_stream_buffer[NUM_BUFFER_CHANNELS*i];
		//	const j_ubyte max_val = numeric_limits<j_ubyte>::max();
		//	//Blue
		//	cur_pixel[2] = cur_pixel[3] = max_val;
		//	cur_pixel[0] = cur_pixel[1] = 0;
		//}
	}
	s_pxc_error = image->ReleaseAccess(&depth_data);
	NO_PXC_ERROR;
//	project_depth_to_color_stream();
}


void J_PXC_Depth_Stream_Image_Processor::push_depth_stream(){
	for(auto& f_function_obj : M_depth_stream_callbacks){
		f_function_obj
			(M_depth_stream_buffer.data(), M_width, M_height);
	}
}

void J_PXC_Depth_Stream_Image_Processor::derived_process_image_async(PXC_Image* i_image, PXC_SyncPoint*){
	M_image = i_image;
}

void J_PXC_Depth_Stream_Image_Processor::push_data(){
	push_depth_stream();
}

J_PXC_Depth_Stream_Image_Processor::J_PXC_Depth_Stream_Image_Processor(J_PXC_Stream_Shared_t i_stream){
#ifndef VS_2013
	default_initialization();
#endif //VS_2013
	M_width = i_stream->width();
	M_height = i_stream->height();
	set_parent(i_stream);
}

void J_PXC_Depth_Stream_Image_Processor
::add_depth_stream_notification_callback(Depth_Stream_Image_Callback_t i_callback){
	M_depth_stream_callbacks.push_back(i_callback);
}

void J_PXC_Depth_Stream_Image_Processor::set_depth_stream_alpha(j_float i_alpha){
	assert(between_inclusive(i_alpha, 0.0f, 1.0f));
	M_alpha = i_alpha;
}

#ifndef VS_2013
void J_PXC_Depth_Stream_Image_Processor::default_initialization(){
	M_image = nullptr;
	M_alpha = 1.0f;
}
#endif




void J_PXC_Depth_Stream_Depth_Processor::fill_depth_value(PXCPoint3DF32* i_depth_pos){
	assert(i_depth_pos);
	i_depth_pos->z 
		= static_cast<j_float>(M_depth_stream_buffer
			[static_cast<int>(i_depth_pos->y*M_width + i_depth_pos->x)]);
}

J_PXC_Depth_Stream_Depth_Processor::J_PXC_Depth_Stream_Depth_Processor(J_PXC_Stream_Shared_t i_stream){
#ifndef VS_2013
	default_initialization();
#endif //!VS_2013
	M_width = i_stream->width();
	M_height = i_stream->height();
	set_parent(i_stream);
}

#ifndef VS_2013
void J_PXC_Depth_Stream_Depth_Processor::default_initialization(){
	M_image = nullptr;
}
#endif //!VS_2013

void J_PXC_Depth_Stream_Depth_Processor
	::add_depth_stream_notification_callback(Depth_Stream_Depth_Callback_t i_callback){
		M_depth_stream_callbacks.push_back(i_callback);
}



void J_PXC_Depth_Stream_Depth_Processor::push_depth_values(){
	for(auto& f_function_obj : M_depth_stream_callbacks){
		f_function_obj
			(M_depth_stream_buffer.data(), M_width, M_height);
	}
}

void J_PXC_Depth_Stream_Depth_Processor::derived_process_image_async(PXC_Image* i_image, PXC_SyncPoint*){
	M_image = i_image;
}

void J_PXC_Depth_Stream_Depth_Processor::derived_finish_processing(){
	process_depth_values();
}

void J_PXC_Depth_Stream_Depth_Processor::push_data(){
	push_depth_values();
}

void J_PXC_Depth_Stream_Depth_Processor::process_depth_values(){
	auto image = *M_image;

	PXC_ImageData depth_data;
	s_pxc_error = image->AcquireAccess(PXCImage::ACCESS_READ
		, PXCImage::COLOR_FORMAT_DEPTH, &depth_data);

	NO_PXC_ERROR;
	int plane_index = 0;

	pxcI32 pitch = depth_data.pitches[plane_index];
	j_short* image_buffer_ptr = reinterpret_cast<j_short*>((pitch >= 0)
		? depth_data.planes[plane_index] :
		depth_data.planes[plane_index]
		+ depth_data.pitches[plane_index] * M_width);

	int num_pixels = M_width * M_height;
	M_depth_stream_buffer.resize(num_pixels);


	M_depth_stream_buffer.assign(image_buffer_ptr, image_buffer_ptr + num_pixels);
	
	s_pxc_error = image->ReleaseAccess(&depth_data);
	NO_PXC_ERROR;
}

J_PXC_Depth_Stream_Depth_Processor::~J_PXC_Depth_Stream_Depth_Processor(){
	M_image = nullptr;

}



}



