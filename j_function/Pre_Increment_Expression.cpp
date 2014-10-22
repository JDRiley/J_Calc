#include "Pre_Increment_Expression.h"


namespace jomike{

Pre_Increment_Expression::Pre_Increment_Expression(j_expression* i_expression):Unary_Expression(i_expression){

}

j_value Pre_Increment_Expression::derived_get_value(const Arguments& i_args)const{
	base_expression().set_value(++base_expression().get_value());
	return base_expression().get_value(i_args);
}

Pre_Increment_Expression* Pre_Increment_Expression::get_copy()const {
	return new Pre_Increment_Expression(*this);
}

Pre_Increment_Expression* Pre_Increment_Expression::move_copy(){
	return  new Pre_Increment_Expression(std::move(*this));
}

j_symbol* Pre_Increment_Expression::make_non_referenced()const {
	j_symbol* new_expr = base_expression().make_non_referenced();
	j_value new_value = ++base_expression().get_value();
	base_expression().set_value(new_value);
	new_expr->set_value(new_value);
	return new_expr;
}



}



