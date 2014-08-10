#include "J_Frame.h"
//
#include <J_UI/J_Shape.h>

namespace jomike{

J_Frame::J_Frame(j_uint i_width, j_uint i_height, const J_Rectangle& irk_rectangle)
: M_width(i_width), M_height(i_height){
	M_frame_over_base_x_ratio = irk_rectangle.width() / 2.0f;
	M_frame_over_base_y_ratio = irk_rectangle.height() / 2.0f;
	
	M_in_base_pos = {irk_rectangle.x1(), irk_rectangle.y1()};

}

jomike::j_float J_Frame::to_y_screen(int i_pixels)const{
	return 2.0f*i_pixels / M_height;
}

jomike::j_float J_Frame::to_x_screen(int i_pixels)const{
	return 2.0f* i_pixels / M_width;
}

jomike::j_uint J_Frame::height()const{
	return M_height;
}

jomike::j_uint J_Frame::width()const{
	return M_width;
}

Pen_Pos_FL_t J_Frame::to_base_pen_pos(Pen_Pos_FL_t i_pen_pos)const{
	j_float frame_x_size = 1.0f + i_pen_pos.first;
	j_float frame_y_size = 1.0f + i_pen_pos.second;

	return{M_in_base_pos.first + M_frame_over_base_x_ratio* frame_x_size
		, M_in_base_pos.second + M_frame_over_base_y_ratio*frame_y_size};
}

j_float J_Frame::to_base_y_screen(j_float i_y_screen)const{
	return i_y_screen*M_frame_over_base_y_ratio;
}

j_float J_Frame::to_base_x_screen(j_float i_x_screen)const{
	return i_x_screen*M_frame_over_base_x_ratio;
}

Pen_Pos_FL_t J_Frame::to_frame_pen_pos(Pen_Pos_FL_t i_pen_pos)const{
	return {to_frame_x_pos(i_pen_pos.first), to_frame_y_pos(i_pen_pos.second)};
}

j_float J_Frame::to_frame_x_pos(j_float i_base_pos)const{
	return (i_base_pos - M_in_base_pos.first) / M_frame_over_base_x_ratio - 1.0f;
}

j_float J_Frame::to_frame_y_pos(j_float i_base_pos)const{
	return (i_base_pos - M_in_base_pos.second) / M_frame_over_base_y_ratio - 1.0f;
}

void J_Frame::to_base_screen(J_Rectangle* i_rec)const{
	i_rec->set_pos(to_base_pen_pos({i_rec->x1(), i_rec->y1()}));
	i_rec->set_width(to_base_x_screen(i_rec->width()));
	i_rec->set_height(to_base_y_screen(i_rec->height()));
}


}



