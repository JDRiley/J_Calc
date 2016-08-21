#include "Variable_Symbol.h"
//
#include "j_expression.h"
//
#include "Type_Syntax.h"
//
#include "Value_Expression.h"
//
#include "J_Symbol_Scope.h"
namespace jomike{



void Variable_Symbol::clear(){
	assert(0);
}

jc_string_t Variable_Symbol::get_display_name(){
	if(has_value()){
		return get_value().to_str();
	}
	return name();
}

j_value Variable_Symbol::derived_get_value(const Arguments& )const{
	return M_value;
}

Variable_Symbol::Variable_Symbol(
	const yy::location& irk_loc, Type_Syntax* i_syntax, J_Symbol_Identifier* i_identifier)
	:j_declaration(irk_loc, i_syntax, i_identifier){
	
}

Variable_Symbol::Variable_Symbol(const yy::location& irk_loc
	, Type_Syntax* i_syntax, J_Symbol_Identifier* i_identifier, const j_expression& i_expression)
	:j_declaration(irk_loc, i_syntax, i_identifier){
	if(!i_expression.has_value()){
		throw J_Value_Error(L"Cannot Declare variable and assign it to expression with no value");
	}
	M_value = type_syntax().convert_value(i_expression.get_value());

}

bool Variable_Symbol::has_value()const {
	return M_value.value_status();
}

j_expression* Variable_Symbol::as_expression(){
	return new Value_Expression(location(), M_value);
}

void Variable_Symbol::set_value(j_value i_value){
	M_value = type_syntax().convert_value(i_value);
}

Variable_Symbol* Variable_Symbol::get_copy()const {
	return new Variable_Symbol(*this);
}

Variable_Symbol* Variable_Symbol::move_copy(){
	return  new Variable_Symbol(std::move(*this));
}

void Variable_Symbol::process(){
	symbol_scope().add_symbol(get_copy());
}

void Variable_Symbol::alert_symbol_scope_set(){

}

}



