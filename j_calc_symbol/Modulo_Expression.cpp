#include "Modulo_Expression.h"


namespace jomike{


Modulo_Expression::Modulo_Expression(const yy::location& irk_loc
	, j_expression* i_left_expression, j_expression* i_right_expression)
	:Binary_Expression(irk_loc, i_left_expression, i_right_expression, Operators::DIVIDE){}

bool Modulo_Expression::has_value()const {
	return left_expression().has_value() && right_expression().has_value();
}

j_value Modulo_Expression::derived_get_value(const Arguments& i_args)const {
	j_value return_val = left_expression().get_value(i_args);
	return_val %= right_expression().get_value(i_args);
	return return_val;
}

Modulo_Expression* Modulo_Expression::move_copy(){
	return  new Modulo_Expression(std::move(*this));
}

Modulo_Expression* Modulo_Expression::get_copy()const {
	return new Modulo_Expression(*this);
}


}



