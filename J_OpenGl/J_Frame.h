#ifndef J_FRAME_H
#define J_FRAME_H

#include "J_OpenGL_Fwd_Decl.h"
//
#include <J_UI/J_UI_Fwd_Decl.h>

namespace jomike{
class J_Frame{
public:
	J_Frame(
		j_uint i_width, j_uint i_height, const J_Rectangle& irk_rectangle);

	j_float to_x_screen(int i_pixels)const;

	j_float to_y_screen(int i_pixels)const;

	j_uint width()const;

	j_uint height()const;

	void to_base_screen(J_Rectangle* i_rec)const;

	Pen_Pos_FL_t to_base_pen_pos(Pen_Pos_FL_t i_pen_pos)const;
	Pen_Pos_FL_t to_frame_pen_pos(Pen_Pos_FL_t i_pen_pos)const;

	j_float to_base_x_screen(j_float i_x_screen)const;
	j_float to_base_y_screen(j_float i_y_screen)const;

	j_float to_frame_x_pos(j_float)const;
	j_float to_frame_y_pos(j_float)const;

private:
	j_uint M_width;
	j_uint M_height;

	j_float M_frame_over_base_x_ratio;
	j_float M_frame_over_base_y_ratio;
	Pen_Pos_FL_t M_in_base_pos;
};

}

#endif //J_FRAME_H


