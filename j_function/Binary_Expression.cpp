#include "Binary_Expression.h"


namespace jomike{



bool Binary_Expression::has_value()const {
	if(!M_left_expression->has_value()){
		return false;
	}

	if(!M_right_expression->has_value()){
		return false;
	}

	return true;
}

Binary_Expression::Binary_Expression(
	j_expression* i_left_expression, j_expression* i_right_expression)
:j_expression(i_left_expression->symbol_type()){
	M_left_expression = i_left_expression;
	M_right_expression = i_right_expression;

}

Binary_Expression::Binary_Expression(const Binary_Expression& irk_source)
:j_expression(irk_source){
	M_left_expression = irk_source.M_left_expression->get_copy();
	M_right_expression = irk_source.M_right_expression->get_copy();
}

Binary_Expression::Binary_Expression(Binary_Expression&& irr_right)
: j_expression(std::move(irr_right)){
	M_left_expression = irr_right.M_left_expression->move_copy();
	M_right_expression = irr_right.M_right_expression->move_copy();
}

Binary_Expression::~Binary_Expression(){
	delete M_left_expression;
	delete M_right_expression;
}

const j_expression& Binary_Expression::right_expression()const{
	return *M_right_expression;
}

j_expression& Binary_Expression::right_expression(){
	return *M_right_expression;
}

const j_expression& Binary_Expression::left_expression()const{
	return *M_left_expression;
}

j_expression& Binary_Expression::left_expression(){
	return *M_left_expression;
}

}


