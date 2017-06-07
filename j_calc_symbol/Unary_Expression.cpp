#include "Unary_Expression.h"


namespace jomike{


Unary_Expression::~Unary_Expression(){
	delete M_expression;
}

bool Unary_Expression::has_value()const {
	assert(M_expression);
	return M_expression->has_value();
}

Unary_Expression::Unary_Expression(Unary_Expression&& irv_right): j_calc_expression(std::move(irv_right)){
	if(irv_right.M_expression){
		M_expression = irv_right.M_expression->move_copy();
	} else{
		M_expression = nullptr;
	}
}

Unary_Expression::Unary_Expression(const Unary_Expression& irk_src) : j_calc_expression(irk_src){
	if(irk_src.M_expression){
		M_expression = irk_src.M_expression->get_copy();
	} else{
		M_expression = nullptr;
	}
}

Unary_Expression::Unary_Expression(const yy::location& irk_loc, j_calc_expression* i_expression) 
: j_calc_expression(irk_loc, i_expression->symbol_type()){
	M_expression = i_expression;


	if(M_expression && M_expression->has_type_syntax()){
		set_type_syntax(M_expression->type_syntax());
	}
}


j_calc_expression& Unary_Expression::base_expression()const{
	assert(M_expression);
	return *M_expression;
}

j_calc_expression& Unary_Expression::base_expression(){
	assert(M_expression);
	return *M_expression;
}

bool Unary_Expression::has_expression()const{
	return M_expression;
}

void Unary_Expression::alert_symbol_scope_set(){

	if(has_expression()){
		base_expression().set_symbol_scope(&symbol_scope());
	}
}

void Unary_Expression::calculate_type(){
	if(M_expression->has_type_syntax()){
		set_type_syntax(M_expression->type_syntax());
	}
}

void Unary_Expression::process(){
	M_expression->process();
}

j_calc_symbol* Unary_Expression::make_non_referenced()const {
	return M_expression->make_non_referenced();
}

}