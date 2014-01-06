#include "Boolean_Testers.h"


namespace jomike{


bool Boolean_Tester::eval(const j_value& )const{
	throw J_Argument_Error("This Boolean Operation Does Not Support One Arg");
}

bool Boolean_Tester::eval(const j_value& , const j_value& )const{
	throw J_Argument_Error("This Boolean Operation Does Not Support Two Args");
}

Boolean_Tester* Boolean_Tester::get_copy()const{
	return new Boolean_Tester(*this);
}


bool equal_tester::eval(const j_value& irk_left, const j_value& irk_right)const{
	return irk_left == irk_right;
}


bool not_equal_tester::eval(const j_value& irk_left, const j_value& irk_right)const{
	return irk_left != irk_right;
}

}



