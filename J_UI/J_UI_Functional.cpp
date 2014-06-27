#include "J_UI_Functional.h"
//
#include "J_UI_Object.h"
//
#include "J_UI_Box.h"

//Utilities
#include <cassert>
namespace jomike{

bool J_UI_Box_Position_FO::operator()(
	const J_UI_Box_Shared_t& irk_left, const J_UI_Box_Shared_t& irk_right)const{
	j_float compare_val = irk_right->y1() - irk_left->y1();

	if(compare_val > 0){
		return false;
	}else if(compare_val < 0){
		return true;
	}else{
		assert(!compare_val);
		return (irk_left->x1() < irk_right->x1());
	}
}

bool J_UI_Box_Position_FO::operator()(Pen_Pos_FL_t i_left_pos, const J_UI_Box_Shared_t& irk_right)const{
	if(i_left_pos.second > irk_right->y1()){
		return false;
	}

	return (i_left_pos.first < irk_right->x1());
}

bool J_UI_Box_Position_FO::operator()(const J_UI_Box_Shared_t& irk_left, Pen_Pos_FL_t i_right_pos)const{
	if(irk_left->y1() > i_right_pos.second){
		return false;
	}

	return (irk_left->x1() < i_right_pos.first);
}



}

