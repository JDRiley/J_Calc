#include "Letter_Box_String.h"
//
#include "J_UI_Letter_Box.h"

namespace jomike{

const j_float FLOAT_DELTA = 0.00001f;

void Letter_Box_String::draw(j_size_t i_start_pos, j_size_t i_end_pos)const{
	for(j_size_t i = i_start_pos; i < i_end_pos; i++){
		M_letter_boxes[i]->draw();
	}
}

void Letter_Box_String::push_back(J_UI_Letter_Box_Shared_t i_letter){
	M_letter_boxes.push_back(i_letter);
}

Letter_Box_String::const_iterator Letter_Box_String::begin()const{
	return M_letter_boxes.begin();
}

Letter_Box_String::const_iterator Letter_Box_String::end()const{
	return M_letter_boxes.end();
}

J_UI_Letter_Box_Shared_t Letter_Box_String::operator[](j_size_t i_index){
	return M_letter_boxes[i_index];
}

void Letter_Box_String::resize(j_size_t i_size){
	M_letter_boxes.resize(i_size);
}


Letter_Box_Pos_Order_Comp
	::Letter_Box_Pos_Order_Comp(j_float i_reference_line, j_float i_line_size)
		:M_line_size(i_line_size), M_reference_line(i_reference_line){}


int Letter_Box_Pos_Order_Comp::get_line_index(j_float i_float)const{
	int line_number = 0;

	while(i_float < (M_reference_line - line_number*M_line_size) - FLOAT_DELTA){
		++line_number;
	}

	if(line_number){
		return line_number;
	}

	if(i_float < M_reference_line){
		return line_number;
	}

	while(i_float >(M_reference_line - line_number*M_line_size) - FLOAT_DELTA){
		--line_number;
	}

	return line_number;
}

bool Letter_Box_Pos_Order_Comp
	::operator()(J_UI_Letter_Box_Shared_t ik_left, const Pen_Pos_FL_t& ik_right)const{
	int compare_val = get_line_index(ik_right.second) - get_line_index(ik_left->y2());
	if(compare_val < 0){
		return false;
	} else if(compare_val > 0){
		return true;
	} else if((ik_right.first - ik_left->x1()) > 0){
		return true;
	} else{
		return false;
	}
}

bool Letter_Box_Pos_Order_Comp
::operator()(const Pen_Pos_FL_t& ik_left,  J_UI_Letter_Box_Shared_t ik_right)const{
	int compare_val = get_line_index(ik_right->y2()) - get_line_index(ik_left.second);
	if(compare_val < 0){
		return false;
	} else if(compare_val > 0){
		return true;
	} else if((ik_right->x1() - ik_left.first) > 0){
		return true;
	} else{
		return false;
	}
}


}



