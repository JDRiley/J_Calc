#include "Pre_Decrement_Expression.h"


namespace jomike{



Pre_Decrement_Expression::Pre_Decrement_Expression(j_expression* i_expression)
	:Unary_Expression(i_expression){
	calculate_type();
}

void Pre_Decrement_Expression::process(){
	base_expression().set_value(--base_expression().get_value());
}


j_value Pre_Decrement_Expression::derived_get_value(const Arguments& i_args)const{
	
	
	return base_expression().get_value(i_args);
}

Pre_Decrement_Expression* Pre_Decrement_Expression::move_copy(){
	return  new Pre_Decrement_Expression(std::move(*this));
}


Pre_Decrement_Expression* Pre_Decrement_Expression::get_copy()const {
	return new Pre_Decrement_Expression(*this);
}


Pre_Decrement_Expression::~Pre_Decrement_Expression(){

}

j_symbol* Pre_Decrement_Expression::make_non_referenced()const {
	j_symbol* new_expr = base_expression().make_non_referenced();
	j_value new_value = --base_expression().get_value();
	base_expression().set_value(new_value);
	new_expr->set_value(new_value);
	return new_expr;
}



}



