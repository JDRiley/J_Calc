#include "J_Outline_Fill_Management.h"
//
#include "J_Image/J_Image.h"

namespace jomike{


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



void J_Outline_Fill_Management::set_outline_color(const J_UI_Color& i_color){
	M_outline_color = i_color;
	derived_set_outline_color();
}

void J_Outline_Fill_Management::set_fill_color(const J_UI_Color& i_color){
	M_fill_color = i_color;
	derived_set_fill_color();
}

const J_UI_Color& J_Outline_Fill_Management::outline_color()const{
	return M_outline_color;
}

const J_UI_Color& J_Outline_Fill_Management::fill_color()const{
	return M_fill_color;
}


}



