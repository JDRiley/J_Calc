#include "Addition_Expression.h"


namespace jomike{



Addition_Expression::Addition_Expression(
	j_expression* i_left_expression, j_expression* i_right_expression)
	:j_expression(i_left_expression->symbol_type()){

	M_left_expression = i_left_expression;
	M_right_expression = i_right_expression;
}

bool Addition_Expression::has_value()const {
	return M_left_expression->has_value() && M_right_expression->has_value();
}

j_value Addition_Expression::derived_get_value(const Arguments& i_args)const {
	j_value return_val = M_left_expression->get_value(i_args);
	return_val += M_right_expression->get_value(i_args);
	return return_val;
}

}



