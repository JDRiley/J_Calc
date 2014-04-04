#include "Variable_Symbol.h"
//
#include "j_expression.h"
//
#include "Type_Syntax.h"
//
#include "Value_Expression.h"

namespace jomike{



void Variable_Symbol::clear(){
	assert(0);
}

J_UI_String Variable_Symbol::get_display_name(){
	if(has_value()){
		return get_value().to_str();
	}
	return name();
}

j_value Variable_Symbol::derived_get_value(const Arguments& )const{
	return M_value;
}

Variable_Symbol::Variable_Symbol(Type_Syntax* i_syntax, J_Symbol_Identifier* i_identifier)
	:j_declaration(i_syntax, i_identifier){
	
}

Variable_Symbol::Variable_Symbol(
	Type_Syntax* i_syntax, J_Symbol_Identifier* i_identifier, const j_expression& i_expression)
	:j_declaration(i_syntax, i_identifier){
	assert(i_expression.has_value());
	M_value = type_syntax().convert_value(i_expression.get_value());

}

bool Variable_Symbol::has_value()const {
	return M_value.value_status();
}

j_expression* Variable_Symbol::as_expression(){
	return new Value_Expression(M_value);
}

void Variable_Symbol::set_value(j_value i_value){
	M_value = type_syntax().convert_value(i_value);
}

}



