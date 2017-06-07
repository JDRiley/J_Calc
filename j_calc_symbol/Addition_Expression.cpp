#include "Addition_Expression.h"


namespace jomike{



Addition_Expression::Addition_Expression(const yy::location& irk_loc
	, j_calc_expression* i_left_expression, j_calc_expression* i_right_expression)
	:Binary_Expression(irk_loc, i_left_expression, i_right_expression, Operators::PLUS){}

bool Addition_Expression::has_value()const {
	return left_expression().has_value() && right_expression().has_value();
}

j_value Addition_Expression::derived_get_value(const Arguments& i_args)const {
	j_value return_val = left_expression().get_value(i_args);
	return_val += right_expression().get_value(i_args);
	return return_val;
}

}



