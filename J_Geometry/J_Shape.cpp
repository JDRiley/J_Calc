#include "J_Shape.h"


namespace jomike{

void J_Shape::set_center(Pen_Pos_FL_t i_pen_pos){
	set_center(i_pen_pos.first, i_pen_pos.second);
}

bool J_Shape::is_inside(Pen_Pos_FL_t i_pen_pos)const{
	return is_inside(i_pen_pos.first, i_pen_pos.second);
}
}



