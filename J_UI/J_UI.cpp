#include "J_UI.h"
//
#include <J_OpenGl_Functions.h>
//
#include "J_View.h"
//
#include <J_Frame_Counter.h>
namespace jomike{

J_UI::J_UI(){
	M_frame_counter = J_Frame_Counter_Shared_t(new J_Frame_Counter);
}

J_UI& J_UI::get_instance(){
	static J_UI ui_instance;
	return ui_instance;
}

jomike::j_dbl J_UI::time()const{
	return M_time;
}

void J_UI::update(){
	assert(M_frame_counter);
	M_time = j_get_time();
	M_frame_counter->count_frame();
}



void J_UI::set_cursor_pos(J_View_Weak_t i_view, j_dbl i_x_pos, j_dbl i_y_pos){
	if(!M_cursor_positions.count(i_view)){
		M_cursor_positions[i_view] = J_Cursor_Position_Shared_t(new J_Cursor_Position);
	}
	assert(!i_view.expired());
	M_cursor_positions[i_view]
		->set_cursor_position(i_view.lock()->get_window(), i_x_pos, i_y_pos);
}

const J_Cursor_Position& J_UI::cursor_pos(J_View_Weak_t i_view){
	if(auto cursor = M_cursor_positions[i_view]){
		return *cursor;
	}
	J_Cursor_Position_Shared_t new_cusor(new J_Cursor_Position);
	M_cursor_positions[i_view] = new_cusor;
	return *new_cusor;
}

J_UI::~J_UI(){

}


void J_Cursor_Position::set_cursor_position(j_window_t i_window, j_dbl i_x_pos, j_dbl i_y_pos){
	M_cursor_pos.first = static_cast<int>(i_x_pos);
	M_cursor_pos.second = static_cast<int>(i_y_pos);
	M_cursor_pos_fl = screen_coords(i_window, M_cursor_pos);
}

Pen_Pos_FL_t J_Cursor_Position::cursor_pos_fl()const{
	return M_cursor_pos_fl;
}

Pen_Pos_Int_t J_Cursor_Position::cursor_pos()const{
	return M_cursor_pos;
}

j_dbl J_UI::fps()const{
	assert(M_frame_counter);
	return M_frame_counter->fps();
}

jomike::j_dbl get_j_ui_time(){
	return J_UI::get_instance().time();
}

}



