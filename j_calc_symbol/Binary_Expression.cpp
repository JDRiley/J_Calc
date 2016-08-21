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

Binary_Expression::Binary_Expression(const yy::location& irk_loc
	, j_expression* i_left_expression, j_expression* i_right_expression
	, Operators i_operator)
:j_expression(irk_loc, i_left_expression->symbol_type()){
	M_left_expression = i_left_expression;
	M_right_expression = i_right_expression;
	M_operator = i_operator;
}

Binary_Expression::Binary_Expression(const Binary_Expression& irk_source)
:j_expression(irk_source){
	M_left_expression = irk_source.M_left_expression->get_copy();
	M_right_expression = irk_source.M_right_expression->get_copy();
	M_operator = irk_source.M_operator;
}

Binary_Expression::Binary_Expression(Binary_Expression&& irr_right)
: j_expression(std::move(irr_right)){
	M_left_expression = irr_right.M_left_expression->move_copy();
	M_right_expression = irr_right.M_right_expression->move_copy();
	M_operator = irr_right.M_operator;
}

Binary_Expression::~Binary_Expression(){
	delete M_left_expression;
	delete M_right_expression;
}

j_expression& Binary_Expression::right_expression()const{
	return *M_right_expression;
}


j_expression& Binary_Expression::left_expression()const{
	return *M_left_expression;
}

void Binary_Expression::alert_symbol_scope_set(){
	
	M_left_expression->set_symbol_scope(&symbol_scope());
	M_right_expression->set_symbol_scope(&symbol_scope());
}

Operators Binary_Expression::operator_enum()const{
	return M_operator;
}

void Binary_Expression::process(){
	M_left_expression->process();
	M_right_expression->process();
}


}



