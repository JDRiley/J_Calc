#include "Subtraction_Expression.h"


namespace jomike{


Subtraction_Expression::Subtraction_Expression(
	j_expression* i_left_expression, j_expression* i_right_expression)
	:Binary_Expression(i_left_expression, i_right_expression, Operators::MINUS){

}



bool Subtraction_Expression::has_value()const {
	return left_expression().has_value() && right_expression().has_value();
}

j_value Subtraction_Expression::derived_get_value(const Arguments& i_args)const {
	j_value return_val = left_expression().get_value(i_args);
	return_val -= right_expression().get_value(i_args);
	return return_val;
}

Subtraction_Expression* Subtraction_Expression::move_copy(){
	return  new Subtraction_Expression(std::move(*this));
}

Subtraction_Expression* Subtraction_Expression::get_copy()const {
	return new Subtraction_Expression(*this);
}

Subtraction_Expression::~Subtraction_Expression(){

}


}



