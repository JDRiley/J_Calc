#include "Unary_Operator_Expression.h"


namespace jomike{


Unary_Operator_Expression::~Unary_Operator_Expression(){
	delete M_expression;
}

bool Unary_Operator_Expression::has_value()const {
	assert(M_expression);
	return M_expression->has_value();
}

Unary_Operator_Expression::Unary_Operator_Expression(Unary_Operator_Expression&& irv_right): j_expression(std::move(irv_right)){
	M_expression = irv_right.M_expression->move_copy();
}

Unary_Operator_Expression::Unary_Operator_Expression(const Unary_Operator_Expression& irk_src) : j_expression(irk_src){
	M_expression = irk_src.M_expression->get_copy();
}

Unary_Operator_Expression::Unary_Operator_Expression(j_expression* i_expression) 
: j_expression(i_expression->symbol_type()){
	M_expression = i_expression;
}

const j_expression& Unary_Operator_Expression::base_expression()const{
	assert(M_expression);
	return *M_expression;
}

j_expression& Unary_Operator_Expression::base_expression(){
	assert(M_expression);
	return *M_expression;
}

}