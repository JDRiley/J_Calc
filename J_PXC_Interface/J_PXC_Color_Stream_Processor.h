#ifndef J_COLOR_STREAM_PROCESSOR_H
#define J_COLOR_STREAM_PROCESSOR_H

#include "J_PXC_Fwd_Decl.h"
//
#include"J_PXC_Stream.h"


namespace jomike{


class J_PXC_Color_Stream_Processor : public J_Stream_Processor{
public:
	typedef std::function<void(j_ubyte*, Image_Format
		, int width, int height)> Color_Stream_Callback_t;

	void add_color_stream_notification_callback(Color_Stream_Callback_t);


	J_PXC_Color_Stream_Processor(J_PXC_Stream_Shared_t i_stream);
	~J_PXC_Color_Stream_Processor();
	void push_data()override;

private:
#ifdef VS_2013
	PXC_Image* M_image = nullptr;
#else
	PXC_Image* M_image;
	void default_initialization();
#endif // VS_2013
	ex_array<j_ubyte> M_stream_buffer;
	ex_array<Color_Stream_Callback_t> M_color_stream_callbacks;
	void process_color_image();
	void derived_process_image_async(PXC_Image*, PXC_SyncPoint*)override;
	void derived_finish_processing()override;
	int M_width;
	int M_height;

	///void push_depth_stream()
	/*																			*/
	/*		Pushes out the current color stream									*/
	/*																			*/

	void push_color_stream();
	void reverse_color_stream(const int NUM_COLOR_STREAM_CHANNELS);
	ex_array<Color_Stream_Callback_t> M_depth_stream_callbacks;

	void alert_mirror_status_change()override;
};
}

#endif //J_COLOR_STREAM_PROCESSOR_H

