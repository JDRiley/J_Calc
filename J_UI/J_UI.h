#ifndef J_UI_H
#define J_UI_H

#include "J_UI_Fwd_Decl.h"
//
#include <j_map.h>


namespace jomike{
class J_UI{
public:
	static J_UI& get_instance();
	void update();
	j_dbl time()const;
	virtual void set_cursor_pos(J_View_Shared_t i_view, j_dbl i_x_pos, j_dbl i_y_pos);
	const J_Cursor_Position& cursor_pos(J_View_Shared_t)const;
	j_dbl fps()const;
	~J_UI();
private:
	J_UI();
	j_dbl M_time = 0.0f;

	typedef j_map<J_View_Shared_t, J_Cursor_Position_Shared_t> Cursor_Position_Cont_t;
	Cursor_Position_Cont_t M_cursor_positions;
	J_Frame_Counter_Shared_t M_frame_counter;
};

j_dbl get_j_ui_time();

class J_Cursor_Position{
public:
	void set_cursor_position(j_window_t i_window, j_dbl i_x_pos, j_dbl i_y_pos);
	Pen_Pos_FL_t cursor_pos_fl()const;
	Pen_Pos_t cursor_pos()const;

private:
	Pen_Pos_FL_t M_cursor_pos_fl;
	Pen_Pos_t M_cursor_pos;

};


}

#endif //J_UI_H


