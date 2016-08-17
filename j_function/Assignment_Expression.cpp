#include "Assignment_Expression.h"


namespace jomike{



j_value Assignment_Expression::derived_get_value(const Arguments& i_args)const {

	return left_expression().get_value(i_args);
}

Assignment_Expression::Assignment_Expression(const yy::location& irk_loc
	, j_expression* i_left_expression, j_expression* i_right_expression)
	:Binary_Expression(irk_loc, i_left_expression, i_right_expression, Operators::ASSIGN){

}

Assignment_Expression* Assignment_Expression::move_copy(){
	return  new Assignment_Expression(std::move(*this));
}

Assignment_Expression* Assignment_Expression::get_copy()const {
	return new Assignment_Expression(*this);
}

bool Assignment_Expression::has_value()const {
	return right_expression().has_value();
}

void Assignment_Expression::process(){
	Binary_Expression::process();
	left_expression().set_value(right_expression().get_value());
}

}



