#include "J_Shape.h"
#include <J_Error.h>
//
#include <cassert>
//
#include <j_physical_constants.h>
#include <cmath>
using std::pow; using std::sqrtf;
namespace jomike{

void J_Shape::set_center(Pen_Pos_FL_t i_pen_pos){
	set_center(i_pen_pos.first, i_pen_pos.second);
}

bool J_Shape::is_inside(Pen_Pos_FL_t i_pen_pos)const{
	return is_inside(i_pen_pos.first, i_pen_pos.second);
}

bool J_Outline_Fill_Management::fill_visibility_status()const{ return M_fill_visibility_status; }

bool J_Outline_Fill_Management::outline_visibility_status()const{ return M_outline_visibility_status; }

void J_Outline_Fill_Management::set_outline_thickness(j_float i_thickness){
	M_outline_thickness = i_thickness;
}

void J_Outline_Fill_Management::set_fill_visibility_status(bool i_status){
	M_fill_visibility_status = i_status;
}

void J_Outline_Fill_Management::set_outline_visibility_status(bool i_status){
	M_outline_visibility_status = i_status;
}
j_float J_Outline_Fill_Management::outline_thickness()const{
	return M_outline_thickness;
}

void J_Outline_Fill_Management::set_outline_and_fill_visibility_status(bool i_status){
	set_outline_visibility_status(i_status);
	set_fill_visibility_status(i_status);
}


#ifndef VS_2013
J_Outline_Fill_Management::J_Outline_Fill_Management(){
	default_initialization();
}

void J_Outline_Fill_Management::default_initialization(){
	M_outline_thickness = 1.0f;
	M_outline_visibility_status
		= M_fill_visibility_status = false;
}

#endif //!VS_2013





//---------------------------------------------------------------------------------------
J_Rectangle::J_Rectangle():M_x_pos(0), M_y_pos(0), M_width(0), M_height(0){}

J_Rectangle::J_Rectangle(j_float i_x, j_float i_y, j_float i_width, j_float i_height)
	:M_x_pos(i_x), M_y_pos(i_y), M_width(i_width), M_height(i_height){}

j_float J_Rectangle::x1()const{return M_x_pos;}
j_float J_Rectangle::x2()const{return M_x_pos + M_width;}
j_float J_Rectangle::y1()const{
	return M_y_pos + M_height;
}
j_float J_Rectangle::y2()const{return M_y_pos;}


J_Rectangle* J_Rectangle::get_copy()const{
	return new J_Rectangle(*this);
}

void J_Rectangle::set_box(j_float i_x1, j_float i_x2, j_float i_y1, j_float i_y2){
	if(!(i_y2 < i_y1) || !(i_x2 > i_x1)){
		throw J_Argument_Error("Invalid Args given to J_Rectangle set_box()");
	}
	M_x_pos = i_x1;
	M_y_pos = i_y2;
	
	M_width = i_x2 - i_x1;
	
	M_height = i_y1 - i_y2;
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
	bool answer =  (x1() <= i_x) && (x2() >= i_x) && (y1() >= i_y) && (y2() <= i_y);
	return answer;
}

j_float J_Rectangle::width()const{return M_width;}

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

}// namespace jomike