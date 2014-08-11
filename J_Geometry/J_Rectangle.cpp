#include "J_Rectangle.h"
//
#include <J_Utile.h>
//
#include <J_Error.h>
namespace jomike{

J_Rectangle::J_Rectangle():M_x_pos(0), M_y_pos(0), M_width(0), M_height(0){}

J_Rectangle::J_Rectangle(j_float i_x, j_float i_y, j_float i_width, j_float i_height)
	:M_x_pos(i_x), M_y_pos(i_y), M_width(i_width), M_height(i_height){}

j_float J_Rectangle::x1()const{return M_x_pos;}
j_float J_Rectangle::x2()const{return M_x_pos + M_width;}

j_float J_Rectangle::y1()const{return M_y_pos;}

j_float J_Rectangle::y2()const{
	return M_y_pos + M_height;
}


J_Rectangle* J_Rectangle::get_copy()const{
	return new J_Rectangle(*this);
}

void J_Rectangle::set_box(j_float i_x1, j_float i_x2, j_float i_y1, j_float i_y2){
	if(!(i_y1 <= i_y2) || !(i_x2 >= i_x1)){
		throw J_Argument_Error("Invalid Args given to J_Rectangle set_box()");
	}
	M_x_pos = i_x1;
	M_y_pos = i_y1;
	
	M_width = i_x2 - i_x1;
	
	M_height = i_y2 - i_y1;
}

void J_Rectangle::set_box(const J_Rectangle& irk_rec){
	*this = irk_rec;
}

j_float J_Rectangle::area()const{return M_width*M_height;}
j_float J_Rectangle::perimeter()const{return 2*M_width + 2*M_height;}

void J_Rectangle::set_center(j_float , j_float ){
	assert(!"Attempting to set Center of a box");
}

bool J_Rectangle::is_inside(j_float i_x, j_float i_y)const{
	return is_x_inside(i_x) && is_y_inside(i_y);
}

j_float J_Rectangle::width()const{
	return M_width;
}

j_float J_Rectangle::height()const{return M_height;}

void J_Rectangle::set_rectangle(const J_Rectangle& irk_source){*this = irk_source;}

void J_Rectangle::set_width(j_float i_width){
	assert(i_width >= 0.0f);
	M_width = i_width;
}

void J_Rectangle::set_height(j_float i_height){
	assert(i_height >= 0.0f);
	M_height = i_height;
}

void J_Rectangle::set_x(j_float i_x){
	M_x_pos = i_x;
}

void J_Rectangle::set_y(j_float i_y){
	M_y_pos = i_y;
}

bool J_Rectangle::is_y_inside(j_float i_y)const{
	return between_inclusive(i_y, y1(), y2());

}

bool J_Rectangle::is_x_inside(j_float i_x)const{
	return between_inclusive(i_x, x1(), x2());

}

void J_Rectangle::set_x2(j_float end_x_pos){
	set_width(end_x_pos - M_x_pos);
}

void J_Rectangle::set_pos(Pen_Pos_FL_t i_pos){
	set_rectangle(J_Rectangle(i_pos.first, i_pos.second, width(), height()));
}



}



