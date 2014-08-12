#include "j_expression.h"


namespace jomike{



j_expression::j_expression(Symbol_Types i_type):j_symbol(i_type){
	
}

j_expression::j_expression(
	Symbol_Types i_symbol_type, J_Symbol_Identifier* i_identifier) 
	: j_symbol(i_identifier, i_symbol_type){

}

j_expression::j_expression(const j_expression& irk_right) : j_symbol(irk_right){

}

j_expression* j_expression::as_expression(){
	return get_copy();
}

jomike::J_UI_String j_expression::get_display_name(){
	if(has_value()){
		return get_value().to_str();
	}
	return name();
}

j_expression::~j_expression(){

}



}



