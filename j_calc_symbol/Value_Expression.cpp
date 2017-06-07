#include "Value_Expression.h"



namespace jomike{



Value_Expression::Value_Expression(const yy::location& irk_loc, j_value i_value)
	: j_calc_expression(irk_loc, i_value.symbol_type()),M_value(i_value) {

}

j_value Value_Expression::derived_get_value(const Arguments& /*i_args*/)const{
	return M_value;
}

Value_Expression* Value_Expression::move_copy(){
	return  new Value_Expression(std::move(*this));
}

bool Value_Expression::has_value()const {
	return M_value.value_status();
}

Value_Expression* Value_Expression::get_copy()const {
	return new Value_Expression(*this);
}

void Value_Expression::process(){

}

void Value_Expression::alert_symbol_scope_set(){

}

void Value_Expression::set_value(j_value i_value){
	M_value = i_value;
}

}



