#include "J_Image_Pane.h"
//
#include "J_UI_Model.h"
//
#include <J_Utile.h>

namespace jomike{

static Instance_Pointer<J_UI_Model> s_model;

J_Image_Pane::J_Image_Pane(const J_Rectangle& ik_rec
	, Image_Format i_format, int i_width, int i_height)
	:J_UI_Box(ik_rec), M_width(i_width), M_height(i_height){
	assert(image_height() >= 0);
	assert(image_width() >= 0);
	M_format = i_format;
	s_model->notify_image_input_format(get_ID(), i_format);
	s_model->notify_new_image_buffer(get_ID(), nullptr);
}

void J_Image_Pane::set_buffer(const j_ubyte* i_buffer){
	if(i_buffer){
		s_model->notify_new_image_buffer(get_ID(), i_buffer);
	}else{
		s_model->notify_image_clear(get_ID());
	}
}

void J_Image_Pane::set_image_width(int i_width){
	assert(i_width >= 0);
	M_width = i_width;
	s_model->notify_image_width(get_ID(), i_width);
}

void J_Image_Pane::set_image_height(int i_height){
	assert(i_height >= 0);
	M_height = i_height;
	s_model->notify_image_height(get_ID(), i_height);
}


int J_Image_Pane::image_width()const{return M_width;}

int J_Image_Pane::image_height()const{return M_height;}

void J_Image_Pane::broadcast_current_state()const{
	J_UI_Box::broadcast_current_state();

	s_model->notify_image_width(get_ID(), M_width);
	s_model->notify_image_height(get_ID(), M_height);
	s_model->notify_image_input_format(get_ID(), M_format);
}

void J_Image_Pane::set_input_format(Image_Format i_format){
	if(M_format == i_format){
		return;
	}

	M_format = i_format;
	s_model->notify_image_input_format(get_ID(), M_format);
}

Image_Format J_Image_Pane::input_format()const{
	return M_format;
}



}