#include "J_Circle.h"
//
#include <j_physical_constants.h>

namespace jomike{

J_Circle::J_Circle(){
#ifndef VS_2013
	default_initialization();
#endif //!VS_2013
}

J_Circle::J_Circle(j_float i_x1, j_float i_y1, j_float i_radius)
: M_x_pos(i_x1), M_y_pos(i_y1), M_radius(i_radius){
	assert(radius() >= 0.0f);
}



j_float J_Circle::x1()const{ return M_x_pos; }

j_float J_Circle::y1()const{ return M_y_pos; }

J_Circle* J_Circle::get_copy()const{ return new J_Circle(*this); }

void J_Circle::set_circle(const J_Circle& irk_circle){
	set_circle(irk_circle.M_x_pos, irk_circle.M_y_pos, irk_circle.radius());
}

void J_Circle::set_center(j_float i_x_pos, j_float i_y_pos){
	M_x_pos = i_x_pos;
	M_y_pos = i_y_pos;
}

void J_Circle::set_radius(j_float i_radius){
	assert(i_radius >= 0.0f);
	M_radius = i_radius;
}

j_float J_Circle::radius()const{ return M_radius; }

void J_Circle::set_x(j_float i_x_pos){
	M_x_pos = i_x_pos;
}

void J_Circle::set_y(j_float i_y_pos){
	M_y_pos = i_y_pos;
}

bool J_Circle::is_inside(j_float i_x_pos, j_float i_y_pos)const{
	return M_radius >= sqrt(pow(i_x_pos - M_x_pos, 2) + pow(i_y_pos - M_y_pos, 2));
}

void J_Circle::set_circle(j_float i_x_pos, j_float i_y_pos, j_float i_radius){
	M_x_pos = i_x_pos;
	M_y_pos = i_y_pos;
	M_radius = i_radius;
}

j_float J_Circle::perimeter()const{
	return static_cast<j_float>(2 * PI * radius());
}

j_float J_Circle::area()const{
	return static_cast<j_float>(pow(radius(), 2)*PI);
}
#ifndef VS_2013

void J_Circle::default_initialization(){
	M_x_pos = 0.0f;
	M_y_pos = 0.0f;
	M_radius = 0.0f;
}
#endif //!VS_2013
}



