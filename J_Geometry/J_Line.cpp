#include "J_Line.h"


namespace jomike{

void J_Line::set_line(const J_Line& i_line){
	*this = i_line;
}

J_Line::J_Line(){

}

J_Line::J_Line(j_float i_x1, j_float i_y1, j_float i_x2, j_float i_y2){
	M_x1 = i_x1;
	M_y1 = i_y1;
	M_x2 = i_x2;
	M_y2 = i_y2;
}


void J_Line::set_points(j_float i_x1, j_float i_y1, j_float i_x2, j_float i_y2){
	set_line(J_Line(i_x1, i_y1, i_x2, i_y2));
}


J_Line* J_Line::get_copy()const {
	return new J_Line(*this);
}

j_float J_Line::area()const {
	return 0.0f;
}

j_float J_Line::perimeter()const {
	assert(!"No definition for perimeter for line");
	return 0.0f;
}

bool J_Line::is_inside(j_float, j_float)const {
	return false;
}


void J_Line::set_center(j_float , j_float ){
	assert(!"Trying to set center of a line");
}

j_float J_Line::y2()const{
	return M_y2;
}

j_float J_Line::x2()const{
	return M_x2;
}

j_float J_Line::y1()const{
	return M_y1;
}

j_float J_Line::x1()const{
	return M_x1;
}

}



