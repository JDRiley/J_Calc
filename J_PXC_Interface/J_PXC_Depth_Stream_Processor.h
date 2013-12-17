#ifndef J_PXC_DEPTH_STREAM_PROCESSOR_H
#define J_PXC_DEPTH_STREAM_PROCESSOR_H

#include <J_Fwd_Decl.h>
//
#include "J_PXC_Fwd_Decl.h"
//
#include "J_Stream_Processor.h"
//
#include "J_PXC_Device.h"
namespace jomike{

class J_PXC_Depth_Stream_Image_Processor : public J_Stream_Processor{
public:
	typedef std::function <void(j_ubyte*, int width, int height)> 
		Depth_Stream_Image_Callback_t;

	
	J_PXC_Depth_Stream_Image_Processor(J_PXC_Stream_Shared_t i_stream);
	void add_depth_stream_notification_callback(Depth_Stream_Image_Callback_t);
	void push_data()override;
	void set_depth_stream_alpha(j_float i_alpha);
private:
#ifdef VS_2013
	PXC_Image* M_image = nullptr;
	j_float M_alpha = 1.0f;
#else
	PXC_Image* M_image;
	j_float M_alpha;
	void default_initialization();
#endif

	ex_array<j_ubyte> M_depth_stream_buffer;
	
	void derived_process_image_async(PXC_Image*, PXC_SyncPoint*)override;
	void derived_finish_processing()override;
	int M_width;
	int M_height;
	///void push_depth_stream()
	/*																			*/
	/*		Pushes out the current color stream									*/
	/*																			*/
	void push_depth_stream();
	void process_depth_image();
	ex_array<Depth_Stream_Image_Callback_t> M_depth_stream_callbacks;
};


class J_PXC_Depth_Stream_Depth_Processor : public J_Stream_Processor{
public:
	typedef std::function <void(j_short*, int width, int height)>
		Depth_Stream_Depth_Callback_t;

	
	J_PXC_Depth_Stream_Depth_Processor(J_PXC_Stream_Shared_t i_stream);
	void add_depth_stream_notification_callback(Depth_Stream_Depth_Callback_t);
	void push_data()override;
	void set_depth_stream_alpha(j_float i_alpha);
	void fill_depth_value(PXCPoint3DF32* i_depth_pos);
	~J_PXC_Depth_Stream_Depth_Processor();
private:
#ifdef VS_2013
	PXC_Image* M_image = nullptr;
#else
	PXC_Image* M_image;
	void default_initialization();
#endif // VS_2013

	
	ex_array<j_short> M_depth_stream_buffer;
	void derived_process_image_async(PXC_Image*, PXC_SyncPoint*)override;
	void derived_finish_processing()override;
	int M_width;
	int M_height;
	///void push_depth_stream()
	/*																			*/
	/*		Pushes out the current color stream									*/
	/*																			*/
	void push_depth_values();
	void process_depth_values();
	ex_array<Depth_Stream_Depth_Callback_t> M_depth_stream_callbacks;
};


}

#endif //J_PXC_DEPTH_STREAM_PROCESSOR_H

