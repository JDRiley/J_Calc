#include "j_symbol_component.h"


namespace jomike{

static int g_symbol_ids = 0;

j_symbol_component::j_symbol_component(
	Symbol_Types i_symbol_type, const J_UI_String& irk_name /*= J_UI_String("%Unnamed")*/)
	:M_ID(++g_symbol_ids), M_name(irk_name), M_type(i_symbol_type){

}

const J_UI_String& j_symbol_component::name()const{
	return M_name;
}

void j_symbol_component::set_name(const J_UI_String& irk_string){
	M_name = irk_string;
}

j_symbol_component::~j_symbol_component(){

}

jomike::Symbol_Types j_symbol_component::symbol_type()const{
	return M_type;
}

}



