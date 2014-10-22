#include "Void_Empty_Expression.h"


namespace jomike{



Void_Empty_Expression::Void_Empty_Expression():j_expression(Symbol_Types::VOID_TYPE){

}


j_value Void_Empty_Expression::derived_get_value(const Arguments& )const {
	return j_value::void_type();
}

Void_Empty_Expression* Void_Empty_Expression::move_copy(){
	return  new Void_Empty_Expression(std::move(*this));
}

Void_Empty_Expression* Void_Empty_Expression::get_copy()const {
	return new Void_Empty_Expression(*this);
}

bool Void_Empty_Expression::has_value()const {
	return true;
}

void Void_Empty_Expression::process(){

}

void Void_Empty_Expression::alert_symbol_scope_set(){

}

}



