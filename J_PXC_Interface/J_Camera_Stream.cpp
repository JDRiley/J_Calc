#include "J_Camera_Stream.h"
//
#include "J_Camera_Manager.h"

//
#include <iostream>

//
#include <J_UI/J_UI_Model.h>



using std::string;
using std::cerr; using std::endl;
using std::wstring;
using std::transform;
using std::min;
using std::numeric_limits;
namespace jomike{

static Instance_Pointer<J_UI_Model> s_model;




J_Camera_Stream::J_Camera_Stream(const J_Rectangle& irk_rectangle, Image_Format i_format)
:J_Image_Pane(irk_rectangle, i_format){
#ifndef VS_2013
	default_initialization();
#endif //!VS_2013
}


#ifndef VS_2013

void J_Camera_Stream::default_initialization(){
	M_camera_filter_fade_status = false;
}

#endif //!VS_2013

J_Camera_Stream::~J_Camera_Stream(){

}


void J_Camera_Stream::update(){
	J_Image_Pane::update();

	camera_filter_fade();
}

void J_Camera_Stream::camera_filter_fade(){
	static bool s_was_fading = false;
	static const j_dbl sk_fade_time_up = 1.0;
	static const j_dbl sk_fade_time_down = 1.5;
	static const j_dbl sk_fade_time_total = 2.5;
	static j_dbl s_time_since_start;

	static const J_UI_Color sk_fill_color = (J_RED*0.12f + J_BLUE*0.02f);
	if (!M_camera_filter_fade_status){
		return;
	}
	j_uint id = get_ID();
	if (!s_was_fading && M_camera_filter_fade_status){
		s_time_since_start = j_get_time();
		s_was_fading = true;
		s_model->notify_fill_visibility_status(id, true);
	}
	j_dbl time = j_get_time() - s_time_since_start;

	if (time > sk_fade_time_total){
		broadcast_current_state();
		s_was_fading = false;
		M_camera_filter_fade_status = false;
	}else if (time > sk_fade_time_up){
		s_model->notify_fill_color(id,
			sk_fill_color.alpha(static_cast<j_float>(1.0f 
			- (time-sk_fade_time_up)/ sk_fade_time_down)));

	}else{
		assert(time <  sk_fade_time_up);
		s_model->notify_fill_color(id, (sk_fill_color
			*static_cast<j_float>(time / sk_fade_time_up)).alpha(1.0f));
	}

}




};