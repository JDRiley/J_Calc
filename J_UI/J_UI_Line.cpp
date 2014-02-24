#include "J_UI_Line.h"
//
#include "J_UI_Model.h"

namespace jomike{
static Instance_Pointer<J_UI_Model> s_model;



void J_UI_Line::set_line(const J_Line& i_line){
	J_Line::set_line(i_line);
	s_model->notify_line_data(get_ID(), *this);
}

J_UI_Line::J_UI_Line(j_float i_x1, j_float i_y1, j_float i_x2, j_float i_y2):J_Line(i_x1, i_y1, i_x2, i_y2){

}

void J_UI_Line::broadcast_current_state()const {
	J_UI_Object::broadcast_current_state();
	s_model->notify_line_data(get_ID(), *this);
}



}



