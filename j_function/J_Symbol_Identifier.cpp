#include "J_Symbol_Identifier.h"


namespace jomike{



J_Symbol_Identifier::J_Symbol_Identifier(const J_UI_String& irk_name)
	:j_symbol_component(Symbol_Types::NAMED), M_name(irk_name){

}

J_UI_String J_Symbol_Identifier::get_display_name(){
	return M_name;
}

J_Symbol_Identifier* J_Symbol_Identifier::move_copy(){
	return  new J_Symbol_Identifier(std::move(*this));
}

J_Symbol_Identifier* J_Symbol_Identifier::get_copy()const {
	return new J_Symbol_Identifier(*this);
}

void J_Symbol_Identifier::set_name(const J_UI_String& irk_name){
	M_name = irk_name;
}

const J_UI_String& J_Symbol_Identifier::identifier_name()const{
	return M_name;
}

}



