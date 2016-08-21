#include "Variable_Reference_Symbol.h"
//
#include "j_expression.h"

namespace jomike{



jc_string_t Variable_Reference_Symbol::get_display_name(){
	if(has_value()){
		return get_value().to_str();
	}
	return name();
}

Variable_Reference_Symbol* Variable_Reference_Symbol::get_copy()const {
	return new Variable_Reference_Symbol(*this);
}

Variable_Reference_Symbol* Variable_Reference_Symbol::move_copy(){
	return  new Variable_Reference_Symbol(std::move(*this));
}

j_value Variable_Reference_Symbol::derived_get_value(const Arguments& )const {
	assert(0);
	return j_value(0ll, J_Unit());
}

bool Variable_Reference_Symbol::has_value()const {
	assert(0);
	return false;
}

void Variable_Reference_Symbol::clear(){
	assert(0);
}



Variable_Reference_Symbol::Variable_Reference_Symbol(const yy::location& irk_loc
	, Type_Syntax* i_syntax, J_Symbol_Identifier* i_identifier, j_expression* i_expression)
	: j_declaration(irk_loc,i_syntax, i_identifier){
	M_expression = i_expression;
}

Variable_Reference_Symbol::Variable_Reference_Symbol(const Variable_Reference_Symbol& irk_src)
:j_declaration(irk_src){
	if(irk_src.M_expression){
		M_expression = irk_src.M_expression->get_copy();
	}
}

Variable_Reference_Symbol::Variable_Reference_Symbol(Variable_Reference_Symbol&& irv_src)
	:j_declaration(std::move(irv_src)){
	if(irv_src.M_expression){
		M_expression = irv_src.M_expression->move_copy();
	}
}

void Variable_Reference_Symbol::process(){
	if(M_expression){
		M_expression->process();
	}
}

void Variable_Reference_Symbol::alert_symbol_scope_set(){
	if(M_expression){
		M_expression->set_symbol_scope(&symbol_scope());
	}
}

}



